# Classes
---

## RuntimeCinematicSystem
---




# Variables
---

### `enableBlending`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimeWarping`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEventTracks`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCameraOverrides`

- **Type:** `bool`

- **Default Value:** `true`



### `maxActiveCinematics`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxTrackCount`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `defaultBlendTime`

- **Type:** `float`

- **Default Value:** `0.5f`



### `defaultEaseInTime`

- **Type:** `float`

- **Default Value:** `0.3f`



### `cinematicCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Cinematics"`



### `config`

- **Type:** `const RuntimeCinematicConfig&`

- **Default Value:** `{})`



### `params`

- **Type:** `const PlaybackParams&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeCinematicConfig`



### `m_Stats`

- **Type:** `RuntimeCinematicStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeCinematicConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RuntimeCinematicSystem&`

---

## Initialize



- **Parameters:** const RuntimeCinematicConfig& config = {}

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

## CreateCinematic



- **Parameters:** const CinematicDesc& desc

- **Return:** `CinematicHandle`

---

## DestroyCinematic



- **Parameters:** CinematicHandle handle

- **Return:** `void`

---

## PlayCinematic



- **Parameters:** CinematicHandle handle, const PlaybackParams& params = {}

- **Return:** `void`

---

## StopCinematic



- **Parameters:** CinematicHandle handle

- **Return:** `void`

---

## PauseCinematic



- **Parameters:** CinematicHandle handle

- **Return:** `void`

---

## ResumeCinematic



- **Parameters:** CinematicHandle handle

- **Return:** `void`

---

## SetCinematicTime



- **Parameters:** CinematicHandle handle, float time

- **Return:** `void`

---

## SetCinematicSpeed



- **Parameters:** CinematicHandle handle, float speed

- **Return:** `void`

---

## SetCinematicWeight



- **Parameters:** CinematicHandle handle, float weight

- **Return:** `void`

---

## AddTrack



- **Parameters:** CinematicHandle handle, const TrackDesc& desc

- **Return:** `void`

---

## RemoveTrack



- **Parameters:** CinematicHandle handle, TrackHandle track

- **Return:** `void`

---

## SetTrackEnabled



- **Parameters:** CinematicHandle handle, TrackHandle track, bool enabled

- **Return:** `void`

---

## SetCameraTarget



- **Parameters:** CinematicHandle handle, EntityHandle target

- **Return:** `void`

---

## SetCameraBlendMode



- **Parameters:** CinematicHandle handle, CameraBlendMode mode

- **Return:** `void`

---

## RegisterEventHandler



- **Parameters:** const std::string& event, const CinematicEventHandler& handler

- **Return:** `void`

---

## UnregisterEventHandler



- **Parameters:** const std::string& event

- **Return:** `void`

---

## SaveCinematicState



- **Parameters:** CinematicHandle handle, const std::string& name

- **Return:** `void`

---

## LoadCinematicState



- **Parameters:** CinematicHandle handle, const std::string& name

- **Return:** `void`

---
