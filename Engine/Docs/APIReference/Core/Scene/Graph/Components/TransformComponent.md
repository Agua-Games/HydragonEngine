# Classes
---

## TransformComponent
---




# Variables
---

### `m_LocalTransform`

- **Type:** `return`



### `m_WorldTransform`

- **Type:** `return`



### `parentTransform`

- **Type:** `const Transform*`

- **Default Value:** `nullptr)`



### `m_Dirty`

- **Type:** `return`



### `m_LocalTransform`

- **Type:** `Transform`



### `m_WorldTransform`

- **Type:** `Transform`



### `m_Dirty`

- **Type:** `bool`

- **Default Value:** `true`




# Functions
---

## SetPosition



- **Parameters:** `const Vector3& position`

- **Return:** `void`

---

## SetRotation



- **Parameters:** `const Quaternion& rotation`

- **Return:** `void`

---

## SetScale



- **Parameters:** `const Vector3& scale`

- **Return:** `void`

---

## Translate



- **Parameters:** `const Vector3& translation`

- **Return:** `void`

---

## Rotate



- **Parameters:** `const Quaternion& rotation`

- **Return:** `void`

---

## Scale



- **Parameters:** `const Vector3& scale`

- **Return:** `void`

---

## UpdateWorldTransform



- **Parameters:** `const Transform* parentTransform = nullptr`

- **Return:** `void`

---
