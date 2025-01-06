
## Classes

### ConfigSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** 

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateConfig



**Parameters:** const std::string& name, const ConfigDesc& desc = {}

### DestroyConfig



**Parameters:** ConfigHandle handle

### LoadConfig



**Parameters:** const std::string& path

### SaveConfig



**Parameters:** ConfigHandle handle, const std::string& path = ""

### SetValue



**Parameters:** ConfigHandle handle, const std::string& key, const Variant& value

### RemoveValue



**Parameters:** ConfigHandle handle, const std::string& key

### CreateSection



**Parameters:** ConfigHandle handle, const std::string& section

### RemoveSection



**Parameters:** ConfigHandle handle, const std::string& section

### SetSchema



**Parameters:** ConfigHandle handle, const ConfigSchema& schema

### CreateSnapshot



**Parameters:** ConfigHandle handle, const std::string& description = ""

### RestoreSnapshot



**Parameters:** ConfigHandle handle, uint32_t version

### SetEncryptionKey



**Parameters:** ConfigHandle handle, const std::string& key

### EncryptConfig



**Parameters:** ConfigHandle handle

### DecryptConfig



**Parameters:** ConfigHandle handle

### ValidateConfig



**Parameters:** ConfigHandle handle

### OptimizeConfig



**Parameters:** ConfigHandle handle

### CompactConfig



**Parameters:** ConfigHandle handle

### EnableConfigTracking



**Parameters:** bool enable

### DumpConfigState



**Parameters:** ConfigHandle handle, const std::string& path

### AnalyzeConfigUsage



**Parameters:** ConfigHandle handle
