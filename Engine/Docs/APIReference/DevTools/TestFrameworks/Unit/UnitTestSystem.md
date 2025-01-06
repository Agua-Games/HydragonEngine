
## Classes

### UnitTestSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const UnitTestConfig& config = {}

### Shutdown



**Parameters:** 

### RegisterTest



**Parameters:** const std::string& suite, const std::string& name, 
                     const TestCallback& callback

### RegisterFixture



**Parameters:** const std::string& suite, const TestFixture& fixture

### RunAll



**Parameters:** 

### RunSuite



**Parameters:** const std::string& suite

### RunTest



**Parameters:** const std::string& suite, const std::string& name

### SetupMock



**Parameters:** const std::string& name, const MockCallback& callback

### ClearMocks



**Parameters:** 

### GenerateReport



**Parameters:** const std::string& path
