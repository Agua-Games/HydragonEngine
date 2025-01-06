# Classes
---

## RuleSeverity
---



## ValidationRule
---



## ValidationContext
---




# Variables
---

### `expression`

- **Type:** `string`



### `context`

- **Type:** `string`



### `action`

- **Type:** `string`



### `autoFix`

- **Type:** `bool`

- **Default Value:** `false`



### `name`

- **Type:** `string`



### `description`

- **Type:** `string`



### `severity`

- **Type:** `RuleSeverity`

- **Default Value:** `RuleSeverity::Error`



### `enabled`

- **Type:** `bool`

- **Default Value:** `true`



### `inherited`

- **Type:** `bool`

- **Default Value:** `false`



### `desc`

- **Type:** `const RuleDesc&`

- **Default Value:** `{})`



### `m_Desc`

- **Type:** `RuleDesc`



### `m_Condition`

- **Type:** `RuleCondition`



### `m_Action`

- **Type:** `RuleAction`



### `m_Stats`

- **Type:** `RuleStats`



### `m_Valid`

- **Type:** `bool`

- **Default Value:** `false`



### `m_Value`

- **Type:** `Variant`



### `m_Path`

- **Type:** `string`



### `m_Schema`

- **Type:** `const ConfigSchema*`

- **Default Value:** `nullptr`



### `m_Rule`

- **Type:** `const ValidationRule*`

- **Default Value:** `nullptr`



### `message`

- **Type:** `string`



### `path`

- **Type:** `string`



### `severity`

- **Type:** `RuleSeverity`



### `rule`

- **Type:** `RuleHandle`



### `canAutoFix`

- **Type:** `bool`

- **Default Value:** `false`



### `success`

- **Type:** `bool`

- **Default Value:** `false`



### `stats`

- **Type:** `ValidationStats`



### `totalRules`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `activeRules`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `totalValidations`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `failedValidations`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `autoFixCount`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `averageValidationTime`

- **Type:** `float`

- **Default Value:** `0.0f`




# Structs
---

### `RuleCondition`

- **Description:** 

- **Members:**

  - `expression`: `string` - 

  - `context`: `string` - 



### `RuleAction`

- **Description:** 

- **Members:**

  - `action`: `string` - 



### `RuleDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `description`: `string` - 



### `ValidationError`

- **Description:** 

- **Members:**

  - `message`: `string` - 

  - `path`: `string` - 

  - `severity`: `RuleSeverity` - 

  - `rule`: `RuleHandle` - 



### `ValidationResult`

- **Description:** 

- **Members:**

  - `stats`: `ValidationStats` - 



### `ValidationStats`

- **Description:** 

- **Members:**




# Functions
---

## Create



- **Parameters:** `const std::string& name`, `const RuleDesc& desc = {}`

- **Return:** `static RuleHandle`

---

## SetCondition



- **Parameters:** `const RuleCondition& condition`

- **Return:** `void`

---

## SetAction



- **Parameters:** `const RuleAction& action`

- **Return:** `void`

---

## SetSeverity



- **Parameters:** `RuleSeverity severity`

- **Return:** `void`

---

## SetEnabled



- **Parameters:** `bool enabled`

- **Return:** `void`

---

## InheritFrom



- **Parameters:** `RuleHandle parent`

- **Return:** `void`

---

## RemoveInheritance



- **Parameters:** ``

- **Return:** `void`

---

## SetValue



- **Parameters:** `const Variant& value`

- **Return:** `void`

---

## AddError



- **Parameters:** `const ValidationError& error`

- **Return:** `void`

---

## AddWarning



- **Parameters:** `const ValidationError& warning`

- **Return:** `void`

---
