# Classes
---

## AudioSystem
---




# Variables
---

### `maxSources`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxBuffers`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `maxEffects`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `enableSpatialization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEffects`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStreamingAudio`

- **Type:** `bool`

- **Default Value:** `true`



### `streamingBufferSize`

- **Type:** `size_t`

- **Default Value:** `32 * 1024 * 1024`



### `sampleRate`

- **Type:** `uint32_t`

- **Default Value:** `48000`



### `channels`

- **Type:** `uint32_t`

- **Default Value:** `2`



### `config`

- **Type:** `const AudioConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const PlayParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AudioConfig`



### `m_Stats`

- **Type:** `AudioStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AudioConfig`

- **Description:** 

- **Members:**

  - `1024`: `32 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static AudioSystem&`

---

## Initialize



- **Parameters:** const AudioConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateSound



- **Parameters:** const SoundDesc& desc

- **Return:** `SoundHandle`

---

## DestroySound



- **Parameters:** SoundHandle handle

- **Return:** `void`

---

## PlaySound



- **Parameters:** SoundHandle sound, const PlayParams& params = {}

- **Return:** `SourceHandle`

---

## StopSound



- **Parameters:** SourceHandle source

- **Return:** `void`

---

## PauseSound



- **Parameters:** SourceHandle source

- **Return:** `void`

---

## ResumeSound



- **Parameters:** SourceHandle source

- **Return:** `void`

---

## SetListenerTransform



- **Parameters:** const Transform& transform

- **Return:** `void`

---

## SetSourceTransform



- **Parameters:** SourceHandle source, const Transform& transform

- **Return:** `void`

---

## SetVolume



- **Parameters:** SourceHandle source, float volume

- **Return:** `void`

---

## SetPitch



- **Parameters:** SourceHandle source, float pitch

- **Return:** `void`

---

## CreateEffect



- **Parameters:** const EffectDesc& desc

- **Return:** `EffectHandle`

---

## DestroyEffect



- **Parameters:** EffectHandle handle

- **Return:** `void`

---

## ApplyEffect



- **Parameters:** SourceHandle source, EffectHandle effect

- **Return:** `void`

---
