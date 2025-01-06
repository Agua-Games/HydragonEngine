# Classes
---

## AnimationBindings
---




# Variables
---

### `enableBlending`

- **Type:** `bool`

- **Default Value:** `true`



### `enableIK`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMorphTargets`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRagdoll`

- **Type:** `bool`

- **Default Value:** `true`



### `maxAnimators`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxClips`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `maxLayers`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `animationDataPath`

- **Type:** `string`

- **Default Value:** `"Data/Animation"`



### `config`

- **Type:** `const AnimationBindingsConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const PlaybackParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AnimationBindingsConfig`



### `m_Stats`

- **Type:** `AnimationBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AnimationBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AnimationBindings&`

---

## Initialize



- **Parameters:** `const AnimationBindingsConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## CreateAnimator



- **Parameters:** `const AnimatorDesc& desc`

- **Return:** `AnimatorHandle`

---

## DestroyAnimator



- **Parameters:** `AnimatorHandle handle`

- **Return:** `void`

---

## SetSkeleton



- **Parameters:** `AnimatorHandle handle`, `const SkeletonDesc& desc`

- **Return:** `void`

---

## CreateClip



- **Parameters:** `const std::string& name`, `const ClipDesc& desc`

- **Return:** `ClipHandle`

---

## DestroyClip



- **Parameters:** `ClipHandle handle`

- **Return:** `void`

---

## SetClipSpeed



- **Parameters:** `ClipHandle handle`, `float speed`

- **Return:** `void`

---

## SetClipWeight



- **Parameters:** `ClipHandle handle`, `float weight`

- **Return:** `void`

---

## Play



- **Parameters:** `AnimatorHandle handle`, `ClipHandle clip`, `const PlaybackParams& params = {}`

- **Return:** `void`

---

## Stop



- **Parameters:** `AnimatorHandle handle`, `ClipHandle clip`

- **Return:** `void`

---

## Pause



- **Parameters:** `AnimatorHandle handle`, `ClipHandle clip`

- **Return:** `void`

---

## Resume



- **Parameters:** `AnimatorHandle handle`, `ClipHandle clip`

- **Return:** `void`

---

## CrossFade



- **Parameters:** `AnimatorHandle handle`, `ClipHandle fromClip`, `ClipHandle toClip`, `float duration`

- **Return:** `void`

---

## AddLayer



- **Parameters:** `AnimatorHandle handle`, `const LayerDesc& desc`

- **Return:** `void`

---

## RemoveLayer



- **Parameters:** `AnimatorHandle handle`, `uint32_t layerIndex`

- **Return:** `void`

---

## SetLayerWeight



- **Parameters:** `AnimatorHandle handle`, `uint32_t layerIndex`, `float weight`

- **Return:** `void`

---

## SetLayerMask



- **Parameters:** `AnimatorHandle handle`, `uint32_t layerIndex`, `const BoneMask& mask`

- **Return:** `void`

---

## SetIKTarget



- **Parameters:** `AnimatorHandle handle`, `const std::string& chainName`, `const Vector3& target`

- **Return:** `void`

---

## SetIKRotation



- **Parameters:** `AnimatorHandle handle`, `const std::string& chainName`, `const Quaternion& rotation`

- **Return:** `void`

---

## SetIKEnabled



- **Parameters:** `AnimatorHandle handle`, `const std::string& chainName`, `bool enabled`

- **Return:** `void`

---

## SetMorphWeight



- **Parameters:** `AnimatorHandle handle`, `const std::string& targetName`, `float weight`

- **Return:** `void`

---

## BlendMorphTargets



- **Parameters:** `AnimatorHandle handle`, `const std::vector<MorphBlend>& blends`

- **Return:** `void`

---

## EnableRagdoll



- **Parameters:** `AnimatorHandle handle`, `bool enable`

- **Return:** `void`

---

## SetRagdollBlend



- **Parameters:** `AnimatorHandle handle`, `float blend`

- **Return:** `void`

---

## ApplyRagdollForce



- **Parameters:** `AnimatorHandle handle`, `const std::string& boneName`, `const Vector3& force`

- **Return:** `void`

---

## EnableDebugVisualization



- **Parameters:** `bool enable`

- **Return:** `void`

---

## SetDebugBone



- **Parameters:** `AnimatorHandle handle`, `const std::string& boneName`

- **Return:** `void`

---

## DumpAnimatorState



- **Parameters:** `AnimatorHandle handle`, `const std::string& path`

- **Return:** `void`

---
