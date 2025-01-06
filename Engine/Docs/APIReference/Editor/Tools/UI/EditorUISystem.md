
## Classes

### EditorUISystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const EditorUIConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### RegisterWindow



**Parameters:** const std::string& name, std::unique_ptr<IEditorWindow> window

### UnregisterWindow



**Parameters:** const std::string& name

### ShowWindow



**Parameters:** const std::string& name

### HideWindow



**Parameters:** const std::string& name

### FocusWindow



**Parameters:** const std::string& name

### SaveLayout



**Parameters:** const std::string& name

### LoadLayout



**Parameters:** const std::string& name

### ResetLayout



**Parameters:** 

### SetTheme



**Parameters:** const std::string& name

### CustomizeTheme



**Parameters:** const ThemeCustomization& customization
