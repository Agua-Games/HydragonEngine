
## Classes

### RuntimeAnimationSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeAnimationConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### PlayAnimation



**Parameters:** EntityHandle entity, const std::string& animName, const PlaybackParams& params = {}

---

### StopAnimation



**Parameters:** AnimationHandle handle

---

### PauseAnimation



**Parameters:** AnimationHandle handle

---

### ResumeAnimation



**Parameters:** AnimationHandle handle

---

### SetBlendWeight



**Parameters:** AnimationHandle handle, float weight

---

### SetPlaybackSpeed



**Parameters:** AnimationHandle handle, float speed

---

### SetTimePosition



**Parameters:** AnimationHandle handle, float time

---

### AddAnimationLayer



**Parameters:** EntityHandle entity, const std::string& layerName

---

### RemoveAnimationLayer



**Parameters:** EntityHandle entity, const std::string& layerName

---

### SetLayerWeight



**Parameters:** EntityHandle entity, const std::string& layerName, float weight

---

### EnableIK



**Parameters:** EntityHandle entity, const std::string& chainName

---

### DisableIK



**Parameters:** EntityHandle entity, const std::string& chainName

---

### SetIKTarget



**Parameters:** EntityHandle entity, const std::string& chainName, const Vector3& target

---

### RegisterAnimationEvent



**Parameters:** const std::string& eventName, const AnimationEventCallback& callback

---

### UnregisterAnimationEvent



**Parameters:** const std::string& eventName

---
