
## Classes

### NetworkSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const NetworkConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### Connect



**Parameters:** const ConnectionDesc& desc

---

### Disconnect



**Parameters:** ConnectionHandle handle

---

### Send



**Parameters:** ConnectionHandle conn, const NetworkMessage& msg, const SendConfig& config = {}

---

### Broadcast



**Parameters:** const NetworkMessage& msg, const SendConfig& config = {}

---

### AddMessageHandler



**Parameters:** MessageType type, const MessageHandler& handler

---

### RemoveMessageHandler



**Parameters:** MessageType type

---

### EnableReplication



**Parameters:** EntityHandle entity

---

### DisableReplication



**Parameters:** EntityHandle entity

---

### SetReplicationFilter



**Parameters:** EntityHandle entity, const ReplicationFilter& filter

---
