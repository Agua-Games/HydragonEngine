# Classes
---

## ConfigValidationBindings
---




# Variables
---

### `enableCustomRules`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRuleInheritance`

- **Type:** `bool`

- **Default Value:** `true`



### `enableValidationCache`

- **Type:** `bool`

- **Default Value:** `true`



### `enableErrorReporting`

- **Type:** `bool`

- **Default Value:** `true`



### `maxRulesPerConfig`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxCustomRules`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `validationCacheSize`

- **Type:** `uint32_t`

- **Default Value:** `4 * 1024 * 1024`



### `validationRulesPath`

- **Type:** `string`

- **Default Value:** `"Config/Rules"`



### `config`

- **Type:** `const ConfigValidationBindingsConfig&`

- **Default Value:** `{})`



### `desc`

- **Type:** `const RuleDesc&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ConfigValidationBindingsConfig`



### `m_Stats`

- **Type:** `ConfigValidationBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ConfigValidationBindingsConfig`

- **Description:** 

- **Members:**

  - `1024`: `4 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static ConfigValidationBindings&`

---

## Initialize



- **Parameters:** `const ConfigValidationBindingsConfig& config = {}`

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

## CreateRule



- **Parameters:** `const std::string& name`, `const RuleDesc& desc = {}`

- **Return:** `RuleHandle`

---

## LoadRules



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## SaveRules



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## DeleteRule



- **Parameters:** `RuleHandle handle`

- **Return:** `void`

---

## SetRuleCondition



- **Parameters:** `RuleHandle handle`, `const RuleCondition& condition`

- **Return:** `void`

---

## SetRuleAction



- **Parameters:** `RuleHandle handle`, `const RuleAction& action`

- **Return:** `void`

---

## SetRuleSeverity



- **Parameters:** `RuleHandle handle`, `RuleSeverity severity`

- **Return:** `void`

---

## SetRuleEnabled



- **Parameters:** `RuleHandle handle`, `bool enabled`

- **Return:** `void`

---

## RegisterCustomRule



- **Parameters:** `const std::string& name`, `std::unique_ptr<ICustomRule> rule`

- **Return:** `void`

---

## UnregisterCustomRule



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## ValidateConfig



- **Parameters:** `ConfigHandle config`

- **Return:** `ValidationResult`

---

## ValidateSection



- **Parameters:** `ConfigHandle config`, `const std::string& section`

- **Return:** `ValidationResult`

---

## ValidateValue



- **Parameters:** `const Variant& value`, `const ValidationContext& context`

- **Return:** `ValidationResult`

---

## ClearValidationErrors



- **Parameters:** `ValidationHandle handle`

- **Return:** `void`

---

## EnableValidationTracking



- **Parameters:** `bool enable`

- **Return:** `void`

---

## DumpValidationState



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## AnalyzeRulePerformance



- **Parameters:** ``

- **Return:** `void`

---
