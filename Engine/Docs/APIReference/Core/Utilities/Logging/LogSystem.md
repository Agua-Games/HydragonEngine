# Classes
---

## LogLevel
---



## LogSystem
---




# Variables
---

### `enableConsole`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFile`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimestamps`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSourceLocation`

- **Type:** `bool`

- **Default Value:** `true`



### `logFilePath`

- **Type:** `string`

- **Default Value:** `"game.log"`



### `minLevel`

- **Type:** `LogLevel`

- **Default Value:** `LogLevel::Info`



### `config`

- **Type:** `const LogConfig&`

- **Default Value:** `{})`



### `location`

- **Type:** `source_location&`

- **Default Value:** `std::source_location::current())`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `LogConfig`



### `m_Stats`

- **Type:** `LogStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `LogConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static LogSystem&`

---

## Initialize



- **Parameters:** `const LogConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Trace



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## Debug



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## Info



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## Warning



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## Error



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## Fatal



- **Parameters:** `const std::string& message`

- **Return:** `void`

---

## SetLogLevel



- **Parameters:** `LogLevel level`

- **Return:** `void`

---

## AddLogHandler



- **Parameters:** `std::unique_ptr<ILogHandler> handler`

- **Return:** `void`

---
