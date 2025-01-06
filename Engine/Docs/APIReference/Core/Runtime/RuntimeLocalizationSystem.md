
## Classes

### RuntimeLocalizationSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeLocalizationConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### LoadLocale



**Parameters:** const std::string& locale, bool setActive = false

---

### UnloadLocale



**Parameters:** const std::string& locale

---

### SetActiveLocale



**Parameters:** const std::string& locale

---

### GetText



**Parameters:** const std::string& key, const LocalizationParams& params = {}

---

### GetPlural



**Parameters:** const std::string& key, int count, const LocalizationParams& params = {}

---

### GetContextual



**Parameters:** const std::string& key, const std::string& context, const LocalizationParams& params = {}

---

### RegisterFormatter



**Parameters:** const std::string& name, std::unique_ptr<ILocalizationFormatter> formatter

---

### UnregisterFormatter



**Parameters:** const std::string& name

---
