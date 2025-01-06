
## Classes

### IntegrationTestSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const IntegrationTestConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### RegisterScenario



**Parameters:** const std::string& name, const ScenarioCallback& callback

---

### RegisterEnvironment



**Parameters:** const std::string& name, const EnvironmentSetup& setup

---

### RunAllScenarios



**Parameters:** 

---

### RunScenario



**Parameters:** const std::string& name

---

### SetupTestEnvironment



**Parameters:** const std::string& name

---

### TeardownTestEnvironment



**Parameters:** 

---

### CaptureState



**Parameters:** const std::string& checkpoint

---

### RestoreState



**Parameters:** const std::string& checkpoint

---

### GenerateReport



**Parameters:** const std::string& path

---
