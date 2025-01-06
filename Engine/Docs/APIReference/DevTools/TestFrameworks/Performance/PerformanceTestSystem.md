
## Classes

### PerformanceTestSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const PerformanceTestConfig& config = {}

### Shutdown



**Parameters:** 

### RegisterBenchmark



**Parameters:** const std::string& name, const BenchmarkCallback& callback

### RegisterBaseline



**Parameters:** const std::string& name, const BaselineMetrics& baseline

### RunAllBenchmarks



**Parameters:** 

### RunBenchmark



**Parameters:** const std::string& name

### SetThreshold



**Parameters:** const std::string& metric, float threshold

### ClearThresholds



**Parameters:** 

### GenerateReport



**Parameters:** const std::string& path

### ExportMetrics



**Parameters:** const std::string& path
