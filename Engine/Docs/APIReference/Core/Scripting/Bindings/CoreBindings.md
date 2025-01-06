# Classes
---

## CoreBindings
---




# Variables
---

### `enableReflection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTypeConversion`

- **Type:** `bool`

- **Default Value:** `true`



### `enableExceptionHandling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugInfo`

- **Type:** `bool`

- **Default Value:** `true`



### `maxCallStackDepth`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `scriptRootPath`

- **Type:** `string`

- **Default Value:** `"Scripts"`



### `config`

- **Type:** `const CoreBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `CoreBindingsConfig`



### `m_Stats`

- **Type:** `CoreBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `CoreBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static CoreBindings&`

---

## Initialize



- **Parameters:** `const CoreBindingsConfig& config = {}`

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

## RegisterType



- **Parameters:** `const std::string& name`, `const TypeInfo& typeInfo`

- **Return:** `void`

---

## UnregisterType



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## RegisterFunction



- **Parameters:** `const std::string& name`, `const FunctionInfo& funcInfo`

- **Return:** `void`

---

## UnregisterFunction



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## RegisterEnum



- **Parameters:** `const std::string& name`, `const EnumInfo& enumInfo`

- **Return:** `void`

---

## UnregisterEnum



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## RegisterConverter



- **Parameters:** `const TypePair& types`, `std::unique_ptr<ITypeConverter> converter`

- **Return:** `void`

---

## UnregisterConverter



- **Parameters:** `const TypePair& types`

- **Return:** `void`

---
