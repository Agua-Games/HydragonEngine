# Classes
---

## RuntimeAudioSystem
---




# Variables
---

### `enableSpatialization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableOcclusion`

- **Type:** `bool`

- **Default Value:** `true`



### `enableReverbZones`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDynamicMixing`

- **Type:** `bool`

- **Default Value:** `true`



### `maxVoices`

- **Type:** `uint32_t`

- **Default Value:** `128`



### `mixerBufferSize`

- **Type:** `uint32_t`

- **Default Value:** `2048`



### `sampleRate`

- **Type:** `uint32_t`

- **Default Value:** `48000`



### `dopplerScale`

- **Type:** `float`

- **Default Value:** `1.0f`



### `audioCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Audio"`



### `config`

- **Type:** `const RuntimeAudioConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const PlaybackParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeAudioConfig`



### `m_Stats`

- **Type:** `RuntimeAudioStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeAudioConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeAudioSystem&`

---

## Initialize



- **Parameters:** const RuntimeAudioConfig& config = {}

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

## CreateSource



- **Parameters:** const AudioSourceDesc& desc

- **Return:** `AudioSourceHandle`

---

## DestroySource



- **Parameters:** AudioSourceHandle handle

- **Return:** `void`

---

## PlaySound



- **Parameters:** AudioSourceHandle handle, const PlaybackParams& params = {}

- **Return:** `void`

---

## StopSound



- **Parameters:** AudioSourceHandle handle

- **Return:** `void`

---

## PauseSound



- **Parameters:** AudioSourceHandle handle

- **Return:** `void`

---

## ResumeSound



- **Parameters:** AudioSourceHandle handle

- **Return:** `void`

---

## SetSourcePosition



- **Parameters:** AudioSourceHandle handle, const Vector3& position

- **Return:** `void`

---

## SetSourceVelocity



- **Parameters:** AudioSourceHandle handle, const Vector3& velocity

- **Return:** `void`

---

## SetSourceVolume



- **Parameters:** AudioSourceHandle handle, float volume

- **Return:** `void`

---

## SetSourcePitch



- **Parameters:** AudioSourceHandle handle, float pitch

- **Return:** `void`

---

## CreateBus



- **Parameters:** const std::string& name, const AudioBusDesc& desc

- **Return:** `void`

---

## DestroyBus



- **Parameters:** const std::string& name

- **Return:** `void`

---

## SetBusVolume



- **Parameters:** const std::string& name, float volume

- **Return:** `void`

---

## AddReverbZone



- **Parameters:** const std::string& name, const ReverbZoneDesc& desc

- **Return:** `void`

---

## RemoveReverbZone



- **Parameters:** const std::string& name

- **Return:** `void`

---

## UpdateReverbZone



- **Parameters:** const std::string& name, const ReverbParams& params

- **Return:** `void`

---

## SetListenerTransform



- **Parameters:** const Transform& transform

- **Return:** `void`

---

## SetListenerVelocity



- **Parameters:** const Vector3& velocity

- **Return:** `void`

---

## RegisterEffectProcessor



- **Parameters:** const std::string& name, std::unique_ptr<IAudioProcessor> processor

- **Return:** `void`

---

## UnregisterEffectProcessor



- **Parameters:** const std::string& name

- **Return:** `void`

---
