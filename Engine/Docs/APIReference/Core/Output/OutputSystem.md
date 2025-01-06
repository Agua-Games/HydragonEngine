
## Classes

### OutputSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const OutputConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### Log



**Parameters:** const LogMessage& message

---

### LogFormat



**Parameters:** LogLevel level, const char* format, ...

---

### LogError



**Parameters:** const char* message, const char* file = nullptr, int line = 0

---

### AddOutputChannel



**Parameters:** std::unique_ptr<IOutputChannel> channel

---

### RemoveOutputChannel



**Parameters:** OutputChannelId id

---

### SetLogLevel



**Parameters:** LogLevel level

---

### SetLogFilter



**Parameters:** const LogFilter& filter

---

### Flush



**Parameters:** 

---
