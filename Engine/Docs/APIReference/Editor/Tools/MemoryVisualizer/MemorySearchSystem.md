
# Variables
---

### `block`

- **Type:** `MemoryBlock*`



### `matchScore`

- **Type:** `float`



### `caseSensitive`

- **Type:** `bool`

- **Default Value:** `false`



### `searchInTags`

- **Type:** `bool`

- **Default Value:** `true`



### `searchInAddresses`

- **Type:** `bool`

- **Default Value:** `true`



### `fuzzyMatch`

- **Type:** `bool`

- **Default Value:** `true`



### `options`

- **Type:** `const SearchOptions&`

- **Default Value:** `SearchOptions())`



### `lastQuery`

- **Type:** `QString`



### `lastOptions`

- **Type:** `SearchOptions`



### `timestamp`

- **Type:** `time_point`



### `m_Cache`

- **Type:** `SearchCache`



### `m_Index`

- **Type:** `SearchIndex`




# Structs
---

### `SearchResult`

- **Description:** 

- **Members:**

  - `block`: `MemoryBlock*` - 

  - `matchScore`: `float` - 



### `SearchOptions`

- **Description:** 

- **Members:**



### `SearchCache`

- **Description:** 

- **Members:**

  - `lastQuery`: `QString` - 

  - `lastOptions`: `SearchOptions` - 

  - `timestamp`: `time_point` - 



### `SearchIndex`

- **Description:** 

- **Members:**




# Functions
---

## searchCompleted



- **Parameters:** const std::vector<SearchResult>& results

- **Return:** `void`

---

## resultSelected



- **Parameters:** const Memory::MemoryBlock* block

- **Return:** `void`

---

## calculateRelevance



- **Parameters:** const Memory::MemoryBlock& block, 
                           const QString& query,
                           const SearchOptions& options

- **Return:** `float`

---

## matchesPattern



- **Parameters:** const QString& text, 
                       const QString& pattern,
                       const SearchOptions& options

- **Return:** `bool`

---

## updateSearchIndex



- **Parameters:** 

- **Return:** `void`

---

## pruneCache



- **Parameters:** 

- **Return:** `void`

---
