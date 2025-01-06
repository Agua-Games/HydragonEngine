# Classes
---

## RuntimeReplaySystem
---




# Variables
---

### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDeltaEncoding`

- **Type:** `bool`

- **Default Value:** `true`



### `enableKeyframeGeneration`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTimelineMarkers`

- **Type:** `bool`

- **Default Value:** `true`



### `keyframeInterval`

- **Type:** `uint32_t`

- **Default Value:** `300`



### `bufferSizeMB`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `recordingFidelity`

- **Type:** `float`

- **Default Value:** `1.0f`



### `replayDirectory`

- **Type:** `string`

- **Default Value:** `"Replays"`



### `config`

- **Type:** `const RuntimeReplayConfig&`

- **Default Value:** `{})`



### `data`

- **Type:** `const MarkerData&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeReplayConfig`



### `m_Stats`

- **Type:** `RuntimeReplayStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeReplayConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeReplaySystem&`

---

## Initialize



- **Parameters:** `const RuntimeReplayConfig& config = {}`

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

## StartRecording



- **Parameters:** `const ReplayDesc& desc`

- **Return:** `ReplayHandle`

---

## PauseRecording



- **Parameters:** `ReplayHandle handle`

- **Return:** `void`

---

## ResumeRecording



- **Parameters:** `ReplayHandle handle`

- **Return:** `void`

---

## StopRecording



- **Parameters:** `ReplayHandle handle`

- **Return:** `void`

---

## StartPlayback



- **Parameters:** `const std::string& replayFile`

- **Return:** `void`

---

## PausePlayback



- **Parameters:** ``

- **Return:** `void`

---

## ResumePlayback



- **Parameters:** ``

- **Return:** `void`

---

## StopPlayback



- **Parameters:** ``

- **Return:** `void`

---

## SetPlaybackSpeed



- **Parameters:** `float speed`

- **Return:** `void`

---

## SeekToTime



- **Parameters:** `float timeSeconds`

- **Return:** `void`

---

## SeekToMarker



- **Parameters:** `const std::string& markerName`

- **Return:** `void`

---

## AddReplayMarker



- **Parameters:** `const std::string& name`, `const MarkerData& data = {}`

- **Return:** `void`

---

## RegisterReplayHandler



- **Parameters:** `ReplayEventType type`, `const ReplayEventHandler& handler`

- **Return:** `void`

---

## UnregisterReplayHandler



- **Parameters:** `ReplayEventType type`

- **Return:** `void`

---

## ExportReplay



- **Parameters:** `ReplayHandle handle`, `const std::string& path`

- **Return:** `void`

---

## ImportReplay



- **Parameters:** `const std::string& path`

- **Return:** `ReplayHandle`

---
