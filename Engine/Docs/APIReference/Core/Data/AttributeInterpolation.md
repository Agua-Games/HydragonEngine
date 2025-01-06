# Classes
---

## InterpolationMethod
---



## AttributeInterpolator
---




# Variables
---

### `method`

- **Type:** `InterpolationMethod`

- **Default Value:** `InterpolationMethod::Linear`



### `smoothness`

- **Type:** `float`

- **Default Value:** `0.5f`



### `extrapolate`

- **Type:** `bool`

- **Default Value:** `false`



### `samples`

- **Type:** `uint32_t`

- **Default Value:** `10`



### `config`

- **Type:** `const InterpolationConfig&`

- **Default Value:** `{})
        : m_Config(config) {}

    template<typename T>
    bool Interpolate(const IAttributeStorage* source, 
                    IAttributeStorage* target,
                    size_t startIndex,
                    size_t endIndex,
                    float t) {
        if (!ValidateStorages(source, target)) {
            return false`



### `false`

- **Type:** `return`



### `false`

- **Type:** `return`



### `m_Config`

- **Type:** `InterpolationConfig`




# Structs
---

### `InterpolationConfig`

- **Description:** 

- **Members:**




# Functions
---

## LinearInterpolate



- **Parameters:** `const IAttributeStorage* source`, `IAttributeStorage* target`, `size_t startIndex`, `size_t endIndex`, `float t`

- **Return:** `bool`

---

## SmoothInterpolate



- **Parameters:** `const IAttributeStorage* source`, `IAttributeStorage* target`, `size_t startIndex`, `size_t endIndex`, `float t`

- **Return:** `bool`

---

## CubicInterpolate



- **Parameters:** `const IAttributeStorage* source`, `IAttributeStorage* target`, `size_t startIndex`, `size_t endIndex`, `float t`

- **Return:** `bool`

---

## NearestInterpolate



- **Parameters:** `const IAttributeStorage* source`, `IAttributeStorage* target`, `size_t startIndex`, `size_t endIndex`, `float t`

- **Return:** `bool`

---

## CustomInterpolate



- **Parameters:** `const IAttributeStorage* source`, `IAttributeStorage* target`, `size_t startIndex`, `size_t endIndex`, `float t`

- **Return:** `bool`

---
