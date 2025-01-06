# Classes
---

## RuntimeLocalizationSystem
---




# Variables
---

### `enableDynamicLoading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFallback`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePluralization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableContextualVariants`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConcurrentLoads`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `cacheSize`

- **Type:** `uint32_t`

- **Default Value:** `1024`



### `defaultLocale`

- **Type:** `string`

- **Default Value:** `"en-US"`



### `fallbackLocale`

- **Type:** `string`

- **Default Value:** `"en-US"`



### `config`

- **Type:** `const RuntimeLocalizationConfig&`

- **Default Value:** `{})`



### `setActive`

- **Type:** `bool`

- **Default Value:** `false)`



### `params`

- **Type:** `const LocalizationParams&`

- **Default Value:** `{})`



### `params`

- **Type:** `const LocalizationParams&`

- **Default Value:** `{})`



### `params`

- **Type:** `const LocalizationParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeLocalizationConfig`



### `m_Stats`

- **Type:** `RuntimeLocalizationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeLocalizationConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeLocalizationSystem&`

---

## Initialize



- **Parameters:** `const RuntimeLocalizationConfig& config = {}`

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

## LoadLocale



- **Parameters:** `const std::string& locale`, `bool setActive = false`

- **Return:** `void`

---

## UnloadLocale



- **Parameters:** `const std::string& locale`

- **Return:** `void`

---

## SetActiveLocale



- **Parameters:** `const std::string& locale`

- **Return:** `void`

---

## GetText



- **Parameters:** `const std::string& key`, `const LocalizationParams& params = {}`

- **Return:** `string`

---

## GetPlural



- **Parameters:** `const std::string& key`, `int count`, `const LocalizationParams& params = {}`

- **Return:** `string`

---

## GetContextual



- **Parameters:** `const std::string& key`, `const std::string& context`, `const LocalizationParams& params = {}`

- **Return:** `string`

---

## RegisterFormatter



- **Parameters:** `const std::string& name`, `std::unique_ptr<ILocalizationFormatter> formatter`

- **Return:** `void`

---

## UnregisterFormatter



- **Parameters:** `const std::string& name`

- **Return:** `void`

---
