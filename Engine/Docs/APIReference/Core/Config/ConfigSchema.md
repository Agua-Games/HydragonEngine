
## Classes

### ConfigSchema




## Functions

### Create



**Parameters:** const std::string& name, const SchemaDesc& desc = {}

### AddField



**Parameters:** const std::string& name, const FieldDesc& desc

### RemoveField



**Parameters:** const std::string& name

### SetFieldRequired



**Parameters:** const std::string& name, bool required

### SetFieldDefault



**Parameters:** const std::string& name, const Variant& value

### AddValidationRule



**Parameters:** const std::string& field, const ValidationRule& rule

### RemoveValidationRule



**Parameters:** const std::string& field, RuleHandle rule

### InheritFrom



**Parameters:** SchemaHandle parent

### RemoveInheritance



**Parameters:** 

### SetVersion



**Parameters:** const std::string& version
