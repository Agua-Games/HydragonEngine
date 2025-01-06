
## Classes

### RuntimeStateSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeStateConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RegisterState



**Parameters:** const std::string& name, std::unique_ptr<IState> state

### UnregisterState



**Parameters:** const std::string& name

### TransitionTo



**Parameters:** const std::string& state, const TransitionDesc& desc = {}

### PushState



**Parameters:** const std::string& state

### PopState



**Parameters:** 
