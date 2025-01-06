# Classes
---

## AssetValidationSystem
---




# Variables
---

### `enableAutoValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableReferenceChecking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFormatValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSizeChecks`

- **Type:** `bool`

- **Default Value:** `true`



### `maxValidationThreads`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `maxAssetSize`

- **Type:** `size_t`

- **Default Value:** `2ULL * 1024 * 1024 * 1024`



### `config`

- **Type:** `const AssetValidationConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetValidationConfig`



### `m_Stats`

- **Type:** `AssetValidationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetValidationConfig`

- **Description:** 

- **Members:**

  - `1024`: `2ULL * 1024 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static AssetValidationSystem&`

---

## Initialize



- **Parameters:** const AssetValidationConfig& config = {}

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

## ValidateAsset



- **Parameters:** const std::string& path

- **Return:** `void`

---

## ValidateFolder



- **Parameters:** const std::string& path

- **Return:** `void`

---

## ValidateReferences



- **Parameters:** const std::string& path

- **Return:** `void`

---

## AddValidationRule



- **Parameters:** const std::string& assetType, 
                          const ValidationRule& rule

- **Return:** `void`

---

## RemoveValidationRule



- **Parameters:** const std::string& assetType, 
                            const std::string& ruleName

- **Return:** `void`

---

## RegisterValidator



- **Parameters:** const std::string& assetType, 
                         std::unique_ptr<IAssetValidator> validator

- **Return:** `void`

---
