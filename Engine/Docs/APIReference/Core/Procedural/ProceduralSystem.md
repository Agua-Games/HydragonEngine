
## Classes

### ProceduralSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ProceduralConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateGenerator



**Parameters:** const GeneratorDesc& desc

---

### DestroyGenerator



**Parameters:** GeneratorHandle handle

---

### SetSeed



**Parameters:** GeneratorHandle handle, uint64_t seed

---

### SetParameters



**Parameters:** GeneratorHandle handle, const GeneratorParams& params

---

### Generate



**Parameters:** GeneratorHandle handle, const GenerationContext& context

---

### CancelGeneration



**Parameters:** GeneratorHandle handle

---

### RegisterCallback



**Parameters:** GeneratorHandle handle, const GenerationCallback& callback

---

### UnregisterCallback



**Parameters:** GeneratorHandle handle

---
