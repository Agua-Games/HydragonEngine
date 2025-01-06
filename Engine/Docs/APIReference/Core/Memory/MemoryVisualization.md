
## Classes

### MemoryVisualization




## Functions

### GenerateHTMLReport



**Parameters:** const std::vector<MemoryBenchmark::BenchmarkResult>& results

---

### GenerateConsoleReport



**Parameters:** const MemoryBenchmark::BenchmarkResult& result

---

### GenerateMemoryMap



**Parameters:** const MemoryBenchmark::BenchmarkResult& result

---

### ExportToJSON



**Parameters:** const std::vector<MemoryBenchmark::BenchmarkResult>& results, 
                           const std::string& filepath

---

### GenerateAccessPatternChart



**Parameters:** const BenchmarkResult::AccessPatternMetrics& metrics

---

### GenerateFragmentationVisual



**Parameters:** const BenchmarkResult::FragmentationMetrics& metrics

---

### GenerateHotspotHeatmap



**Parameters:** const std::vector<BenchmarkResult::AccessPatternMetrics::HotSpot>& hotspots

---
