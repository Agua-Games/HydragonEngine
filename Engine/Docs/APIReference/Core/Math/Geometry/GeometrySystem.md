
## Classes

### GeometrySystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const GeometryConfig& config = {}

### Shutdown



**Parameters:** 

### RayIntersectsTriangle



**Parameters:** const Ray& ray, const Triangle& triangle, float* t = nullptr

### RayIntersectsMesh



**Parameters:** const Ray& ray, const Mesh& mesh, RayHit* hit = nullptr

### SpheresIntersect



**Parameters:** const Sphere& a, const Sphere& b

### PointToLineDistance



**Parameters:** const Vector3& point, const Line& line

### PointToPlaneDistance



**Parameters:** const Vector3& point, const Plane& plane

### GenerateNormals



**Parameters:** Mesh* mesh

### GenerateTangents



**Parameters:** Mesh* mesh

### OptimizeMesh



**Parameters:** Mesh* mesh, const MeshOptimizationParams& params
