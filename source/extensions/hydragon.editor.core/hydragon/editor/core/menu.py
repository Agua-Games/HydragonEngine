"""
Hydragon Engine - Create and Context Menu Integration

Registers 'Hydragon' submenu under the top 'Create' menu and the right-click context menu.
"""

import os
from typing import Optional, List

try:
    import carb
    import omni.kit.app
    import omni.usd
    import omni.kit.commands
    import omni.kit.menu.utils
    import omni.kit.context_menu
    from omni.kit.menu.utils import MenuItemDescription
    from pxr import Sdf
    HAS_KIT = True
except ImportError:
    HAS_KIT = False
    carb = None

from .schemas import (
    HydragonActor,
    HydragonPlayerController,
    HydragonFollowCamera,
    HydragonChaserAI,
    HydragonTrigger,
    HydragonGameManager,
    HydragonUICanvas,
)


class HydragonMenuManager:
    """Manages Hydragon menus in Omniverse Kit top bar and context menus."""

    def __init__(self, ext_id: str):
        self._ext_id = ext_id
        self._create_menu_items = []
        self._context_menu_entries = []

    def startup(self):
        """Builds and registers menus."""
        if not HAS_KIT:
            return

        self._build_create_menu()
        self._build_context_menu()

    def shutdown(self):
        """Deregisters all menu items."""
        if not HAS_KIT:
            return

        if self._create_menu_items:
            try:
                omni.kit.menu.utils.remove_menu_items(self._create_menu_items, "Create")
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to remove Create menu items: {e}")
            self._create_menu_items = []

        if self._context_menu_entries:
            try:
                for entry in self._context_menu_entries:
                    try:
                        omni.kit.context_menu.remove_menu(entry)
                    except Exception:
                        pass
            except Exception as e:
                if carb:
                    carb.log_warn(f"[hydragon.editor.core] Failed to remove context menu items: {e}")
            self._context_menu_entries = []

    def _get_menu_glyph(self) -> Optional[str]:
        """Resolves gamepad SVG icon path for Hydragon menu entries."""
        try:
            # 1. Try extension data/assets/icons folder
            source_root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
            candidate = os.path.normpath(os.path.join(source_root, "data", "assets", "icons", "gamepad.svg")).replace("\\", "/")
            if os.path.exists(candidate):
                return candidate

            # 2. Try data_dir via extension manager
            data_dir = self._get_extension_data_dir()
            candidate = os.path.normpath(os.path.join(data_dir, "assets", "icons", "gamepad.svg")).replace("\\", "/")
            if os.path.exists(candidate):
                return candidate
        except Exception:
            pass
        return "gamepad.svg"

    # -------------------------------------------------------------------------
    # Asset Instantiation Helpers
    # -------------------------------------------------------------------------
    def _get_extension_data_dir(self) -> str:
        """Finds the data folder of hydragon.editor.core extension."""
        # 1. Try extension manager path
        try:
            manager = omni.kit.app.get_app_interface().get_extension_manager()
            ext_path = manager.get_extension_path(self._ext_id)
            if ext_path:
                candidate = os.path.join(ext_path, "data")
                if os.path.exists(candidate):
                    return candidate
        except Exception:
            pass

        # 2. Try source folder relative to this file: __file__ is in hydragon/editor/core/
        source_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
        candidate = os.path.join(source_dir, "data")
        if os.path.exists(candidate):
            return candidate

        # 3. Try fallback relative to current working directory
        cwd_candidate = os.path.abspath(os.path.join(os.getcwd(), "source", "extensions", "hydragon.editor.core", "data"))
        if os.path.exists(cwd_candidate):
            return cwd_candidate

        return candidate

    def _instantiate_asset(self, rel_asset_path: str, default_name: str, as_payload: bool = True):
        """Instances an asset USDA into the current stage using Payload (for actors/props) or Reference (for managers)."""
        if not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage()
        if not stage:
            if carb:
                carb.log_warn("[hydragon.editor.core] No active USD stage to instantiate asset.")
            return

        # Determine parent prim path
        selection = omni.usd.get_context().get_selection().get_selected_prim_paths()
        parent_path = selection[0] if selection and selection[0] != "/" else "/World"

        # Ensure unique prim path
        prim_path = f"{parent_path}/{default_name}"
        i = 1
        while stage.GetPrimAtPath(prim_path).IsValid():
            prim_path = f"{parent_path}/{default_name}_{i:02d}"
            i += 1

        data_dir = self._get_extension_data_dir()
        full_asset_path = os.path.normpath(os.path.join(data_dir, rel_asset_path)).replace("\\", "/")

        if not os.path.exists(full_asset_path):
            if carb:
                carb.log_error(f"[hydragon.editor.core] Asset file not found: {full_asset_path}")
            return

        usd_context = omni.usd.get_context()
        cmd_name = "CreatePayloadCommand" if as_payload else "CreateReferenceCommand"
        success = False
        try:
            success, _ = omni.kit.commands.execute(
                cmd_name,
                usd_context=usd_context,
                path_to=Sdf.Path(prim_path),
                asset_path=full_asset_path,
                instanceable=False,
            )
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] {cmd_name} error: {e}, falling back to direct USD API")

        prim = stage.GetPrimAtPath(prim_path)
        needs_fallback = (
            not prim.IsValid()
            or (as_payload and not prim.HasAuthoredPayloads())
            or (not as_payload and not prim.HasAuthoredReferences())
        )
        if needs_fallback:
            try:
                prim = stage.DefinePrim(Sdf.Path(prim_path), "Xform")
                if as_payload:
                    prim.GetPayloads().AddPayload(assetPath=full_asset_path)
                else:
                    prim.GetReferences().AddReference(assetPath=full_asset_path)
                success = True
            except Exception as e:
                if carb:
                    carb.log_error(f"[hydragon.editor.core] Direct USD fallback failed: {e}")

        omni.usd.get_context().get_selection().set_selected_prim_paths([prim_path], True)
        arc_type = "Payload" if as_payload else "Reference"
        if carb:
            carb.log_info(f"[hydragon.editor.core] Instantiated Hydragon asset ({arc_type}) at {prim_path}")

    # -------------------------------------------------------------------------
    # Schema Application Helpers
    # -------------------------------------------------------------------------
    def _apply_schema(self, schema_cls):
        """Applies a Hydragon API schema to all currently selected prims."""
        if not HAS_KIT:
            return

        stage = omni.usd.get_context().get_stage()
        selection = omni.usd.get_context().get_selection().get_selected_prim_paths()
        if not stage or not selection:
            if carb:
                carb.log_warn("[hydragon.editor.core] Select a prim first to apply schema.")
            return

        for path in selection:
            prim = stage.GetPrimAtPath(path)
            if prim and prim.IsValid():
                schema_cls.apply(prim)
                if carb:
                    carb.log_info(f"[hydragon.editor.core] Applied {schema_cls.SCHEMA_NAME} to {path}")

    # -------------------------------------------------------------------------
    # Menu Builders
    # -------------------------------------------------------------------------
    def _build_create_menu(self):
        """Builds top-bar Create -> Hydragon menu."""
        glyph = self._get_menu_glyph()
        sub_items = [
            # Gameplay Entities / Smart Assets
            MenuItemDescription(
                name="Player Ball",
                onclick_fn=lambda: self._instantiate_asset("assets/gameplay/hydragon_player_ball/player_ball.usda", "PlayerBall", as_payload=True)
            ),
            MenuItemDescription(
                name="Foe Ball",
                onclick_fn=lambda: self._instantiate_asset("assets/gameplay/hydragon_foe_ball/foe_ball.usda", "FoeBall", as_payload=True)
            ),
            MenuItemDescription(
                name="Goal Hole",
                onclick_fn=lambda: self._instantiate_asset("assets/gameplay/hydragon_goal_hole/goal_hole.usda", "GoalHole", as_payload=True)
            ),
            MenuItemDescription(
                name="Game Manager",
                onclick_fn=lambda: self._instantiate_asset("assets/gameplay/hydragon_game_manager/game_manager.usda", "GameManager", as_payload=False)
            ),
            MenuItemDescription(
                name="UI Canvas (Game HUD)",
                onclick_fn=lambda: self._instantiate_asset("assets/gameplay/hydragon_ui_canvas/ui_canvas.usda", "UICanvas", as_payload=False)
            ),
            MenuItemDescription(
                name="Character (Kowra)",
                onclick_fn=lambda: self._instantiate_asset("assets/characters/hydragon_character/hydragon_character.usda", "Character", as_payload=True)
            ),
            # Separator
            MenuItemDescription(name=""),
            # API Schemas Submenu
            MenuItemDescription(
                name="Apply Schemas",
                sub_menu=[
                    MenuItemDescription(
                        name="Apply HydragonActorAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonActor)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonPlayerControllerAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonPlayerController)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonFollowCameraAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonFollowCamera)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonChaserAIAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonChaserAI)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonTriggerAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonTrigger)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonGameAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonGameManager)
                    ),
                    MenuItemDescription(
                        name="Apply HydragonUICanvasAPI",
                        onclick_fn=lambda: self._apply_schema(HydragonUICanvas)
                    ),
                ]
            )
        ]

        self._create_menu_items = [
            MenuItemDescription(
                name="Hydragon",
                glyph=glyph,
                sub_menu=sub_items
            )
        ]

        omni.kit.menu.utils.add_menu_items(self._create_menu_items, "Create")
        if carb:
            carb.log_info("[hydragon.editor.core] Registered 'Hydragon' menu items in 'Create' menu.")

    def _build_context_menu(self):
        """Hooks into Kit's right-click context menu under CREATE and STAGE."""
        try:
            glyph = self._get_menu_glyph()
            # In omni.kit.widget.context_menu, submenus must use {"name": {"SubmenuTitle": [items]}}
            schemas_sub_menu = [
                {"name": "Apply HydragonActorAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonActor)},
                {"name": "Apply HydragonPlayerControllerAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonPlayerController)},
                {"name": "Apply HydragonFollowCameraAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonFollowCamera)},
                {"name": "Apply HydragonChaserAIAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonChaserAI)},
                {"name": "Apply HydragonTriggerAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonTrigger)},
                {"name": "Apply HydragonGameAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonGameManager)},
                {"name": "Apply HydragonUICanvasAPI", "onclick_fn": lambda *_: self._apply_schema(HydragonUICanvas)},
            ]

            hydragon_items = [
                {
                    "name": "Player Ball",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/gameplay/hydragon_player_ball/player_ball.usda", "PlayerBall", as_payload=True)
                },
                {
                    "name": "Foe Ball",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/gameplay/hydragon_foe_ball/foe_ball.usda", "FoeBall", as_payload=True)
                },
                {
                    "name": "Goal Hole",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/gameplay/hydragon_goal_hole/goal_hole.usda", "GoalHole", as_payload=True)
                },
                {
                    "name": "Game Manager",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/gameplay/hydragon_game_manager/game_manager.usda", "GameManager", as_payload=False)
                },
                {
                    "name": "UI Canvas (Game HUD)",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/gameplay/hydragon_ui_canvas/ui_canvas.usda", "UICanvas", as_payload=False)
                },
                {
                    "name": "Character (Kowra)",
                    "onclick_fn": lambda *_: self._instantiate_asset("assets/characters/hydragon_character/hydragon_character.usda", "Character", as_payload=True)
                },
                {"name": ""},
                {
                    "name": {"Apply Schemas": schemas_sub_menu}
                }
            ]

            context_dict = {
                "name": {"Hydragon": hydragon_items},
                "glyph": glyph,
            }

            # Register in "CREATE" context menu (right click in Stage / Viewport create submenu)
            entry1 = omni.kit.context_menu.add_menu(context_dict, "CREATE", "")
            self._context_menu_entries.append(entry1)

            if carb:
                carb.log_info("[hydragon.editor.core] Registered 'Hydragon' context menu with proper dictionary hierarchy.")
        except Exception as e:
            if carb:
                carb.log_warn(f"[hydragon.editor.core] Could not register context menu: {e}")

