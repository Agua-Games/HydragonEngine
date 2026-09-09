"""
Hydragon Engine - USD Gameplay API Schema Wrappers

Provides Pythonic accessors and authoring utilities for Hydragon custom API schemas
adhering to OpenUSD best practices and the fail-silent principle.
"""

from typing import Optional, List

try:
    from pxr import Usd, Sdf, Vt
    HAS_PXR = True
except ImportError:
    HAS_PXR = False
    Usd = None
    Sdf = None
    Vt = None


def _ensure_api_schema(prim, schema_name: str) -> None:
    """Helper to append an API schema name to the prim's apiSchemas list if not already present."""
    if not HAS_PXR or not prim:
        return

    try:
        current_schemas = []
        if hasattr(prim, "GetAppliedSchemas"):
            current_schemas = list(prim.GetAppliedSchemas() or [])

        api_schemas_meta = prim.GetMetadata("apiSchemas")
        if api_schemas_meta is not None:
            for attr_name in ("explicitItems", "prependedItems", "appendedItems", "addedItems", "orderedItems"):
                items = getattr(api_schemas_meta, attr_name, None)
                if items:
                    for item in items:
                        if item not in current_schemas:
                            current_schemas.append(item)
            if isinstance(api_schemas_meta, (list, tuple)):
                for item in api_schemas_meta:
                    if item not in current_schemas:
                        current_schemas.append(item)

        if schema_name not in current_schemas:
            current_schemas.append(schema_name)

        if hasattr(Sdf, "TokenListOp"):
            prim.SetMetadata("apiSchemas", Sdf.TokenListOp.CreateExplicit(current_schemas))
        else:
            prim.SetMetadata("apiSchemas", current_schemas)
    except Exception:
        pass


def _has_api_schema(prim, schema_name: str) -> bool:
    """Checks whether an API schema is listed in prim's applied schemas or metadata."""
    if not HAS_PXR or not prim:
        return False

    # 1. Native USD GetAppliedSchemas() (standard in USD 20.08+ / Omniverse Kit)
    try:
        if hasattr(prim, "GetAppliedSchemas"):
            applied = prim.GetAppliedSchemas()
            if applied and schema_name in applied:
                return True
    except Exception:
        pass

    # 2. Native USD HasAPI()
    try:
        if hasattr(prim, "HasAPI") and prim.HasAPI(schema_name):
            return True
    except Exception:
        pass

    # 3. Inspect metadata "apiSchemas" (handles Sdf.TokenListOp, VtTokenArray, list, tuple)
    try:
        api_schemas = prim.GetMetadata("apiSchemas")
        if api_schemas is not None:
            for attr_name in ("explicitItems", "prependedItems", "appendedItems", "addedItems", "orderedItems"):
                items = getattr(api_schemas, attr_name, None)
                if items and schema_name in items:
                    return True
            if isinstance(api_schemas, (list, tuple)):
                if schema_name in api_schemas:
                    return True
    except Exception:
        pass

    return False


def _get_attr_value(prim, attr_name: str, default):
    """Fail-silent getter for an attribute."""
    if not prim or not hasattr(prim, "GetAttribute"):
        return default
    try:
        attr = prim.GetAttribute(attr_name)
        if not attr or (hasattr(attr, "IsValid") and not attr.IsValid()):
            return default
        val = attr.Get()
        return default if val is None else val
    except Exception:
        return default


def _set_attr_value(prim, attr_name: str, value, type_name):
    """Safe setter for an attribute, only writing if the value has changed."""
    if not prim or not hasattr(prim, "IsValid") or not prim.IsValid():
        return
    try:
        attr = prim.GetAttribute(attr_name)
        if not attr or (hasattr(attr, "IsValid") and not attr.IsValid()):
            if hasattr(prim, "CreateAttribute"):
                attr = prim.CreateAttribute(attr_name, type_name)
        else:
            current_val = attr.Get()
            if current_val == value:
                return
        if attr and hasattr(attr, "Set"):
            attr.Set(value)
    except Exception:
        pass


# ==============================================================================
# HydragonActor
# ==============================================================================
class HydragonActor:
    SCHEMA_NAME = "HydragonActorAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, faction: str = "Player", health: float = 100.0, max_health: float = 100.0, damage_on_hit: float = 100.0):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        actor = cls(prim)
        actor.faction = faction
        actor.health = health
        actor.max_health = max_health
        actor.damage_on_hit = damage_on_hit
        actor.is_alive = True
        return actor

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("actor:faction")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def faction(self) -> str:
        return str(_get_attr_value(self._prim, "actor:faction", "Player"))

    @faction.setter
    def faction(self, val: str):
        _set_attr_value(self._prim, "actor:faction", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def health(self) -> float:
        return float(_get_attr_value(self._prim, "actor:health", 100.0))

    @health.setter
    def health(self, val: float):
        _set_attr_value(self._prim, "actor:health", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def max_health(self) -> float:
        return float(_get_attr_value(self._prim, "actor:maxHealth", 100.0))

    @max_health.setter
    def max_health(self, val: float):
        _set_attr_value(self._prim, "actor:maxHealth", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def damage_on_hit(self) -> float:
        return float(_get_attr_value(self._prim, "actor:damageOnHit", 100.0))

    @damage_on_hit.setter
    def damage_on_hit(self, val: float):
        _set_attr_value(self._prim, "actor:damageOnHit", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def is_alive(self) -> bool:
        return bool(_get_attr_value(self._prim, "actor:isAlive", True))

    @is_alive.setter
    def is_alive(self, val: bool):
        _set_attr_value(self._prim, "actor:isAlive", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    def apply_damage(self, amount: float) -> bool:
        """Applies damage and updates is_alive status. Returns True if actor was killed by this hit."""
        new_hp = max(0.0, self.health - amount)
        self.health = new_hp
        if new_hp <= 0.0 and self.is_alive:
            self.is_alive = False
            return True
        return False


# ==============================================================================
# HydragonPlayerController
# ==============================================================================
class HydragonPlayerController:
    SCHEMA_NAME = "HydragonPlayerControllerAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, move_force: float = 500.0, jump_impulse: float = 400.0, max_angular_vel: float = 25.0):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        ctl = cls(prim)
        ctl.move_force = move_force
        ctl.jump_impulse = jump_impulse
        ctl.max_angular_velocity = max_angular_vel
        ctl.can_jump_in_air = False
        ctl.is_active = True
        return ctl

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("player:moveForce")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def input_enabled(self) -> bool:
        return bool(_get_attr_value(self._prim, "player:inputEnabled", self.is_active))

    @input_enabled.setter
    def input_enabled(self, val: bool):
        _set_attr_value(self._prim, "player:inputEnabled", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def move_force(self) -> float:
        return float(_get_attr_value(self._prim, "player:moveForce", 500.0))

    @move_force.setter
    def move_force(self, val: float):
        _set_attr_value(self._prim, "player:moveForce", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def jump_impulse(self) -> float:
        return float(_get_attr_value(self._prim, "player:jumpImpulse", 400.0))

    @jump_impulse.setter
    def jump_impulse(self, val: float):
        _set_attr_value(self._prim, "player:jumpImpulse", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def max_angular_velocity(self) -> float:
        return float(_get_attr_value(self._prim, "player:maxAngularVelocity", 25.0))

    @max_angular_velocity.setter
    def max_angular_velocity(self, val: float):
        _set_attr_value(self._prim, "player:maxAngularVelocity", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def can_jump_in_air(self) -> bool:
        return bool(_get_attr_value(self._prim, "player:canJumpInAir", False))

    @can_jump_in_air.setter
    def can_jump_in_air(self, val: bool):
        _set_attr_value(self._prim, "player:canJumpInAir", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def is_active(self) -> bool:
        return bool(_get_attr_value(self._prim, "player:isActive", True))

    @is_active.setter
    def is_active(self, val: bool):
        _set_attr_value(self._prim, "player:isActive", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)


# ==============================================================================
# HydragonFollowCamera
# ==============================================================================
class HydragonFollowCamera:
    SCHEMA_NAME = "HydragonFollowCameraAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, target_path=None, arm_length: float = 8.0, pitch: float = 20.0, yaw: float = 0.0):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        cam = cls(prim)
        if target_path:
            cam.target_path = target_path
        cam.arm_length = arm_length
        cam.pitch = pitch
        cam.yaw = yaw
        cam.mouse_sensitivity = 0.2
        cam.min_pitch = -20.0
        cam.max_pitch = 80.0
        cam.collision_offset = 0.2
        cam.position_damping = 8.0
        cam.rotation_damping = 12.0
        return cam

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("camera:targetPrim")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def target_path(self):
        if not HAS_PXR or not self._prim:
            return None
        rel = self._prim.GetRelationship("camera:targetPrim")
        if not rel or not rel.IsValid():
            return None
        targets = rel.GetTargets()
        return targets[0] if targets else None

    @target_path.setter
    def target_path(self, path):
        if not HAS_PXR or not self._prim:
            return
        rel = self._prim.GetRelationship("camera:targetPrim")
        if not rel or not rel.IsValid():
            rel = self._prim.CreateRelationship("camera:targetPrim")
        target_sdf = Sdf.Path(str(path)) if not isinstance(path, Sdf.Path) else path
        rel.SetTargets([target_sdf])

    @property
    def arm_length(self) -> float:
        return float(_get_attr_value(self._prim, "camera:armLength", 8.0))

    @arm_length.setter
    def arm_length(self, val: float):
        _set_attr_value(self._prim, "camera:armLength", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def pitch(self) -> float:
        return float(_get_attr_value(self._prim, "camera:pitch", 20.0))

    @pitch.setter
    def pitch(self, val: float):
        _set_attr_value(self._prim, "camera:pitch", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def yaw(self) -> float:
        return float(_get_attr_value(self._prim, "camera:yaw", 0.0))

    @yaw.setter
    def yaw(self, val: float):
        _set_attr_value(self._prim, "camera:yaw", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def mouse_sensitivity(self) -> float:
        return float(_get_attr_value(self._prim, "camera:mouseSensitivity", 0.2))

    @mouse_sensitivity.setter
    def mouse_sensitivity(self, val: float):
        _set_attr_value(self._prim, "camera:mouseSensitivity", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def min_pitch(self) -> float:
        return float(_get_attr_value(self._prim, "camera:minPitch", -20.0))

    @min_pitch.setter
    def min_pitch(self, val: float):
        _set_attr_value(self._prim, "camera:minPitch", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def max_pitch(self) -> float:
        return float(_get_attr_value(self._prim, "camera:maxPitch", 80.0))

    @max_pitch.setter
    def max_pitch(self, val: float):
        _set_attr_value(self._prim, "camera:maxPitch", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def collision_offset(self) -> float:
        return float(_get_attr_value(self._prim, "camera:collisionOffset", 0.2))

    @collision_offset.setter
    def collision_offset(self, val: float):
        _set_attr_value(self._prim, "camera:collisionOffset", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def position_damping(self) -> float:
        return float(_get_attr_value(self._prim, "camera:positionDamping", 8.0))

    @position_damping.setter
    def position_damping(self, val: float):
        _set_attr_value(self._prim, "camera:positionDamping", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def rotation_damping(self) -> float:
        return float(_get_attr_value(self._prim, "camera:rotationDamping", 12.0))

    @rotation_damping.setter
    def rotation_damping(self, val: float):
        _set_attr_value(self._prim, "camera:rotationDamping", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)


# ==============================================================================
# HydragonChaserAI
# ==============================================================================
class HydragonChaserAI:
    SCHEMA_NAME = "HydragonChaserAIAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, detection_radius: float = 10.0, chase_force: float = 350.0, patrol_force: float = 150.0):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        ai = cls(prim)
        ai.detection_radius = detection_radius
        ai.lose_target_radius = detection_radius * 1.5
        ai.patrol_radius = 5.0
        ai.chase_force = chase_force
        ai.patrol_force = patrol_force
        ai.state = "Patrol"
        return ai

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("ai:detectionRadius")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def detection_radius(self) -> float:
        return float(_get_attr_value(self._prim, "ai:detectionRadius", 10.0))

    @detection_radius.setter
    def detection_radius(self, val: float):
        _set_attr_value(self._prim, "ai:detectionRadius", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def lose_target_radius(self) -> float:
        return float(_get_attr_value(self._prim, "ai:loseTargetRadius", 15.0))

    @lose_target_radius.setter
    def lose_target_radius(self, val: float):
        _set_attr_value(self._prim, "ai:loseTargetRadius", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def patrol_radius(self) -> float:
        return float(_get_attr_value(self._prim, "ai:patrolRadius", 5.0))

    @patrol_radius.setter
    def patrol_radius(self, val: float):
        _set_attr_value(self._prim, "ai:patrolRadius", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def chase_force(self) -> float:
        return float(_get_attr_value(self._prim, "ai:chaseForce", 350.0))

    @chase_force.setter
    def chase_force(self, val: float):
        _set_attr_value(self._prim, "ai:chaseForce", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def patrol_force(self) -> float:
        return float(_get_attr_value(self._prim, "ai:patrolForce", 150.0))

    @patrol_force.setter
    def patrol_force(self, val: float):
        _set_attr_value(self._prim, "ai:patrolForce", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def state(self) -> str:
        return str(_get_attr_value(self._prim, "ai:state", "Patrol"))

    @state.setter
    def state(self, val: str):
        _set_attr_value(self._prim, "ai:state", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def target_faction(self) -> str:
        return str(_get_attr_value(self._prim, "ai:targetFaction", "Player"))

    @target_faction.setter
    def target_faction(self, val: str):
        _set_attr_value(self._prim, "ai:targetFaction", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def target_prim(self):
        if not HAS_PXR or not self._prim:
            return None
        rel = self._prim.GetRelationship("ai:targetPrim")
        if not rel or not rel.IsValid():
            return None
        targets = rel.GetTargets()
        return targets[0] if targets else None

    @target_prim.setter
    def target_prim(self, path):
        if not HAS_PXR or not self._prim:
            return
        rel = self._prim.GetRelationship("ai:targetPrim")
        if not rel or not rel.IsValid():
            rel = self._prim.CreateRelationship("ai:targetPrim")
        target_sdf = Sdf.Path(str(path)) if not isinstance(path, Sdf.Path) else path
        rel.SetTargets([target_sdf])


# ==============================================================================
# HydragonTrigger
# ==============================================================================
DEFAULT_TRIGGER_SOUND: str = "data/assets/audio/sound_fx_samples/achievement_02.wav"


class HydragonTrigger:
    SCHEMA_NAME = "HydragonTriggerAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(
        cls,
        prim,
        event_type: str = "OnLevelComplete",
        filter_faction: str = "Player",
        is_one_shot: bool = True,
        sound_asset_path: str = DEFAULT_TRIGGER_SOUND,
        sound_enabled: bool = True,
        sound_play_once: bool = True,
    ):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        trig = cls(prim)
        trig.event_type = event_type
        trig.filter_faction = filter_faction
        trig.is_one_shot = is_one_shot
        trig.is_enabled = True
        trig.sound_asset_path = sound_asset_path
        trig.sound_enabled = sound_enabled
        trig.sound_play_once = sound_play_once
        return trig

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and (
                prim.HasAttribute("trigger:eventType") or prim.HasAttribute("trigger:soundAssetPath")
            )
        )

    @property
    def prim(self):
        return self._prim

    @property
    def event_type(self) -> str:
        return str(_get_attr_value(self._prim, "trigger:eventType", "OnLevelComplete"))

    @event_type.setter
    def event_type(self, val: str):
        _set_attr_value(self._prim, "trigger:eventType", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def filter_faction(self) -> str:
        return str(_get_attr_value(self._prim, "trigger:filterFaction", "Player"))

    @filter_faction.setter
    def filter_faction(self, val: str):
        _set_attr_value(self._prim, "trigger:filterFaction", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def is_one_shot(self) -> bool:
        return bool(_get_attr_value(self._prim, "trigger:isOneShot", True))

    @is_one_shot.setter
    def is_one_shot(self, val: bool):
        _set_attr_value(self._prim, "trigger:isOneShot", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def is_enabled(self) -> bool:
        return bool(_get_attr_value(self._prim, "trigger:isEnabled", True))

    @is_enabled.setter
    def is_enabled(self, val: bool):
        _set_attr_value(self._prim, "trigger:isEnabled", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def sound_asset_path(self) -> str:
        val = _get_attr_value(self._prim, "trigger:soundAssetPath", DEFAULT_TRIGGER_SOUND)
        if hasattr(val, "path"):
            return str(val.path)
        return str(val) if val is not None else DEFAULT_TRIGGER_SOUND

    @sound_asset_path.setter
    def sound_asset_path(self, val: str):
        asset_val = Sdf.AssetPath(str(val)) if HAS_PXR else str(val)
        _set_attr_value(self._prim, "trigger:soundAssetPath", asset_val, Sdf.ValueTypeNames.Asset if HAS_PXR else None)

    @property
    def sound_enabled(self) -> bool:
        return bool(_get_attr_value(self._prim, "trigger:soundEnabled", True))

    @sound_enabled.setter
    def sound_enabled(self, val: bool):
        _set_attr_value(self._prim, "trigger:soundEnabled", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def sound_play_once(self) -> bool:
        return bool(_get_attr_value(self._prim, "trigger:soundPlayOnce", True))

    @sound_play_once.setter
    def sound_play_once(self, val: bool):
        _set_attr_value(self._prim, "trigger:soundPlayOnce", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)


# ==============================================================================
# HydragonGameManager
# ==============================================================================
class HydragonGameManager:
    SCHEMA_NAME = "HydragonGameAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, state: str = "Playing"):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        gm = cls(prim)
        gm.state = state
        gm.score = 0
        gm.foes_destroyed = 0
        gm.total_foes = 0
        gm.elapsed_time = 0.0
        return gm

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("game:state")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def state(self) -> str:
        return str(_get_attr_value(self._prim, "game:state", "Playing"))

    @state.setter
    def state(self, val: str):
        _set_attr_value(self._prim, "game:state", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def score(self) -> int:
        return int(_get_attr_value(self._prim, "game:score", 0))

    @score.setter
    def score(self, val: int):
        _set_attr_value(self._prim, "game:score", int(val), Sdf.ValueTypeNames.Int if HAS_PXR else None)

    @property
    def foes_destroyed(self) -> int:
        return int(_get_attr_value(self._prim, "game:foesDestroyed", 0))

    @foes_destroyed.setter
    def foes_destroyed(self, val: int):
        _set_attr_value(self._prim, "game:foesDestroyed", int(val), Sdf.ValueTypeNames.Int if HAS_PXR else None)

    @property
    def total_foes(self) -> int:
        return int(_get_attr_value(self._prim, "game:totalFoes", 0))

    @total_foes.setter
    def total_foes(self, val: int):
        _set_attr_value(self._prim, "game:totalFoes", int(val), Sdf.ValueTypeNames.Int if HAS_PXR else None)

    @property
    def elapsed_time(self) -> float:
        return float(_get_attr_value(self._prim, "game:elapsedTime", 0.0))

    @elapsed_time.setter
    def elapsed_time(self, val: float):
        _set_attr_value(self._prim, "game:elapsedTime", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    def record_foe_destroyed(self, points: int = 100):
        self.foes_destroyed += 1
        self.score += points

    def trigger_victory(self):
        self.state = "Victory"


# ==============================================================================
# HydragonUICanvas
# ==============================================================================
class HydragonUICanvas:
    SCHEMA_NAME = "HydragonUICanvasAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(
        cls,
        prim,
        canvas_type: str = "InGame",
        show_controls: bool = True,
        show_countdown: bool = True,
        show_score_popups: bool = True,
        auto_activate_on_play: bool = True,
        title: str = "",
        subtitle: str = "",
        show_start_game: bool = True,
        show_resume: bool = True,
        show_restart: bool = True,
        show_settings: bool = True,
        show_quit: bool = True,
    ):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        canvas = cls(prim)
        canvas.canvas_type = canvas_type
        canvas.show_controls = show_controls
        canvas.show_countdown = show_countdown
        canvas.show_score_popups = show_score_popups
        canvas.auto_activate_on_play = auto_activate_on_play
        if title:
            canvas.title = title
        if subtitle:
            canvas.subtitle = subtitle
        canvas.show_start_game = show_start_game
        canvas.show_resume = show_resume
        canvas.show_restart = show_restart
        canvas.show_settings = show_settings
        canvas.show_quit = show_quit
        return canvas

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and (
                prim.HasAttribute("hud:canvasType") or prim.HasAttribute("hud:title")
            )
        )

    @property
    def prim(self):
        return self._prim

    @property
    def canvas_type(self) -> str:
        return str(_get_attr_value(self._prim, "hud:canvasType", "InGame"))

    @canvas_type.setter
    def canvas_type(self, val: str):
        _set_attr_value(self._prim, "hud:canvasType", str(val), Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def title(self) -> str:
        return str(_get_attr_value(self._prim, "hud:title", ""))

    @title.setter
    def title(self, val: str):
        _set_attr_value(self._prim, "hud:title", str(val), Sdf.ValueTypeNames.String if HAS_PXR else None)

    @property
    def subtitle(self) -> str:
        return str(_get_attr_value(self._prim, "hud:subtitle", ""))

    @subtitle.setter
    def subtitle(self, val: str):
        _set_attr_value(self._prim, "hud:subtitle", str(val), Sdf.ValueTypeNames.String if HAS_PXR else None)

    @property
    def show_controls(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showControls", True))

    @show_controls.setter
    def show_controls(self, val: bool):
        _set_attr_value(self._prim, "hud:showControls", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_countdown(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showCountdown", True))

    @show_countdown.setter
    def show_countdown(self, val: bool):
        _set_attr_value(self._prim, "hud:showCountdown", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_score_popups(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showScorePopups", True))

    @show_score_popups.setter
    def show_score_popups(self, val: bool):
        _set_attr_value(self._prim, "hud:showScorePopups", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_start_game(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showStartGame", True))

    @show_start_game.setter
    def show_start_game(self, val: bool):
        _set_attr_value(self._prim, "hud:showStartGame", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_resume(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showResume", True))

    @show_resume.setter
    def show_resume(self, val: bool):
        _set_attr_value(self._prim, "hud:showResume", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_restart(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showRestart", True))

    @show_restart.setter
    def show_restart(self, val: bool):
        _set_attr_value(self._prim, "hud:showRestart", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_settings(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showSettings", True))

    @show_settings.setter
    def show_settings(self, val: bool):
        _set_attr_value(self._prim, "hud:showSettings", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def show_quit(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:showQuit", True))

    @show_quit.setter
    def show_quit(self, val: bool):
        _set_attr_value(self._prim, "hud:showQuit", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def auto_activate_on_play(self) -> bool:
        return bool(_get_attr_value(self._prim, "hud:autoActivateOnPlay", True))

    @auto_activate_on_play.setter
    def auto_activate_on_play(self, val: bool):
        _set_attr_value(self._prim, "hud:autoActivateOnPlay", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)


# ==============================================================================
# HydragonSoundtrack
# ==============================================================================
DEFAULT_AMBIENT_SOUNDTRACK: str = "data/assets/audio/sound_fx_samples/lounge_soundtrack_01.wav"


class HydragonSoundtrack:
    SCHEMA_NAME = "HydragonSoundtrackAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(
        cls,
        prim,
        current_track: str = "ambient",
        track_state: str = "Playing",
        volume: float = 0.8,
        auto_play: bool = True,
        is_looping: bool = True,
        fade_duration: float = 1.5,
        ambient_asset_path: str = DEFAULT_AMBIENT_SOUNDTRACK,
        combat_asset_path: str = "",
        victory_asset_path: str = "",
    ):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        st = cls(prim)
        st.current_track = current_track
        st.track_state = track_state
        st.volume = volume
        st.auto_play = auto_play
        st.is_looping = is_looping
        st.fade_duration = fade_duration
        st.ambient_asset_path = ambient_asset_path
        st.combat_asset_path = combat_asset_path
        st.victory_asset_path = victory_asset_path
        return st

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and (
                prim.HasAttribute("soundtrack:currentTrack") or prim.HasAttribute("soundtrack:ambientAssetPath")
            )
        )

    @property
    def prim(self):
        return self._prim

    @property
    def current_track(self) -> str:
        return str(_get_attr_value(self._prim, "soundtrack:currentTrack", "ambient"))

    @current_track.setter
    def current_track(self, val: str):
        _set_attr_value(self._prim, "soundtrack:currentTrack", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def track_state(self) -> str:
        return str(_get_attr_value(self._prim, "soundtrack:trackState", "Playing"))

    @track_state.setter
    def track_state(self, val: str):
        _set_attr_value(self._prim, "soundtrack:trackState", val, Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def volume(self) -> float:
        return float(_get_attr_value(self._prim, "soundtrack:volume", 0.8))

    @volume.setter
    def volume(self, val: float):
        _set_audio_val = max(0.0, min(1.0, float(val)))
        _set_attr_value(self._prim, "soundtrack:volume", _set_audio_val, Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def auto_play(self) -> bool:
        return bool(_get_attr_value(self._prim, "soundtrack:autoPlay", True))

    @auto_play.setter
    def auto_play(self, val: bool):
        _set_attr_value(self._prim, "soundtrack:autoPlay", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def is_looping(self) -> bool:
        return bool(_get_attr_value(self._prim, "soundtrack:isLooping", True))

    @is_looping.setter
    def is_looping(self, val: bool):
        _set_attr_value(self._prim, "soundtrack:isLooping", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)

    @property
    def fade_duration(self) -> float:
        return float(_get_attr_value(self._prim, "soundtrack:fadeDuration", 1.5))

    @fade_duration.setter
    def fade_duration(self, val: float):
        _set_attr_value(self._prim, "soundtrack:fadeDuration", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def ambient_asset_path(self) -> str:
        val = _get_attr_value(self._prim, "soundtrack:ambientAssetPath", DEFAULT_AMBIENT_SOUNDTRACK)
        if hasattr(val, "path"):
            return str(val.path)
        return str(val) if val is not None else DEFAULT_AMBIENT_SOUNDTRACK

    @ambient_asset_path.setter
    def ambient_asset_path(self, val: str):
        asset_val = Sdf.AssetPath(str(val)) if HAS_PXR else str(val)
        _set_attr_value(self._prim, "soundtrack:ambientAssetPath", asset_val, Sdf.ValueTypeNames.Asset if HAS_PXR else None)

    @property
    def combat_asset_path(self) -> str:
        val = _get_attr_value(self._prim, "soundtrack:combatAssetPath", "")
        if hasattr(val, "path"):
            return str(val.path)
        return str(val) if val is not None else ""

    @combat_asset_path.setter
    def combat_asset_path(self, val: str):
        asset_val = Sdf.AssetPath(str(val)) if HAS_PXR else str(val)
        _set_attr_value(self._prim, "soundtrack:combatAssetPath", asset_val, Sdf.ValueTypeNames.Asset if HAS_PXR else None)

    @property
    def victory_asset_path(self) -> str:
        val = _get_attr_value(self._prim, "soundtrack:victoryAssetPath", "")
        if hasattr(val, "path"):
            return str(val.path)
        return str(val) if val is not None else ""

    @victory_asset_path.setter
    def victory_asset_path(self, val: str):
        asset_val = Sdf.AssetPath(str(val)) if HAS_PXR else str(val)
        _set_attr_value(self._prim, "soundtrack:victoryAssetPath", asset_val, Sdf.ValueTypeNames.Asset if HAS_PXR else None)


# ==============================================================================
# HydragonEffectsManager
# ==============================================================================
class HydragonEffectsManager:
    SCHEMA_NAME = "HydragonEffectsAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(
        cls,
        prim,
        pool_size: int = 3,
        num_sparks: int = 150,
        render_mode: str = "point_instancer",
        spark_radius: float = 20.0,
        flash_intensity: float = 5000000.0,
        burst_lifetime: float = 0.55,
        auto_initialize_on_play: bool = True,
    ):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        mgr = cls(prim)
        mgr.pool_size = pool_size
        mgr.num_sparks = num_sparks
        mgr.render_mode = render_mode
        mgr.spark_radius = spark_radius
        mgr.flash_intensity = flash_intensity
        mgr.burst_lifetime = burst_lifetime
        mgr.auto_initialize_on_play = auto_initialize_on_play
        return mgr

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("effects:poolSize")
        )

    @property
    def prim(self):
        return self._prim

    @property
    def pool_size(self) -> int:
        return int(_get_attr_value(self._prim, "effects:poolSize", 3))

    @pool_size.setter
    def pool_size(self, val: int):
        _set_attr_value(self._prim, "effects:poolSize", int(val), Sdf.ValueTypeNames.Int if HAS_PXR else None)

    @property
    def num_sparks(self) -> int:
        return int(_get_attr_value(self._prim, "effects:numSparks", 150))

    @num_sparks.setter
    def num_sparks(self, val: int):
        _set_attr_value(self._prim, "effects:numSparks", int(val), Sdf.ValueTypeNames.Int if HAS_PXR else None)

    @property
    def render_mode(self) -> str:
        return str(_get_attr_value(self._prim, "effects:renderMode", "point_instancer"))

    @render_mode.setter
    def render_mode(self, val: str):
        _set_attr_value(self._prim, "effects:renderMode", str(val), Sdf.ValueTypeNames.Token if HAS_PXR else None)

    @property
    def spark_radius(self) -> float:
        return float(_get_attr_value(self._prim, "effects:sparkRadius", 20.0))

    @spark_radius.setter
    def spark_radius(self, val: float):
        _set_attr_value(self._prim, "effects:sparkRadius", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def flash_intensity(self) -> float:
        return float(_get_attr_value(self._prim, "effects:flashIntensity", 5000000.0))

    @flash_intensity.setter
    def flash_intensity(self, val: float):
        _set_attr_value(self._prim, "effects:flashIntensity", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def burst_lifetime(self) -> float:
        return float(_get_attr_value(self._prim, "effects:burstLifetime", 0.55))

    @burst_lifetime.setter
    def burst_lifetime(self, val: float):
        _set_attr_value(self._prim, "effects:burstLifetime", float(val), Sdf.ValueTypeNames.Float if HAS_PXR else None)

    @property
    def auto_initialize_on_play(self) -> bool:
        return bool(_get_attr_value(self._prim, "effects:autoInitializeOnPlay", True))

    @auto_initialize_on_play.setter
    def auto_initialize_on_play(self, val: bool):
        _set_attr_value(self._prim, "effects:autoInitializeOnPlay", bool(val), Sdf.ValueTypeNames.Bool if HAS_PXR else None)
