import carb
import omni.ext

class HydragonEditorCoreExtension(omni.ext.IExt):
    def on_startup(self, ext_id):
        carb.log_info("[hydragon.editor.core] Startup...")
        self._patch_menu_utils()

    def on_shutdown(self):
        carb.log_info("[hydragon.editor.core] Shutdown...")

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
