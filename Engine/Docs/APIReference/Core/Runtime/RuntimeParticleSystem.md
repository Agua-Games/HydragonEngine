# Classes
---

## RuntimeParticleSystem
---




# Variables
---

### `enableGPUSimulation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableInstancing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLOD`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCollision`

- **Type:** `bool`

- **Default Value:** `true`



### `maxActiveSystems`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxParticlesPerSystem`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `updateBatchSize`

- **Type:** `uint32_t`

- **Default Value:** `1024`



### `cullingDistance`

- **Type:** `float`

- **Default Value:** `100.0f`



### `effectCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Particles"`



### `config`

- **Type:** `const RuntimeParticleConfig&`

- **Default Value:** `{})`



### `count`

- **Type:** `uint32_t`

- **Default Value:** `1)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeParticleConfig`



### `m_Stats`

- **Type:** `RuntimeParticleStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeParticleConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeParticleSystem&`

---

## Initialize



- **Parameters:** const RuntimeParticleConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateSystem



- **Parameters:** const ParticleSystemDesc& desc

- **Return:** `ParticleSystemHandle`

---

## DestroySystem



- **Parameters:** ParticleSystemHandle handle

- **Return:** `void`

---

## EmitParticles



- **Parameters:** ParticleSystemHandle handle, uint32_t count = 1

- **Return:** `void`

---

## StopEmission



- **Parameters:** ParticleSystemHandle handle

- **Return:** `void`

---

## KillParticles



- **Parameters:** ParticleSystemHandle handle

- **Return:** `void`

---

## SetEmissionRate



- **Parameters:** ParticleSystemHandle handle, float particlesPerSecond

- **Return:** `void`

---

## SetLifetime



- **Parameters:** ParticleSystemHandle handle, float minLife, float maxLife

- **Return:** `void`

---

## SetVelocity



- **Parameters:** ParticleSystemHandle handle, const Vector3& velocity

- **Return:** `void`

---

## AddForceField



- **Parameters:** const std::string& name, const ForceFieldDesc& desc

- **Return:** `void`

---

## RemoveForceField



- **Parameters:** const std::string& name

- **Return:** `void`

---

## UpdateForceField



- **Parameters:** const std::string& name, const ForceFieldParams& params

- **Return:** `void`

---

## SetSystemTransform



- **Parameters:** ParticleSystemHandle handle, const Transform& transform

- **Return:** `void`

---

## SetSimulationSpace



- **Parameters:** ParticleSystemHandle handle, SimulationSpace space

- **Return:** `void`

---

## RegisterCollisionHandler



- **Parameters:** ParticleSystemHandle handle, const CollisionHandler& handler

- **Return:** `void`

---

## UnregisterCollisionHandler



- **Parameters:** ParticleSystemHandle handle

- **Return:** `void`

---
