
## Classes

### WindowBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const WindowBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateWindow



**Parameters:** const WindowDesc& desc

### DestroyWindow



**Parameters:** WindowHandle handle

### ShowWindow



**Parameters:** WindowHandle handle

### HideWindow



**Parameters:** WindowHandle handle

### SetTitle



**Parameters:** WindowHandle handle, const std::string& title

### SetSize



**Parameters:** WindowHandle handle, uint32_t width, uint32_t height

### SetPosition



**Parameters:** WindowHandle handle, int32_t x, int32_t y

### SetFullscreen



**Parameters:** WindowHandle handle, bool fullscreen

### SetBorderless



**Parameters:** WindowHandle handle, bool borderless

### SetResizable



**Parameters:** WindowHandle handle, bool resizable

### SetMinSize



**Parameters:** WindowHandle handle, uint32_t width, uint32_t height

### SetMaxSize



**Parameters:** WindowHandle handle, uint32_t width, uint32_t height

### RegisterWindowCallback



**Parameters:** WindowHandle handle, const WindowCallback& callback

### UnregisterWindowCallback



**Parameters:** WindowHandle handle

### RegisterDropCallback



**Parameters:** WindowHandle handle, const DropCallback& callback

### UnregisterDropCallback



**Parameters:** WindowHandle handle

### ShowCursor



**Parameters:** WindowHandle handle, bool show

### SetCursorPosition



**Parameters:** WindowHandle handle, int32_t x, int32_t y

### SetCursorMode



**Parameters:** WindowHandle handle, CursorMode mode
