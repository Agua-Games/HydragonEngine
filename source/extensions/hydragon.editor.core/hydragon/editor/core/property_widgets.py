"""
Hydragon Engine - Dedicated Property Window Widgets

Provides custom collapsible frames in Omniverse Kit's Property Window
for Hydragon OpenUSD API Schemas (Player Controller, Follow Camera,
Chaser AI, Actor, Trigger, and Game Manager).
"""

from typing import List, Optional

try:
    import carb
    import omni.kit.app
    import omni.kit.window.property as property_window
    from omni.kit.property.usd import UsdPropertiesWidget
    HAS_PROPERTY_WINDOW = True
except ImportError:
    HAS_PROPERTY_WINDOW = False
    UsdPropertiesWidget = object
    property_window = None

from .schemas import (
    HydragonActor,
    HydragonPlayerController,
    HydragonFollowCamera,
    HydragonChaserAI,
    HydragonTrigger,
    HydragonGameManager,
)


if HAS_PROPERTY_WINDOW:
    class HydragonBasePropertyWidget(UsdPropertiesWidget):
        """
        Base property widget for Hydragon API Schemas.
        Filters properties by namespace prefix and supports hierarchical selection
        (e.g., selecting either the root actor or the leaf geometry mesh in viewport).
        """

        def __init__(self, title: str, prefix: str, schema_check_fn):
            super().__init__(title=title, collapsed=False, enable_adapter=True)
            self._prefix: str = prefix
            self._schema_check_fn = schema_check_fn

        def _matches_schema(self, prim) -> bool:
            """Checks whether a prim has this schema or authors attributes with the target prefix."""
            if not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
                return False

            if self._schema_check_fn and self._schema_check_fn(prim):
                return True

            try:
                for prop in prim.GetProperties():
                    if prop.GetName().startswith(self._prefix):
                        return True
            except Exception:
                pass

            return False

        def on_new_payload(self, payload):
            """Determines whether to display this widget based on selected prim schemas or ancestors."""
            if not super().on_new_payload(payload):
                return False

            if not self._payload or len(self._payload) == 0:
                return False

            for prim_path in self._payload:
                prim = self._get_prim(prim_path)
                if not prim or not prim.IsValid():
                    continue

                # 1. Check prim directly
                if self._matches_schema(prim):
                    return True

                # 2. Check parent (e.g. user selected ball_mesh under /World/Player/geometry)
                parent = prim.GetParent() if hasattr(prim, "GetParent") else None
                if parent and parent.IsValid() and self._matches_schema(parent):
                    return True

                # 3. Check grandparent
                grandparent = parent.GetParent() if parent and hasattr(parent, "GetParent") else None
                if grandparent and grandparent.IsValid() and self._matches_schema(grandparent):
                    return True

            return False

        def _get_prim_properties(self, prim):
            """
            Queries properties from the prim. If the prim is a child mesh with no matching
            properties, falls back to parent prims where the API schema is authored.
            """
            props = super()._get_prim_properties(prim)
            matching = [p for p in props if p.GetName().startswith(self._prefix)]
            if matching:
                return props

            # Search parent hierarchy if child geometry was clicked in viewport
            parent = prim.GetParent() if hasattr(prim, "GetParent") else None
            while parent and parent.IsValid() and not parent.IsPseudoRoot():
                parent_props = parent.GetProperties()
                if any(p.GetName().startswith(self._prefix) for p in parent_props):
                    return parent_props
                parent = parent.GetParent()

            return props

        def _filter_props_to_build(self, props):
            """Filters USD properties to only show those belonging to this schema prefix."""
            return [p for p in props if p.GetName().startswith(self._prefix)]

        def _customize_props_layout(self, props):
            """Secondary layout hook ensuring only schema-prefixed UI entries are emitted."""
            return [p for p in props if getattr(p, "prop_name", "").startswith(self._prefix)]


    class HydragonPlayerPropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonPlayerControllerAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Player Controller",
                prefix="player:",
                schema_check_fn=HydragonPlayerController.is_applied,
            )


    class HydragonCameraPropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonFollowCameraAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Follow Camera",
                prefix="camera:",
                schema_check_fn=HydragonFollowCamera.is_applied,
            )


    class HydragonChaserAIPropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonChaserAIAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Chaser AI",
                prefix="ai:",
                schema_check_fn=HydragonChaserAI.is_applied,
            )


    class HydragonActorPropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonActorAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Actor",
                prefix="actor:",
                schema_check_fn=HydragonActor.is_applied,
            )


    class HydragonTriggerPropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonTriggerAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Trigger",
                prefix="trigger:",
                schema_check_fn=HydragonTrigger.is_applied,
            )


    class HydragonGamePropertyWidget(HydragonBasePropertyWidget):
        """Dedicated property panel for HydragonGameAPI."""
        def __init__(self):
            super().__init__(
                title="Hydragon Game Manager",
                prefix="game:",
                schema_check_fn=HydragonGameManager.is_applied,
            )

else:
    class HydragonBasePropertyWidget:
        def __init__(self, *args, **kwargs):
            pass

    class HydragonPlayerPropertyWidget(HydragonBasePropertyWidget):
        pass

    class HydragonCameraPropertyWidget(HydragonBasePropertyWidget):
        pass

    class HydragonChaserAIPropertyWidget(HydragonBasePropertyWidget):
        pass

    class HydragonActorPropertyWidget(HydragonBasePropertyWidget):
        pass

    class HydragonTriggerPropertyWidget(HydragonBasePropertyWidget):
        pass

    class HydragonGamePropertyWidget(HydragonBasePropertyWidget):
        pass


class HydragonPropertyWidgetsManager:
    """
    Manages registration and lifecycle of dedicated Property Window widgets
    for all Hydragon gameplay schemas. Uses deferred registration to ensure
    the Property Window is ready.
    """

    def __init__(self):
        self._registered: bool = False
        self._widgets: List[tuple] = []
        self._app_update_sub = None

    def startup(self):
        """Starts up widget manager and schedules registration as soon as property window is available."""
        if not HAS_PROPERTY_WINDOW or self._registered:
            return

        # Attempt immediate registration
        if self._try_register():
            return

        # If window not ready yet, listen to app update stream until ready
        try:
            app = omni.kit.app.get_app()
            if app:
                self._app_update_sub = app.get_update_event_stream().create_subscription_to_pop(
                    self._on_app_update
                )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to subscribe to app update for property widgets: {e}")

    def _on_app_update(self, e):
        """Polls every frame until the Property Window is ready, then registers and detaches."""
        if self._try_register():
            self._app_update_sub = None

    def _try_register(self) -> bool:
        """Attempts to register widgets with omni.kit.window.property.get_window()."""
        if self._registered or not property_window:
            return True

        try:
            w = property_window.get_window()
            if not w:
                return False

            self._widgets = [
                ("hydragon_player", HydragonPlayerPropertyWidget()),
                ("hydragon_camera", HydragonCameraPropertyWidget()),
                ("hydragon_ai", HydragonChaserAIPropertyWidget()),
                ("hydragon_actor", HydragonActorPropertyWidget()),
                ("hydragon_trigger", HydragonTriggerPropertyWidget()),
                ("hydragon_game", HydragonGamePropertyWidget()),
            ]

            for name, widget in self._widgets:
                w.register_widget("prim", name, widget)

            self._registered = True
            if carb:
                carb.log_info("[hydragon.editor.core] Successfully registered 6 dedicated Hydragon property widgets.")
            return True
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Error registering property widgets: {e}")
            return False

    def shutdown(self):
        """Unregisters all Hydragon property widgets from omni.kit.window.property."""
        if self._app_update_sub:
            self._app_update_sub = None

        if not HAS_PROPERTY_WINDOW or not self._registered:
            return

        try:
            w = property_window.get_window() if property_window else None
            if w:
                for name, _ in self._widgets:
                    w.unregister_widget("prim", name)
            self._widgets.clear()
            self._registered = False
            if carb:
                carb.log_info("[hydragon.editor.core] Unregistered Hydragon property widgets.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Failed to unregister property widgets: {e}")
