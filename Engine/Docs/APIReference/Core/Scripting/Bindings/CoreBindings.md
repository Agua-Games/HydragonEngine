
## Classes

### CoreBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const CoreBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RegisterType



**Parameters:** const std::string& name, const TypeInfo& typeInfo

### UnregisterType



**Parameters:** const std::string& name

### RegisterFunction



**Parameters:** const std::string& name, const FunctionInfo& funcInfo

### UnregisterFunction



**Parameters:** const std::string& name

### RegisterEnum



**Parameters:** const std::string& name, const EnumInfo& enumInfo

### UnregisterEnum



**Parameters:** const std::string& name

### RegisterConverter



**Parameters:** const TypePair& types, std::unique_ptr<ITypeConverter> converter

### UnregisterConverter



**Parameters:** const TypePair& types
