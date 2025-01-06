# Classes
---

## ConfigSchema
---




# Variables
---

### `type`

- **Type:** `ValueType`

- **Default Value:** `ValueType::Null`



### `required`

- **Type:** `bool`

- **Default Value:** `false`



### `unique`

- **Type:** `bool`

- **Default Value:** `false`



### `defaultValue`

- **Type:** `Variant`



### `description`

- **Type:** `string`



### `enableInheritance`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVersioning`

- **Type:** `bool`

- **Default Value:** `true`



### `strict`

- **Type:** `bool`

- **Default Value:** `false`



### `version`

- **Type:** `string`

- **Default Value:** `"1.0"`



### `description`

- **Type:** `string`



### `desc`

- **Type:** `const SchemaDesc&`

- **Default Value:** `{})`



### `m_Desc`

- **Type:** `SchemaDesc`



### `m_Stats`

- **Type:** `SchemaStats`



### `m_Valid`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `FieldDesc`

- **Description:** 

- **Members:**

  - `defaultValue`: `Variant` - 

  - `description`: `string` - 



### `SchemaDesc`

- **Description:** 

- **Members:**

  - `description`: `string` - 




# Functions
---

## Create



- **Parameters:** `const std::string& name`, `const SchemaDesc& desc = {}`

- **Return:** `static SchemaHandle`

---

## AddField



- **Parameters:** `const std::string& name`, `const FieldDesc& desc`

- **Return:** `void`

---

## RemoveField



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## SetFieldRequired



- **Parameters:** `const std::string& name`, `bool required`

- **Return:** `void`

---

## SetFieldDefault



- **Parameters:** `const std::string& name`, `const Variant& value`

- **Return:** `void`

---

## AddValidationRule



- **Parameters:** `const std::string& field`, `const ValidationRule& rule`

- **Return:** `void`

---

## RemoveValidationRule



- **Parameters:** `const std::string& field`, `RuleHandle rule`

- **Return:** `void`

---

## InheritFrom



- **Parameters:** `SchemaHandle parent`

- **Return:** `void`

---

## RemoveInheritance



- **Parameters:** ``

- **Return:** `void`

---

## SetVersion



- **Parameters:** `const std::string& version`

- **Return:** `void`

---
