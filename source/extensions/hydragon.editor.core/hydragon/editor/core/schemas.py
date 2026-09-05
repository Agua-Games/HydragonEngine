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
    if not HAS_PXR or not prim:
        return default
    attr = prim.GetAttribute(attr_name)
    if not attr or not attr.IsValid():
        return default
    val = attr.Get()
    return default if val is None else val


def _set_attr_value(prim, attr_name: str, value, type_name):
    """Safe setter for an attribute, creating it if it doesn't exist."""
    if not HAS_PXR or not prim:
        return
    attr = prim.GetAttribute(attr_name)
    if not attr or not attr.IsValid():
        attr = prim.CreateAttribute(attr_name, type_name)
    attr.Set(value)


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


# ==============================================================================
# HydragonTrigger
# ==============================================================================
class HydragonTrigger:
    SCHEMA_NAME = "HydragonTriggerAPI"

    def __init__(self, prim):
        self._prim = prim

    @classmethod
    def apply(cls, prim, event_type: str = "OnLevelComplete", filter_faction: str = "Player", is_one_shot: bool = True):
        _ensure_api_schema(prim, cls.SCHEMA_NAME)
        trig = cls(prim)
        trig.event_type = event_type
        trig.filter_faction = filter_faction
        trig.is_one_shot = is_one_shot
        trig.is_enabled = True
        return trig

    @classmethod
    def is_applied(cls, prim) -> bool:
        return _has_api_schema(prim, cls.SCHEMA_NAME) or bool(
            prim and hasattr(prim, "HasAttribute") and prim.HasAttribute("trigger:eventType")
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
