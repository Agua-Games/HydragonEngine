
## Classes

### InputBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const InputBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateAction



**Parameters:** const std::string& name, const ActionDesc& desc

### DestroyAction



**Parameters:** ActionHandle handle

### BindAction



**Parameters:** ActionHandle handle, const ActionBinding& binding

### UnbindAction



**Parameters:** ActionHandle handle

### RegisterActionCallback



**Parameters:** const std::string& name, const ActionCallback& callback

### UnregisterActionCallback



**Parameters:** const std::string& name

### RegisterGesturePattern



**Parameters:** const std::string& name, const GesturePattern& pattern

### UnregisterGesturePattern



**Parameters:** const std::string& name

### StartInputRecording



**Parameters:** 

### StopInputRecording



**Parameters:** 

### PlaybackRecording



**Parameters:** const std::string& path

### SaveRecording



**Parameters:** const std::string& path

### TriggerHapticFeedback



**Parameters:** const std::string& device, const HapticEffect& effect
