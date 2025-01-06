
## Classes

### RuntimeParticleSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeParticleConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSystem



**Parameters:** const ParticleSystemDesc& desc

### DestroySystem



**Parameters:** ParticleSystemHandle handle

### EmitParticles



**Parameters:** ParticleSystemHandle handle, uint32_t count = 1

### StopEmission



**Parameters:** ParticleSystemHandle handle

### KillParticles



**Parameters:** ParticleSystemHandle handle

### SetEmissionRate



**Parameters:** ParticleSystemHandle handle, float particlesPerSecond

### SetLifetime



**Parameters:** ParticleSystemHandle handle, float minLife, float maxLife

### SetVelocity



**Parameters:** ParticleSystemHandle handle, const Vector3& velocity

### AddForceField



**Parameters:** const std::string& name, const ForceFieldDesc& desc

### RemoveForceField



**Parameters:** const std::string& name

### UpdateForceField



**Parameters:** const std::string& name, const ForceFieldParams& params

### SetSystemTransform



**Parameters:** ParticleSystemHandle handle, const Transform& transform

### SetSimulationSpace



**Parameters:** ParticleSystemHandle handle, SimulationSpace space

### RegisterCollisionHandler



**Parameters:** ParticleSystemHandle handle, const CollisionHandler& handler

### UnregisterCollisionHandler



**Parameters:** ParticleSystemHandle handle
