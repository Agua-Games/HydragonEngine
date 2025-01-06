
## Classes

### RuntimeReplaySystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeReplayConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### StartRecording



**Parameters:** const ReplayDesc& desc

### PauseRecording



**Parameters:** ReplayHandle handle

### ResumeRecording



**Parameters:** ReplayHandle handle

### StopRecording



**Parameters:** ReplayHandle handle

### StartPlayback



**Parameters:** const std::string& replayFile

### PausePlayback



**Parameters:** 

### ResumePlayback



**Parameters:** 

### StopPlayback



**Parameters:** 

### SetPlaybackSpeed



**Parameters:** float speed

### SeekToTime



**Parameters:** float timeSeconds

### SeekToMarker



**Parameters:** const std::string& markerName

### AddReplayMarker



**Parameters:** const std::string& name, const MarkerData& data = {}

### RegisterReplayHandler



**Parameters:** ReplayEventType type, const ReplayEventHandler& handler

### UnregisterReplayHandler



**Parameters:** ReplayEventType type

### ExportReplay



**Parameters:** ReplayHandle handle, const std::string& path

### ImportReplay



**Parameters:** const std::string& path
