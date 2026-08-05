import carb
import omni.ext
import omni.ui as ui
import omni.kit.app
import omni.kit.menu.utils as menu_utils
from omni.kit.menu.utils import MenuItemDescription

class HydragonEditorCoreExtension(omni.ext.IExt):
    def on_startup(self, ext_id):
        carb.log_info("[hydragon.editor.core] Startup...")
        self._patch_menu_utils()
        
        # Keep track of UI window
        self._window = None
        self._menu_items = None
        
        # Register menu item
        self._menu_items = [
            MenuItemDescription(
                name="Performance Control Panel",
                onclick_fn=self._show_window
            )
        ]
        menu_utils.add_menu_items(self._menu_items, "Window")

    def on_shutdown(self):
        carb.log_info("[hydragon.editor.core] Shutdown...")
        
        # Clean up menu items
        if self._menu_items:
            try:
                menu_utils.remove_menu_items(self._menu_items, "Window")
            except Exception as e:
                carb.log_warn(f"[hydragon.editor.core] Failed to remove menu items: {e}")
            self._menu_items = None
            
        # Clean up window
        if self._window:
            self._window.destroy()
            self._window = None

    def _show_window(self):
        if self._window:
            self._window.visible = True
            self._window.focus()
            return

        # Create a beautiful custom utility window
        self._window = ui.Window("Performance & Editors Control", width=420, height=440)
        
        # Define groups of extensions with descriptions
        self._groups = [
            {
                "name": "Scene Optimizers & Validators",
                "desc": "Runs background checks on USD assets. Triggers heavy CPU notices on stage updates.",
                "exts": ["omni.scene.optimizer.bundle", "omni.asset_validator.ui"]
            },
            {
                "name": "AI & Navigation Paths",
                "desc": "Navmesh builders and motion path solvers running update cycles.",
                "exts": ["omni.anim.navigation.bundle", "omni.anim.motion_path.bundle"]
            },
            {
                "name": "Skeletal Animation Graph",
                "desc": "Animation state machines and blend trees.",
                "exts": ["omni.anim.graph.bundle"]
            },
            {
                "name": "Skeletal Retargeting Solver",
                "desc": "Active bone-to-bone skeletal retargeting calculation loops.",
                "exts": ["omni.anim.retarget.bundle"]
            },
            {
                "name": "Sequencer & Timeline Panels",
                "desc": "Timeline UI panels, curve editors, and non-linear tracks.",
                "exts": ["omni.kit.window.sequencer", "omni.anim.window.timeline"]
            },
            {
                "name": "Warp GPU Graph",
                "desc": "Action Graph integration node loop for Warp physics/GPU operations.",
                "exts": ["omni.warp.graph"]
            }
        ]

        ext_manager = omni.kit.app.get_app().get_extension_manager()

        with self._window.frame:
            with ui.VStack(spacing=15, padding=15):
                # Header info
                ui.Label("Select which editor features to run. Disabling unneeded editors immediately releases their CPU listeners and increases FPS.", 
                         style={"font_size": 13, "color": 0xFFCCCCCC}, word_wrap=True)
                
                # Checkbox list with a dark, clean panel styling
                with ui.ScrollingFrame(style={"background_color": 0xFF181818, "border_color": 0xFF2A2A2A, "border_width": 1, "border_radius": 4}):
                    with ui.VStack(spacing=12, padding=12):
                        for group in self._groups:
                            # Calculate if any extension in this group is currently active
                            is_enabled = any(ext_manager.is_extension_enabled(e) for e in group["exts"])
                            
                            with ui.HStack(height=45, spacing=12):
                                # Stylized checkbox
                                cb = ui.CheckBox()
                                cb.model.set_value(is_enabled)
                                
                                # Use closure helper to pass state change to corresponding group
                                def make_callback(g=group):
                                    def on_changed(model):
                                        self._toggle_group(g, model.get_value_as_bool())
                                    return on_changed
                                
                                cb.model.add_value_changed_fn(make_callback(group))
                                
                                with ui.VStack(spacing=2):
                                    ui.Label(group["name"], style={"font_size": 13, "font_weight": "bold", "color": 0xFFFFFFFF})
                                    ui.Label(group["desc"], style={"font_size": 10, "color": 0xFF888888}, word_wrap=True)
                
                # Quick Action Buttons
                with ui.HStack(height=35, spacing=10):
                    # Red button for max performance
                    btn_off = ui.Button("Disable All (Maximum FPS)", style={"background_color": 0xFF2B2B8F, "font_size": 12, "font_weight": "bold"})
                    btn_off.set_clicked_fn(lambda: self._toggle_all(False))
                    
                    # Green button for full feature workspace
                    btn_on = ui.Button("Enable All (Full Workspace)", style={"background_color": 0xFF2C7D32, "font_size": 12, "font_weight": "bold"})
                    btn_on.set_clicked_fn(lambda: self._toggle_all(True))

    def _toggle_group(self, group, enable):
        ext_manager = omni.kit.app.get_app().get_extension_manager()
        for ext_id in group["exts"]:
            curr = ext_manager.is_extension_enabled(ext_id)
            if curr != enable:
                carb.log_info(f"[hydragon.editor.core] Toggling extension '{ext_id}' to {enable}")
                # We use set_extension_enabled so the dependency solver handles it cleanly
                ext_manager.set_extension_enabled(ext_id, enable)

    def _toggle_all(self, enable):
        if not self._window:
            return
        ext_manager = omni.kit.app.get_app().get_extension_manager()
        for group in self._groups:
            self._toggle_group(group, enable)
        
        # Redraw the window to refresh checkbox states and configurations
        self._window.destroy()
        self._window = None
        self._show_window()

    def _patch_menu_utils(self):
        try:
            import omni.kit.menu.utils.app_menu as app_menu
            
            # Save original method just in case
            self._original_sort_menu_hook = app_menu.AppMenu.sort_menu_hook
            
            @staticmethod
            def patched_sort_menu_hook(merged_menu):
                from omni.kit.menu.utils.builder_utils import MenuItemDescription
                
                def priority_sort(menu_entry):
                    if hasattr(menu_entry, "priority"):
                        return menu_entry.priority
                    return 0

                def sort_sub_menu(menu_entry, max_depth=MenuItemDescription.MAX_DEPTH):
                    for item in menu_entry:
                        if item.sub_menu:
                            if isinstance(item.sub_menu, tuple):
                                carb.log_warn(f"[hydragon.editor.core] Extension menu item '{item.name}' has sub_menu as a tuple. Converting to list.")
                                item.sub_menu = list(item.sub_menu)
                            item.sub_menu.sort(key=priority_sort)
                            if max_depth:
                                sort_sub_menu(item.sub_menu, max_depth - 1)
                            else:
                                carb.log_warn("Recursive sub_menu, aborting sorting")

                for name in merged_menu:
                    sort_sub_menu(merged_menu[name])
                    
            app_menu.AppMenu.sort_menu_hook = patched_sort_menu_hook
            carb.log_info("[hydragon.editor.core] Successfully monkeypatched AppMenu.sort_menu_hook to protect against tuple sub_menus.")
            
        except Exception as e:
            carb.log_error(f"[hydragon.editor.core] Failed to monkeypatch omni.kit.menu.utils: {e}")
