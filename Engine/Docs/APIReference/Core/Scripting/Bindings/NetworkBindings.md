
## Classes

### NetworkBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const NetworkBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### Connect



**Parameters:** const std::string& address, uint16_t port

### Disconnect



**Parameters:** ConnectionHandle handle

### SendMessage



**Parameters:** ConnectionHandle handle, const NetworkMessage& message

### BroadcastMessage



**Parameters:** const NetworkMessage& message

### RegisterMessageHandler



**Parameters:** MessageType type, const MessageHandler& handler

### UnregisterMessageHandler



**Parameters:** MessageType type

### RegisterReplicatedObject



**Parameters:** const std::string& name, const ReplicatedObject& object

### UnregisterReplicatedObject



**Parameters:** const std::string& name

### UpdateReplicatedObject



**Parameters:** const std::string& name, const ReplicatedState& state

### EnablePacketLogging



**Parameters:** bool enable

### DumpNetworkStats



**Parameters:** const std::string& path

### SimulateLatency



**Parameters:** uint32_t milliseconds

### SimulatePacketLoss



**Parameters:** float percentage
