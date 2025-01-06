
## Classes

### RuntimeSerializationSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeSerializationConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### BeginSerialize



**Parameters:** const std::string& identifier

### EndSerialize



**Parameters:** SerializeHandle handle

### BeginDeserialize



**Parameters:** const std::string& identifier

### EndDeserialize



**Parameters:** DeserializeHandle handle

### RegisterSerializer



**Parameters:** const std::string& type, std::unique_ptr<ITypeSerializer> serializer

### UnregisterSerializer



**Parameters:** const std::string& type

### SaveState



**Parameters:** const std::string& name, const SerializationData& data

### LoadState



**Parameters:** const std::string& name, SerializationData& data

### ExportData



**Parameters:** const std::string& path, const SerializationData& data

### ImportData



**Parameters:** const std::string& path, SerializationData& data

### CreateSnapshot



**Parameters:** const std::string& name

### RestoreSnapshot



**Parameters:** const std::string& name

### DeleteSnapshot



**Parameters:** const std::string& name
