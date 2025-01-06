
## Classes

### AnimationSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AnimationConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** float deltaTime

### CreateAnimation



**Parameters:** const AnimationDesc& desc

### DestroyAnimation



**Parameters:** AnimationHandle handle

### CreateSkeleton



**Parameters:** const SkeletonDesc& desc

### DestroySkeleton



**Parameters:** SkeletonHandle handle

### PlayAnimation



**Parameters:** AnimationHandle anim, SkeletonHandle skeleton

### StopAnimation



**Parameters:** AnimationHandle anim, SkeletonHandle skeleton

### BlendAnimation



**Parameters:** AnimationHandle from, AnimationHandle to, float blendTime

### SetAnimationTime



**Parameters:** AnimationHandle handle, float time

### SetAnimationSpeed



**Parameters:** AnimationHandle handle, float speed
