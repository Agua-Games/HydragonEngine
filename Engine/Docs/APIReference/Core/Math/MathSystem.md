
## Classes

### MathSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const MathConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### MultiplyMatrices



**Parameters:** const Matrix4x4* matrices, uint32_t count, Matrix4x4* results

### InvertMatrices



**Parameters:** const Matrix4x4* matrices, uint32_t count, Matrix4x4* results

### TransformVectors



**Parameters:** const Vector4* vectors, const Matrix4x4& transform, 
                         uint32_t count, Vector4* results

### NormalizeVectors



**Parameters:** const Vector4* vectors, uint32_t count, Vector4* results

### SlerpQuaternions



**Parameters:** const Quaternion* start, const Quaternion* end, 
                         float* t, uint32_t count, Quaternion* results

### BatchProcess



**Parameters:** const ComputeTask& task

### ScheduleTask



**Parameters:** const ComputeTask& task
