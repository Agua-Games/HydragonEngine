
## Classes

### PluginSandboxSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const PluginSandboxConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSandbox



**Parameters:** const std::string& pluginId

### DestroySandbox



**Parameters:** SandboxHandle handle

### SetResourceLimits



**Parameters:** SandboxHandle handle, const ResourceLimits& limits

### SetPermissions



**Parameters:** SandboxHandle handle, const SandboxPermissions& permissions

### MonitorResource



**Parameters:** SandboxHandle handle, ResourceType type
