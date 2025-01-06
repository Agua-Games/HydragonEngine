
## Classes

### ScriptCompletionSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const ScriptCompletionConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### RequestCompletion



**Parameters:** const std::string& path, uint32_t line, uint32_t column

---

### CancelCompletion



**Parameters:** 

---

### AddSnippet



**Parameters:** const std::string& trigger, const ScriptSnippet& snippet

---

### RemoveSnippet



**Parameters:** const std::string& trigger

---

### RegisterSymbol



**Parameters:** const std::string& name, const SymbolInfo& info

---

### UnregisterSymbol



**Parameters:** const std::string& name

---
