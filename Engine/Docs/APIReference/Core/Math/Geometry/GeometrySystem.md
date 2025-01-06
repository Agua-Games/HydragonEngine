# Classes
---

## GeometrySystem
---




# Variables
---

### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGPUAcceleration`

- **Type:** `bool`

- **Default Value:** `true`



### `maxVertices`

- **Type:** `uint32_t`

- **Default Value:** `1000000`



### `maxIndices`

- **Type:** `uint32_t`

- **Default Value:** `3000000`



### `collisionPrecision`

- **Type:** `float`

- **Default Value:** `0.001f`



### `config`

- **Type:** `const GeometryConfig&`

- **Default Value:** `{})`



### `t`

- **Type:** `float*`

- **Default Value:** `nullptr)`



### `hit`

- **Type:** `RayHit*`

- **Default Value:** `nullptr)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `GeometryConfig`



### `m_Stats`

- **Type:** `GeometryStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `GeometryConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static GeometrySystem&`

---

## Initialize



- **Parameters:** `const GeometryConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## RayIntersectsTriangle



- **Parameters:** `const Ray& ray`, `const Triangle& triangle`, `float* t = nullptr`

- **Return:** `bool`

---

## RayIntersectsMesh



- **Parameters:** `const Ray& ray`, `const Mesh& mesh`, `RayHit* hit = nullptr`

- **Return:** `bool`

---

## SpheresIntersect



- **Parameters:** `const Sphere& a`, `const Sphere& b`

- **Return:** `bool`

---

## PointToLineDistance



- **Parameters:** `const Vector3& point`, `const Line& line`

- **Return:** `float`

---

## PointToPlaneDistance



- **Parameters:** `const Vector3& point`, `const Plane& plane`

- **Return:** `float`

---

## GenerateNormals



- **Parameters:** `Mesh* mesh`

- **Return:** `void`

---

## GenerateTangents



- **Parameters:** `Mesh* mesh`

- **Return:** `void`

---

## OptimizeMesh



- **Parameters:** `Mesh* mesh`, `const MeshOptimizationParams& params`

- **Return:** `void`

---
