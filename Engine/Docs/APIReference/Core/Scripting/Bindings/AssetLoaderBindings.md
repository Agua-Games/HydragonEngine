
## Classes

### AssetLoaderBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AssetLoaderBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### QueueLoad



**Parameters:** const std::string& path, LoadPriority priority = LoadPriority::Normal

### CancelLoad



**Parameters:** LoadHandle handle

### CreateLoadGroup



**Parameters:** const std::string& name

### DestroyLoadGroup



**Parameters:** GroupHandle handle

### AddToGroup



**Parameters:** GroupHandle group, LoadHandle load

### RemoveFromGroup



**Parameters:** GroupHandle group, LoadHandle load

### PreloadAsset



**Parameters:** const std::string& path

### PreloadGroup



**Parameters:** const std::vector<std::string>& paths

### CancelPreload



**Parameters:** const std::string& path

### SetLoadCallback



**Parameters:** LoadHandle handle, const LoadCallback& callback

### SetGroupCallback



**Parameters:** GroupHandle handle, const GroupCallback& callback

### SetGlobalLoadCallback



**Parameters:** const GlobalLoadCallback& callback

### SetLoadPriority



**Parameters:** LoadHandle handle, LoadPriority priority

### PauseLoading



**Parameters:** bool pause

### CancelAllLoads



**Parameters:** 

### FlushLoadCache



**Parameters:** 

### EnableLoadProfiling



**Parameters:** bool enable

### DumpLoadStats



**Parameters:** const std::string& path

### ValidateLoadQueue



**Parameters:** 
