
## Classes

### StressTestSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const StressTestConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### RegisterScenario



**Parameters:** const std::string& name, const LoadScenario& scenario

---

### RegisterMetric



**Parameters:** const std::string& name, const MetricCallback& callback

---

### RunLoadTest



**Parameters:** const std::string& scenario

---

### StopLoadTest



**Parameters:** 

---

### SetLoadProfile



**Parameters:** const LoadProfile& profile

---

### SetResourceLimits



**Parameters:** const ResourceLimits& limits

---

### GenerateReport



**Parameters:** const std::string& path

---

### ExportMetrics



**Parameters:** const std::string& path

---
