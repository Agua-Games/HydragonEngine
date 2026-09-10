try:
    import carb
    import omni.ext
    _IExt = omni.ext.IExt
except ImportError:
    carb = None
    omni = None
    class _IExt:
        pass

class HydragonEditorCoreExtension(_IExt):
    def on_startup(self, ext_id):
        if carb:
            carb.log_info("[hydragon.editor.core] Startup...")
        self._patch_menu_utils()

        from .menu import HydragonMenuManager
        self._menu_manager = HydragonMenuManager(ext_id)
        self._menu_manager.startup()

        from .player_controller import HydragonPlayerControllerSystem
        self._player_controller_system = HydragonPlayerControllerSystem()
        self._player_controller_system.startup()

        from .camera_controller import HydragonCameraControllerSystem
        self._camera_controller_system = HydragonCameraControllerSystem()
        self._camera_controller_system.startup()

        from .foes_controller import HydragonFoesControllerSystem
        self._foes_controller_system = HydragonFoesControllerSystem()
        self._foes_controller_system.startup()

        from .trigger_controller import HydragonTriggerSystem
        self._trigger_system = HydragonTriggerSystem()
        self._trigger_system.startup()

        from .property_widgets import HydragonPropertyWidgetsManager
        self._property_widgets_manager = HydragonPropertyWidgetsManager()
        self._property_widgets_manager.startup()

        from .game_hud import HydragonGameHUD
        self._game_hud = HydragonGameHUD()
        self._game_hud.startup()

        from .effects_controller import HydragonEffectsSystem
        self._effects_system = HydragonEffectsSystem()
        self._effects_system.startup()

        from .soundtrack_controller import HydragonSoundtrackSystem
        self._soundtrack_system = HydragonSoundtrackSystem()
        self._soundtrack_system.startup()

        from .force_volume_controller import HydragonForceVolumeSystem
        self._force_volume_system = HydragonForceVolumeSystem()
        self._force_volume_system.startup()

        from .kill_volume_controller import HydragonKillVolumeSystem
        self._kill_volume_system = HydragonKillVolumeSystem()
        self._kill_volume_system.startup()

        from .volume_viewport_manipulator import HydragonVolumeViewportOverlay
        self._volume_viewport_overlay = HydragonVolumeViewportOverlay(ext_id)
        self._volume_viewport_overlay.startup()

    def on_shutdown(self):
        if hasattr(self, "_volume_viewport_overlay") and self._volume_viewport_overlay:
            self._volume_viewport_overlay.shutdown()
            self._volume_viewport_overlay = None
        if hasattr(self, "_kill_volume_system") and self._kill_volume_system:
            self._kill_volume_system.shutdown()
            self._kill_volume_system = None
        if hasattr(self, "_force_volume_system") and self._force_volume_system:
            self._force_volume_system.shutdown()
            self._force_volume_system = None
        if hasattr(self, "_soundtrack_system") and self._soundtrack_system:
            self._soundtrack_system.shutdown()
            self._soundtrack_system = None
        if hasattr(self, "_effects_system") and self._effects_system:
            self._effects_system.shutdown()
            self._effects_system = None
        if hasattr(self, "_game_hud") and self._game_hud:
            self._game_hud.shutdown()
            self._game_hud = None
        if hasattr(self, "_property_widgets_manager") and self._property_widgets_manager:
            self._property_widgets_manager.shutdown()
            self._property_widgets_manager = None
        if hasattr(self, "_trigger_system") and self._trigger_system:
            self._trigger_system.shutdown()
            self._trigger_system = None
        if hasattr(self, "_foes_controller_system") and self._foes_controller_system:
            self._foes_controller_system.shutdown()
            self._foes_controller_system = None
        if hasattr(self, "_camera_controller_system") and self._camera_controller_system:
            self._camera_controller_system.shutdown()
            self._camera_controller_system = None
        if hasattr(self, "_player_controller_system") and self._player_controller_system:
            self._player_controller_system.shutdown()
            self._player_controller_system = None
        if hasattr(self, "_menu_manager") and self._menu_manager:
            self._menu_manager.shutdown()
            self._menu_manager = None
        if carb:
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
