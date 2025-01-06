
## Classes

### AttributeSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AttributeSystemConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateGroup



**Parameters:** const std::string& name, const GroupDesc& desc = {}

### DeleteGroup



**Parameters:** const std::string& name

### AddToGroup



**Parameters:** const std::string& name, size_t index

### RemoveFromGroup



**Parameters:** const std::string& name, size_t index

### InterpolateAttributes



**Parameters:** const std::string& sourceName, const std::string& targetName

### TransferAttributes



**Parameters:** AttributeOwner sourceOwner, AttributeOwner targetOwner

### CompactAttributes



**Parameters:** const std::string& pattern = "*"

### RegisterAttributeHandler



**Parameters:** std::unique_ptr<IAttributeHandler> handler

### RegisterGroupHandler



**Parameters:** std::unique_ptr<IGroupHandler> handler

### ValidateAttributeIntegrity



**Parameters:** 

### NotifyAttributeCreated



**Parameters:** const std::string& name, const AttributeDesc& desc

### NotifyAttributeModified



**Parameters:** const std::string& name, size_t index
