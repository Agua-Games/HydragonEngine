
## Classes

### ConfigValidationBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const ConfigValidationBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateRule



**Parameters:** const std::string& name, const RuleDesc& desc = {}

### LoadRules



**Parameters:** const std::string& path

### SaveRules



**Parameters:** const std::string& path

### DeleteRule



**Parameters:** RuleHandle handle

### SetRuleCondition



**Parameters:** RuleHandle handle, const RuleCondition& condition

### SetRuleAction



**Parameters:** RuleHandle handle, const RuleAction& action

### SetRuleSeverity



**Parameters:** RuleHandle handle, RuleSeverity severity

### SetRuleEnabled



**Parameters:** RuleHandle handle, bool enabled

### RegisterCustomRule



**Parameters:** const std::string& name, std::unique_ptr<ICustomRule> rule

### UnregisterCustomRule



**Parameters:** const std::string& name

### ValidateConfig



**Parameters:** ConfigHandle config

### ValidateSection



**Parameters:** ConfigHandle config, const std::string& section

### ValidateValue



**Parameters:** const Variant& value, const ValidationContext& context

### ClearValidationErrors



**Parameters:** ValidationHandle handle

### EnableValidationTracking



**Parameters:** bool enable

### DumpValidationState



**Parameters:** const std::string& path

### AnalyzeRulePerformance



**Parameters:** 
