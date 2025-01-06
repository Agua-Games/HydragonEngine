# Classes
---

## NetworkBindings
---




# Variables
---

### `enableReplication`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEncryption`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugStats`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConnections`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxPacketSize`

- **Type:** `uint32_t`

- **Default Value:** `1024`



### `sendBufferSize`

- **Type:** `uint32_t`

- **Default Value:** `32768`



### `networkConfigPath`

- **Type:** `string`

- **Default Value:** `"Config/Network"`



### `config`

- **Type:** `const NetworkBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `NetworkBindingsConfig`



### `m_Stats`

- **Type:** `NetworkBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `NetworkBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static NetworkBindings&`

---

## Initialize



- **Parameters:** const NetworkBindingsConfig& config = {}

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

## Connect



- **Parameters:** const std::string& address, uint16_t port

- **Return:** `ConnectionHandle`

---

## Disconnect



- **Parameters:** ConnectionHandle handle

- **Return:** `void`

---

## SendMessage



- **Parameters:** ConnectionHandle handle, const NetworkMessage& message

- **Return:** `void`

---

## BroadcastMessage



- **Parameters:** const NetworkMessage& message

- **Return:** `void`

---

## RegisterMessageHandler



- **Parameters:** MessageType type, const MessageHandler& handler

- **Return:** `void`

---

## UnregisterMessageHandler



- **Parameters:** MessageType type

- **Return:** `void`

---

## RegisterReplicatedObject



- **Parameters:** const std::string& name, const ReplicatedObject& object

- **Return:** `void`

---

## UnregisterReplicatedObject



- **Parameters:** const std::string& name

- **Return:** `void`

---

## UpdateReplicatedObject



- **Parameters:** const std::string& name, const ReplicatedState& state

- **Return:** `void`

---

## EnablePacketLogging



- **Parameters:** bool enable

- **Return:** `void`

---

## DumpNetworkStats



- **Parameters:** const std::string& path

- **Return:** `void`

---

## SimulateLatency



- **Parameters:** uint32_t milliseconds

- **Return:** `void`

---

## SimulatePacketLoss



- **Parameters:** float percentage

- **Return:** `void`

---
