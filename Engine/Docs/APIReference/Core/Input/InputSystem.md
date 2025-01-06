
## Classes

### InputSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const InputConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RegisterDevice



**Parameters:** const DeviceDesc& desc

### UnregisterDevice



**Parameters:** DeviceHandle handle

### CreateAction



**Parameters:** const ActionDesc& desc

### DestroyAction



**Parameters:** ActionHandle handle

### BindAction



**Parameters:** ActionHandle action, const ActionBinding& binding

### UnbindAction



**Parameters:** ActionHandle action, const ActionBinding& binding

### AddActionCallback



**Parameters:** ActionHandle action, const ActionCallback& callback

### RemoveActionCallback



**Parameters:** ActionHandle action, const ActionCallback& callback

### SetHapticFeedback



**Parameters:** DeviceHandle device, const HapticEffect& effect
