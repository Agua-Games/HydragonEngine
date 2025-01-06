# Classes
---

## OutputSystem
---




# Variables
---

### `enableFileOutput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableConsoleOutput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRemoteOutput`

- **Type:** `bool`

- **Default Value:** `false`



### `enableTimestamps`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSourceLocation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableThreadId`

- **Type:** `bool`

- **Default Value:** `true`



### `maxQueueSize`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxFileSize`

- **Type:** `size_t`

- **Default Value:** `100 * 1024 * 1024`



### `outputDirectory`

- **Type:** `string`



### `config`

- **Type:** `const OutputConfig&`

- **Default Value:** `{})`



### `file`

- **Type:** `const char*`

- **Default Value:** `nullptr, int line = 0)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `OutputConfig`



### `m_Stats`

- **Type:** `OutputStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `OutputConfig`

- **Description:** 

- **Members:**

  - `1024`: `100 * 1024 *` - 

  - `outputDirectory`: `string` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static OutputSystem&`

---

## Initialize



- **Parameters:** `const OutputConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## Log



- **Parameters:** `const LogMessage& message`

- **Return:** `void`

---

## LogFormat



- **Parameters:** `LogLevel level`, `const char* format`, `...`

- **Return:** `void`

---

## LogError



- **Parameters:** `const char* message`, `const char* file = nullptr`, `int line = 0`

- **Return:** `void`

---

## AddOutputChannel



- **Parameters:** `std::unique_ptr<IOutputChannel> channel`

- **Return:** `void`

---

## RemoveOutputChannel



- **Parameters:** `OutputChannelId id`

- **Return:** `void`

---

## SetLogLevel



- **Parameters:** `LogLevel level`

- **Return:** `void`

---

## SetLogFilter



- **Parameters:** `const LogFilter& filter`

- **Return:** `void`

---

## Flush



- **Parameters:** ``

- **Return:** `void`

---
