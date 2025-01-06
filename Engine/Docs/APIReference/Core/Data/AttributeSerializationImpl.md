
# Variables
---

### `false`

- **Type:** `return`



### `start`

- **Type:** `auto`

- **Default Value:** `std::chrono::high_resolution_clock::now()`



### `success`

- **Type:** `bool`

- **Default Value:** `false`



### `false`

- **Type:** `return`



### `end`

- **Type:** `auto`

- **Default Value:** `std::chrono::high_resolution_clock::now()`



### `duration`

- **Type:** `auto`

- **Default Value:** `std::chrono::duration_cast<std::chrono::milliseconds>(end - start)`



### `success`

- **Type:** `return`



### `false`

- **Type:** `return`



### `header`

- **Type:** `SerializationHeader`



### `false`

- **Type:** `return`



### `i`

- **Type:** `size_t`

- **Default Value:** `0`



### `attr`

- **Type:** `const auto&`

- **Default Value:** `attributes[i]`



### `desc`

- **Type:** `const auto&`

- **Default Value:** `descriptions[i]`



### `size`

- **Type:** `size_t`

- **Default Value:** `attr->Size()`



### `rawData`

- **Type:** `const uint8_t*`

- **Default Value:** `static_cast<const uint8_t*>(attr->GetRawData())`



### `dataSize`

- **Type:** `size_t`

- **Default Value:** `size * GetValueTypeSize(desc.type)`



### `false`

- **Type:** `return`



### `true`

- **Type:** `return`



### `header`

- **Type:** `SerializationHeader`



### `false`

- **Type:** `return`



### `i`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `desc`

- **Type:** `AttributeDesc`



### `size`

- **Type:** `size_t`



### `storage`

- **Type:** `auto`

- **Default Value:** `CreateStorage(desc.type, desc.owner)`



### `dataSize`

- **Type:** `size_t`

- **Default Value:** `size * GetValueTypeSize(desc.type)`



### `rawData`

- **Type:** `uint8_t*`

- **Default Value:** `static_cast<uint8_t*>(storage->GetRawData())`



### `false`

- **Type:** `return`



### `true`

- **Type:** `return`



### `destLen`

- **Type:** `uLongf`

- **Default Value:** `output.size()`



### `result`

- **Type:** `int`

- **Default Value:** `compress2(
        output.data(),
        &destLen,
        input.data(),
        input.size(),
        Z_BEST_COMPRESSION
    )`



### `false`

- **Type:** `return`



### `true`

- **Type:** `return`



### `destLen`

- **Type:** `uLongf`

- **Default Value:** `output.size()`



### `result`

- **Type:** `int`

- **Default Value:** `uncompress(
            output.data(),
            &destLen,
            input.data(),
            input.size()
        )`



### `false`

- **Type:** `return`



### `true`

- **Type:** `return`



### `root`

- **Type:** `json`



### `attributesArray`

- **Type:** `json`

- **Default Value:** `nlohmann::json::array()`



### `i`

- **Type:** `size_t`

- **Default Value:** `0`



### `attr`

- **Type:** `const auto&`

- **Default Value:** `attributes[i]`



### `desc`

- **Type:** `const auto&`

- **Default Value:** `descriptions[i]`



### `attributeObj`

- **Type:** `json`



### `dataArray`

- **Type:** `json`

- **Default Value:** `nlohmann::json::array()`



### `rawData`

- **Type:** `const void*`

- **Default Value:** `attr->GetRawData()`



### `j`

- **Type:** `size_t`

- **Default Value:** `0`



### `strings`

- **Type:** `const auto&`

- **Default Value:** `*static_cast<const std::vector<std::string>*>(rawData)`



### `true`

- **Type:** `return`



### `false`

- **Type:** `return`



### `root`

- **Type:** `json`



### `attributesArray`

- **Type:** `const auto&`

- **Default Value:** `root["attributes"]`



### `desc`

- **Type:** `AttributeDesc`



### `size`

- **Type:** `size_t`

- **Default Value:** `attributeObj["size"].get<size_t>()`



### `storage`

- **Type:** `auto`

- **Default Value:** `CreateStorage(desc.type, desc.owner)`



### `dataArray`

- **Type:** `const auto&`

- **Default Value:** `attributeObj["data"]`



### `rawData`

- **Type:** `void*`

- **Default Value:** `storage->GetRawData()`



### `i`

- **Type:** `size_t`

- **Default Value:** `0`



### `strings`

- **Type:** `auto&`

- **Default Value:** `*static_cast<std::vector<std::string>*>(rawData)`



### `true`

- **Type:** `return`



### `false`

- **Type:** `return`




# Functions
---

## file



- **Parameters:** filename, std::ios::binary

- **Return:** `ofstream`

---

## SerializeValue



- **Parameters:** stream, desc.name

- **Return:** `Write attribute metadata`

---

## GetValueTypeSize



- **Parameters:** desc.type

- **Return:** `size *`

---

## DeserializeValue



- **Parameters:** stream, desc.name

- **Return:** `Read metadata`

---

## GetValueTypeSize



- **Parameters:** desc.type

- **Return:** `size *`

---
