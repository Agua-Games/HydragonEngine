
## Classes

### RuleSeverity



### ValidationRule



### ValidationContext




## Functions

### Create



**Parameters:** const std::string& name, const RuleDesc& desc = {}

### SetCondition



**Parameters:** const RuleCondition& condition

### SetAction



**Parameters:** const RuleAction& action

### SetSeverity



**Parameters:** RuleSeverity severity

### SetEnabled



**Parameters:** bool enabled

### InheritFrom



**Parameters:** RuleHandle parent

### RemoveInheritance



**Parameters:** 

### SetValue



**Parameters:** const Variant& value

### AddError



**Parameters:** const ValidationError& error

### AddWarning



**Parameters:** const ValidationError& warning
