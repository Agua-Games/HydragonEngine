
## Classes

### RuntimeAudioSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeAudioConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSource



**Parameters:** const AudioSourceDesc& desc

### DestroySource



**Parameters:** AudioSourceHandle handle

### PlaySound



**Parameters:** AudioSourceHandle handle, const PlaybackParams& params = {}

### StopSound



**Parameters:** AudioSourceHandle handle

### PauseSound



**Parameters:** AudioSourceHandle handle

### ResumeSound



**Parameters:** AudioSourceHandle handle

### SetSourcePosition



**Parameters:** AudioSourceHandle handle, const Vector3& position

### SetSourceVelocity



**Parameters:** AudioSourceHandle handle, const Vector3& velocity

### SetSourceVolume



**Parameters:** AudioSourceHandle handle, float volume

### SetSourcePitch



**Parameters:** AudioSourceHandle handle, float pitch

### CreateBus



**Parameters:** const std::string& name, const AudioBusDesc& desc

### DestroyBus



**Parameters:** const std::string& name

### SetBusVolume



**Parameters:** const std::string& name, float volume

### AddReverbZone



**Parameters:** const std::string& name, const ReverbZoneDesc& desc

### RemoveReverbZone



**Parameters:** const std::string& name

### UpdateReverbZone



**Parameters:** const std::string& name, const ReverbParams& params

### SetListenerTransform



**Parameters:** const Transform& transform

### SetListenerVelocity



**Parameters:** const Vector3& velocity

### RegisterEffectProcessor



**Parameters:** const std::string& name, std::unique_ptr<IAudioProcessor> processor

### UnregisterEffectProcessor



**Parameters:** const std::string& name
