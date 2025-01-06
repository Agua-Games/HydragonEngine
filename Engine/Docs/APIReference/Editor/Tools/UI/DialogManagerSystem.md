
## Classes

### DialogManagerSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const DialogManagerConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### ShowDialog



**Parameters:** const std::string& type, const DialogDesc& desc

### CloseDialog



**Parameters:** DialogHandle handle

### CloseAllDialogs



**Parameters:** 

### ShowModal



**Parameters:** const std::string& type, const DialogDesc& desc

### CloseModal



**Parameters:** DialogHandle handle

### RegisterDialogType



**Parameters:** const std::string& type, std::unique_ptr<IDialog> dialog

### UnregisterDialogType



**Parameters:** const std::string& type
