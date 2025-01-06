
## Classes

### SerializationFormat



### AttributeSerializer




## Functions

### SaveToFile



**Parameters:** const std::string& filename, 
                   const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   const std::vector<AttributeDesc>& descriptions

### LoadFromFile



**Parameters:** const std::string& filename,
                     std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                     std::vector<AttributeDesc>& descriptions

### ValidateFormat



**Parameters:** const std::string& filename

### GetFormatVersion



**Parameters:** const std::string& filename

### GetStats



**Parameters:** const std::string& filename

### SaveBinary



**Parameters:** std::ostream& stream,
                   const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   const std::vector<AttributeDesc>& descriptions

### LoadBinary



**Parameters:** std::istream& stream,
                   std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                   std::vector<AttributeDesc>& descriptions

### SaveJSON



**Parameters:** std::ostream& stream,
                 const std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                 const std::vector<AttributeDesc>& descriptions

### LoadJSON



**Parameters:** std::istream& stream,
                 std::vector<std::shared_ptr<IAttributeStorage>>& attributes,
                 std::vector<AttributeDesc>& descriptions

### SerializeValue



**Parameters:** std::ostream& stream, const T& value

### DeserializeValue



**Parameters:** std::istream& stream, T& value
