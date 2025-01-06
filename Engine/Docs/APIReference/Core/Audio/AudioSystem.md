
## Classes

### AudioSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AudioConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSound



**Parameters:** const SoundDesc& desc

### DestroySound



**Parameters:** SoundHandle handle

### PlaySound



**Parameters:** SoundHandle sound, const PlayParams& params = {}

### StopSound



**Parameters:** SourceHandle source

### PauseSound



**Parameters:** SourceHandle source

### ResumeSound



**Parameters:** SourceHandle source

### SetListenerTransform



**Parameters:** const Transform& transform

### SetSourceTransform



**Parameters:** SourceHandle source, const Transform& transform

### SetVolume



**Parameters:** SourceHandle source, float volume

### SetPitch



**Parameters:** SourceHandle source, float pitch

### CreateEffect



**Parameters:** const EffectDesc& desc

### DestroyEffect



**Parameters:** EffectHandle handle

### ApplyEffect



**Parameters:** SourceHandle source, EffectHandle effect
