
## Classes

### ConfigSchemaBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const ConfigSchemaBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSchema



**Parameters:** const std::string& name, const SchemaDesc& desc = {}

### LoadSchema



**Parameters:** const std::string& path

### SaveSchema



**Parameters:** SchemaHandle handle, const std::string& path

### CloseSchema



**Parameters:** SchemaHandle handle

### AddField



**Parameters:** SchemaHandle handle, const std::string& name, const FieldDesc& desc

### RemoveField



**Parameters:** SchemaHandle handle, const std::string& name

### SetFieldRequired



**Parameters:** SchemaHandle handle, const std::string& name, bool required

### SetFieldDefault



**Parameters:** SchemaHandle handle, const std::string& name, const Variant& value

### AddValidationRule



**Parameters:** SchemaHandle handle, const std::string& field, const ValidationRule& rule

### RemoveValidationRule



**Parameters:** SchemaHandle handle, const std::string& field, RuleHandle rule

### AddCustomValidator



**Parameters:** const std::string& name, std::unique_ptr<ICustomValidator> validator

### InheritSchema



**Parameters:** SchemaHandle child, SchemaHandle parent

### RemoveInheritance



**Parameters:** SchemaHandle child

### ValidateConfig



**Parameters:** ConfigHandle config, SchemaHandle schema

### ValidateValue



**Parameters:** const Variant& value, const FieldDesc& desc

### ValidateSchema



**Parameters:** SchemaHandle handle

### OptimizeSchema



**Parameters:** SchemaHandle handle

### MergeSchemas



**Parameters:** SchemaHandle target, const std::vector<SchemaHandle>& sources

### EnableSchemaTracking



**Parameters:** bool enable

### DumpSchemaState



**Parameters:** SchemaHandle handle, const std::string& path

### AnalyzeSchemaUsage



**Parameters:** SchemaHandle handle
