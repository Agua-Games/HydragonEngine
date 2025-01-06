
## Classes

### RuntimeCinematicSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RuntimeCinematicConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateCinematic



**Parameters:** const CinematicDesc& desc

### DestroyCinematic



**Parameters:** CinematicHandle handle

### PlayCinematic



**Parameters:** CinematicHandle handle, const PlaybackParams& params = {}

### StopCinematic



**Parameters:** CinematicHandle handle

### PauseCinematic



**Parameters:** CinematicHandle handle

### ResumeCinematic



**Parameters:** CinematicHandle handle

### SetCinematicTime



**Parameters:** CinematicHandle handle, float time

### SetCinematicSpeed



**Parameters:** CinematicHandle handle, float speed

### SetCinematicWeight



**Parameters:** CinematicHandle handle, float weight

### AddTrack



**Parameters:** CinematicHandle handle, const TrackDesc& desc

### RemoveTrack



**Parameters:** CinematicHandle handle, TrackHandle track

### SetTrackEnabled



**Parameters:** CinematicHandle handle, TrackHandle track, bool enabled

### SetCameraTarget



**Parameters:** CinematicHandle handle, EntityHandle target

### SetCameraBlendMode



**Parameters:** CinematicHandle handle, CameraBlendMode mode

### RegisterEventHandler



**Parameters:** const std::string& event, const CinematicEventHandler& handler

### UnregisterEventHandler



**Parameters:** const std::string& event

### SaveCinematicState



**Parameters:** CinematicHandle handle, const std::string& name

### LoadCinematicState



**Parameters:** CinematicHandle handle, const std::string& name
