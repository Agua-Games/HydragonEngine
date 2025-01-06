
## Classes

### NodeLayoutSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const NodeLayoutConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### ArrangeNodes



**Parameters:** const std::vector<NodeHandle>& nodes

---

### AlignNodes



**Parameters:** const std::vector<NodeHandle>& nodes, AlignmentMode mode

---

### DistributeNodes



**Parameters:** const std::vector<NodeHandle>& nodes, DistributionMode mode

---

### MinimizeGroup



**Parameters:** GroupHandle handle

---

### ExpandGroup



**Parameters:** GroupHandle handle

---

### SetLayoutStrategy



**Parameters:** std::unique_ptr<ILayoutStrategy> strategy

---

### ApplyLayout



**Parameters:** const std::string& layoutName

---
