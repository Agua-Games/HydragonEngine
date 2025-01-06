
## Classes

### RuntimeNetworkSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeNetworkConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateSession



**Parameters:** const SessionDesc& desc

### DestroySession



**Parameters:** NetworkSessionHandle handle

### JoinSession



**Parameters:** const std::string& address, const JoinParams& params = {}

### LeaveSession



**Parameters:** 

### ConnectPeer



**Parameters:** const PeerDesc& desc

### DisconnectPeer



**Parameters:** ConnectionHandle handle

### SendData



**Parameters:** ConnectionHandle handle, const NetworkPacket& packet, SendPriority priority = SendPriority::Normal

### BroadcastData



**Parameters:** const NetworkPacket& packet, SendPriority priority = SendPriority::Normal

### EnableReplication



**Parameters:** EntityHandle entity, const ReplicationConfig& config = {}

### DisableReplication



**Parameters:** EntityHandle entity

### SetReplicationAuthority



**Parameters:** EntityHandle entity, ConnectionHandle authority

### RegisterPacketHandler



**Parameters:** PacketType type, const PacketHandler& handler

### UnregisterPacketHandler



**Parameters:** PacketType type

### RegisterReplicationFilter



**Parameters:** const std::string& name, std::unique_ptr<IReplicationFilter> filter

### UnregisterReplicationFilter



**Parameters:** const std::string& name
