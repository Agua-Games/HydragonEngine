
## Classes

### AnimationBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const AnimationBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateAnimator



**Parameters:** const AnimatorDesc& desc

### DestroyAnimator



**Parameters:** AnimatorHandle handle

### SetSkeleton



**Parameters:** AnimatorHandle handle, const SkeletonDesc& desc

### CreateClip



**Parameters:** const std::string& name, const ClipDesc& desc

### DestroyClip



**Parameters:** ClipHandle handle

### SetClipSpeed



**Parameters:** ClipHandle handle, float speed

### SetClipWeight



**Parameters:** ClipHandle handle, float weight

### Play



**Parameters:** AnimatorHandle handle, ClipHandle clip, const PlaybackParams& params = {}

### Stop



**Parameters:** AnimatorHandle handle, ClipHandle clip

### Pause



**Parameters:** AnimatorHandle handle, ClipHandle clip

### Resume



**Parameters:** AnimatorHandle handle, ClipHandle clip

### CrossFade



**Parameters:** AnimatorHandle handle, ClipHandle fromClip, ClipHandle toClip, float duration

### AddLayer



**Parameters:** AnimatorHandle handle, const LayerDesc& desc

### RemoveLayer



**Parameters:** AnimatorHandle handle, uint32_t layerIndex

### SetLayerWeight



**Parameters:** AnimatorHandle handle, uint32_t layerIndex, float weight

### SetLayerMask



**Parameters:** AnimatorHandle handle, uint32_t layerIndex, const BoneMask& mask

### SetIKTarget



**Parameters:** AnimatorHandle handle, const std::string& chainName, const Vector3& target

### SetIKRotation



**Parameters:** AnimatorHandle handle, const std::string& chainName, const Quaternion& rotation

### SetIKEnabled



**Parameters:** AnimatorHandle handle, const std::string& chainName, bool enabled

### SetMorphWeight



**Parameters:** AnimatorHandle handle, const std::string& targetName, float weight

### BlendMorphTargets



**Parameters:** AnimatorHandle handle, const std::vector<MorphBlend>& blends

### EnableRagdoll



**Parameters:** AnimatorHandle handle, bool enable

### SetRagdollBlend



**Parameters:** AnimatorHandle handle, float blend

### ApplyRagdollForce



**Parameters:** AnimatorHandle handle, const std::string& boneName, const Vector3& force

### EnableDebugVisualization



**Parameters:** bool enable

### SetDebugBone



**Parameters:** AnimatorHandle handle, const std::string& boneName

### DumpAnimatorState



**Parameters:** AnimatorHandle handle, const std::string& path
