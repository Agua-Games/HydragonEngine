
## Classes

### ConsoleSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ConsoleConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### RegisterCommand



**Parameters:** const std::string& name, const CommandCallback& callback,
                        const std::string& description = ""

---

### UnregisterCommand



**Parameters:** const std::string& name

---

### ExecuteCommand



**Parameters:** const std::string& command

---

### ExecuteScript



**Parameters:** const std::string& path

---

### AddOutput



**Parameters:** const std::string& text, OutputLevel level = OutputLevel::Info

---

### ClearOutput



**Parameters:** 

---
