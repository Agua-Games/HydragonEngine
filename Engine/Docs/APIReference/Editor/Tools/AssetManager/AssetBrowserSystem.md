# Classes
---

## AssetBrowserSystem
---




# Variables
---

### `enableFiltering`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSorting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDragDrop`

- **Type:** `bool`

- **Default Value:** `true`



### `enableQuickSearch`

- **Type:** `bool`

- **Default Value:** `true`



### `maxRecentItems`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `thumbnailCacheSize`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `defaultViewMode`

- **Type:** `string`

- **Default Value:** `"Grid"`



### `config`

- **Type:** `const AssetBrowserConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AssetBrowserConfig`



### `m_Stats`

- **Type:** `AssetBrowserStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AssetBrowserConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AssetBrowserSystem&`

---

## Initialize



- **Parameters:** `const AssetBrowserConfig& config = {}`

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

## SetCurrentDirectory



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## RefreshView



- **Parameters:** ``

- **Return:** `void`

---

## SetViewMode



- **Parameters:** `const std::string& mode`

- **Return:** `void`

---

## SetFilter



- **Parameters:** `const AssetFilter& filter`

- **Return:** `void`

---

## ClearFilter



- **Parameters:** ``

- **Return:** `void`

---

## SetSortMode



- **Parameters:** `AssetSortMode mode`, `bool ascending`

- **Return:** `void`

---

## SelectAsset



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## DeselectAsset



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## ClearSelection



- **Parameters:** ``

- **Return:** `void`

---

## RegisterContextMenu



- **Parameters:** `const std::string& assetType`, `const ContextMenuCallback& callback`

- **Return:** `void`

---
