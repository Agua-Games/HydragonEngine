
## Classes

### WindowManagerSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const WindowManagerConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateDockSpace



**Parameters:** const std::string& name, const DockSpaceDesc& desc

### DestroyDockSpace



**Parameters:** DockID id

### DockWindow



**Parameters:** const std::string& window, DockID dock, DockSlot slot

### UndockWindow



**Parameters:** const std::string& window

### GroupWindows



**Parameters:** const std::vector<std::string>& windows

### UngroupWindows



**Parameters:** const std::vector<std::string>& windows

### SetWindowVisibility



**Parameters:** const std::string& window, bool visible
