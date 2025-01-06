
## Classes

### BuildSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const BuildConfig& config = {}

### Shutdown



**Parameters:** 

### CreateBuild



**Parameters:** const BuildDesc& desc

### DestroyBuild



**Parameters:** BuildHandle handle

### AddTarget



**Parameters:** BuildHandle build, const BuildTarget& target

### RemoveTarget



**Parameters:** BuildHandle build, const std::string& targetName

### SetBuildEnvironment



**Parameters:** BuildHandle build, const BuildEnvironment& env

### AddDependency



**Parameters:** BuildHandle build, const std::string& target, 
                      const std::string& dependency

### BuildTarget



**Parameters:** BuildHandle build, const std::string& targetName

### BuildAll



**Parameters:** BuildHandle build

### CancelBuild



**Parameters:** BuildHandle build
