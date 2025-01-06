# Classes
---

## AudioBindings
---




# Variables
---

### `enableSpatialization`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEffects`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStreamedAudio`

- **Type:** `bool`

- **Default Value:** `true`



### `enableVisualization`

- **Type:** `bool`

- **Default Value:** `true`



### `maxSoundInstances`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxEffectInstances`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `streamingBufferSize`

- **Type:** `uint32_t`

- **Default Value:** `32768`



### `audioDataPath`

- **Type:** `string`

- **Default Value:** `"Data/Audio"`



### `config`

- **Type:** `const AudioBindingsConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const PlayParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AudioBindingsConfig`



### `m_Stats`

- **Type:** `AudioBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AudioBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static AudioBindings&`

---

## Initialize



- **Parameters:** const AudioBindingsConfig& config = {}

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



- **Parameters:** const std::string& name, const SoundDesc& desc

- **Return:** `SoundHandle`

---

## DestroySound



- **Parameters:** SoundHandle handle

- **Return:** `void`

---

## PreloadSound



- **Parameters:** const std::string& name

- **Return:** `void`

---

## PlaySound



- **Parameters:** const std::string& name, const PlayParams& params = {}

- **Return:** `SourceHandle`

---

## StopSound



- **Parameters:** SourceHandle handle

- **Return:** `void`

---

## PauseSound



- **Parameters:** SourceHandle handle

- **Return:** `void`

---

## ResumeSound



- **Parameters:** SourceHandle handle

- **Return:** `void`

---

## SetVolume



- **Parameters:** const std::string& name, float volume

- **Return:** `void`

---

## SetPitch



- **Parameters:** const std::string& name, float pitch

- **Return:** `void`

---

## SetPosition



- **Parameters:** SourceHandle handle, const Vector3& position

- **Return:** `void`

---

## CreateEffect



- **Parameters:** const std::string& name, const EffectDesc& desc

- **Return:** `EffectHandle`

---

## DestroyEffect



- **Parameters:** EffectHandle handle

- **Return:** `void`

---

## ApplyEffect



- **Parameters:** const std::string& soundName, const std::string& effectName

- **Return:** `void`

---

## SetListenerPosition



- **Parameters:** const Vector3& position

- **Return:** `void`

---

## SetListenerOrientation



- **Parameters:** const Vector3& forward, const Vector3& up

- **Return:** `void`

---

## EnableVisualization



- **Parameters:** bool enable

- **Return:** `void`

---

## GetSpectrum



- **Parameters:** std::vector<float>& spectrum

- **Return:** `void`

---

## GetWaveform



- **Parameters:** std::vector<float>& waveform

- **Return:** `void`

---
