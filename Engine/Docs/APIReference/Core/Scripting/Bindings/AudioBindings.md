
## Classes

### AudioBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AudioBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSound



**Parameters:** const std::string& name, const SoundDesc& desc

### DestroySound



**Parameters:** SoundHandle handle

### PreloadSound



**Parameters:** const std::string& name

### PlaySound



**Parameters:** const std::string& name, const PlayParams& params = {}

### StopSound



**Parameters:** SourceHandle handle

### PauseSound



**Parameters:** SourceHandle handle

### ResumeSound



**Parameters:** SourceHandle handle

### SetVolume



**Parameters:** const std::string& name, float volume

### SetPitch



**Parameters:** const std::string& name, float pitch

### SetPosition



**Parameters:** SourceHandle handle, const Vector3& position

### CreateEffect



**Parameters:** const std::string& name, const EffectDesc& desc

### DestroyEffect



**Parameters:** EffectHandle handle

### ApplyEffect



**Parameters:** const std::string& soundName, const std::string& effectName

### SetListenerPosition



**Parameters:** const Vector3& position

### SetListenerOrientation



**Parameters:** const Vector3& forward, const Vector3& up

### EnableVisualization



**Parameters:** bool enable

### GetSpectrum



**Parameters:** std::vector<float>& spectrum

### GetWaveform



**Parameters:** std::vector<float>& waveform
