# Classes
---

## RuntimeNetworkSystem
---




# Variables
---

### `enableReliableUDP`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEncryption`

- **Type:** `bool`

- **Default Value:** `true`



### `enableReplication`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConnections`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `maxPacketSize`

- **Type:** `uint32_t`

- **Default Value:** `1400`



### `replicationRate`

- **Type:** `uint32_t`

- **Default Value:** `20`



### `connectionTimeout`

- **Type:** `float`

- **Default Value:** `5.0f`



### `networkProtocol`

- **Type:** `string`

- **Default Value:** `"UDP"`



### `config`

- **Type:** `const RuntimeNetworkConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const JoinParams&`

- **Default Value:** `{})`



### `priority`

- **Type:** `SendPriority`

- **Default Value:** `SendPriority::Normal)`



### `priority`

- **Type:** `SendPriority`

- **Default Value:** `SendPriority::Normal)`



### `config`

- **Type:** `const ReplicationConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeNetworkConfig`



### `m_Stats`

- **Type:** `RuntimeNetworkStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeNetworkConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeNetworkSystem&`

---

## Initialize



- **Parameters:** const RuntimeNetworkConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateSession



- **Parameters:** const SessionDesc& desc

- **Return:** `NetworkSessionHandle`

---

## DestroySession



- **Parameters:** NetworkSessionHandle handle

- **Return:** `void`

---

## JoinSession



- **Parameters:** const std::string& address, const JoinParams& params = {}

- **Return:** `void`

---

## LeaveSession



- **Parameters:** 

- **Return:** `void`

---

## ConnectPeer



- **Parameters:** const PeerDesc& desc

- **Return:** `ConnectionHandle`

---

## DisconnectPeer



- **Parameters:** ConnectionHandle handle

- **Return:** `void`

---

## SendData



- **Parameters:** ConnectionHandle handle, const NetworkPacket& packet, SendPriority priority = SendPriority::Normal

- **Return:** `void`

---

## BroadcastData



- **Parameters:** const NetworkPacket& packet, SendPriority priority = SendPriority::Normal

- **Return:** `void`

---

## EnableReplication



- **Parameters:** EntityHandle entity, const ReplicationConfig& config = {}

- **Return:** `void`

---

## DisableReplication



- **Parameters:** EntityHandle entity

- **Return:** `void`

---

## SetReplicationAuthority



- **Parameters:** EntityHandle entity, ConnectionHandle authority

- **Return:** `void`

---

## RegisterPacketHandler



- **Parameters:** PacketType type, const PacketHandler& handler

- **Return:** `void`

---

## UnregisterPacketHandler



- **Parameters:** PacketType type

- **Return:** `void`

---

## RegisterReplicationFilter



- **Parameters:** const std::string& name, std::unique_ptr<IReplicationFilter> filter

- **Return:** `void`

---

## UnregisterReplicationFilter



- **Parameters:** const std::string& name

- **Return:** `void`

---
