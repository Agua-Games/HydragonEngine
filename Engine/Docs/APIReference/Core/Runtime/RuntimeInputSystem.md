
## Classes

### RuntimeInputSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeInputConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RegisterInputMap



**Parameters:** const std::string& name, const InputMapDesc& desc

### UnregisterInputMap



**Parameters:** const std::string& name

### SetActiveInputMap



**Parameters:** const std::string& name

### EnableInputContext



**Parameters:** const std::string& context

### DisableInputContext



**Parameters:** const std::string& context

### ProcessInputEvent



**Parameters:** const InputEvent& event

### StartInputRecording



**Parameters:** const std::string& filename

### StopInputRecording



**Parameters:** 

### PlaybackRecording



**Parameters:** const std::string& filename
