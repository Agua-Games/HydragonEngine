# Classes
---

## ConfigSchemaBindings
---




# Variables
---

### `enableSchemaInheritance`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCustomValidators`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSchemaVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSchemaCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `maxSchemaSize`

- **Type:** `uint32_t`

- **Default Value:** `1024 * 1024`



### `maxCustomValidators`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `schemaCacheSize`

- **Type:** `uint32_t`

- **Default Value:** `8 * 1024 * 1024`



### `schemaRootPath`

- **Type:** `string`

- **Default Value:** `"Config/Schemas"`



### `config`

- **Type:** `const ConfigSchemaBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const SchemaDesc&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ConfigSchemaBindingsConfig`



### `m_Stats`

- **Type:** `ConfigSchemaBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ConfigSchemaBindingsConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 

  - `1024`: `8 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ConfigSchemaBindings&`

---

## Initialize



- **Parameters:** const ConfigSchemaBindingsConfig& config = {}

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

## CreateSchema



- **Parameters:** const std::string& name, const SchemaDesc& desc = {}

- **Return:** `SchemaHandle`

---

## LoadSchema



- **Parameters:** const std::string& path

- **Return:** `void`

---

## SaveSchema



- **Parameters:** SchemaHandle handle, const std::string& path

- **Return:** `void`

---

## CloseSchema



- **Parameters:** SchemaHandle handle

- **Return:** `void`

---

## AddField



- **Parameters:** SchemaHandle handle, const std::string& name, const FieldDesc& desc

- **Return:** `void`

---

## RemoveField



- **Parameters:** SchemaHandle handle, const std::string& name

- **Return:** `void`

---

## SetFieldRequired



- **Parameters:** SchemaHandle handle, const std::string& name, bool required

- **Return:** `void`

---

## SetFieldDefault



- **Parameters:** SchemaHandle handle, const std::string& name, const Variant& value

- **Return:** `void`

---

## AddValidationRule



- **Parameters:** SchemaHandle handle, const std::string& field, const ValidationRule& rule

- **Return:** `void`

---

## RemoveValidationRule



- **Parameters:** SchemaHandle handle, const std::string& field, RuleHandle rule

- **Return:** `void`

---

## AddCustomValidator



- **Parameters:** const std::string& name, std::unique_ptr<ICustomValidator> validator

- **Return:** `void`

---

## InheritSchema



- **Parameters:** SchemaHandle child, SchemaHandle parent

- **Return:** `void`

---

## RemoveInheritance



- **Parameters:** SchemaHandle child

- **Return:** `void`

---

## ValidateConfig



- **Parameters:** ConfigHandle config, SchemaHandle schema

- **Return:** `ValidationResult`

---

## ValidateValue



- **Parameters:** const Variant& value, const FieldDesc& desc

- **Return:** `ValidationResult`

---

## ValidateSchema



- **Parameters:** SchemaHandle handle

- **Return:** `void`

---

## OptimizeSchema



- **Parameters:** SchemaHandle handle

- **Return:** `void`

---

## MergeSchemas



- **Parameters:** SchemaHandle target, const std::vector<SchemaHandle>& sources

- **Return:** `void`

---

## EnableSchemaTracking



- **Parameters:** bool enable

- **Return:** `void`

---

## DumpSchemaState



- **Parameters:** SchemaHandle handle, const std::string& path

- **Return:** `void`

---

## AnalyzeSchemaUsage



- **Parameters:** SchemaHandle handle

- **Return:** `void`

---
