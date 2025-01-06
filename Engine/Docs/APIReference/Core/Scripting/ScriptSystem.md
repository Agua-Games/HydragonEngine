
## Classes

### ScriptSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const ScriptConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** float deltaTime

### CreateScript



**Parameters:** const ScriptDesc& desc

### DestroyScript



**Parameters:** ScriptHandle handle

### ExecuteScript



**Parameters:** ScriptHandle handle

### StopScript



**Parameters:** ScriptHandle handle

### SetGlobal



**Parameters:** const std::string& name, const ScriptValue& value

### StartCoroutine



**Parameters:** ScriptHandle script, const std::string& function

### StopCoroutine



**Parameters:** CoroutineHandle handle
