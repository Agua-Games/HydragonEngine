# Classes
---

## MemoryVisualization
---




# Functions
---

## GenerateHTMLReport



- **Parameters:** `const std::vector<MemoryBenchmark::BenchmarkResult>& results`

- **Return:** `string`

---

## GenerateConsoleReport



- **Parameters:** `const MemoryBenchmark::BenchmarkResult& result`

- **Return:** `string`

---

## GenerateMemoryMap



- **Parameters:** `const MemoryBenchmark::BenchmarkResult& result`

- **Return:** `string`

---

## ExportToJSON



- **Parameters:** `const std::vector<MemoryBenchmark::BenchmarkResult>& results`, `const std::string& filepath`

- **Return:** `static bool`

---

## GenerateAccessPatternChart



- **Parameters:** `const BenchmarkResult::AccessPatternMetrics& metrics`

- **Return:** `string`

---

## GenerateFragmentationVisual



- **Parameters:** `const BenchmarkResult::FragmentationMetrics& metrics`

- **Return:** `string`

---

## GenerateHotspotHeatmap



- **Parameters:** `const std::vector<BenchmarkResult::AccessPatternMetrics::HotSpot>& hotspots`

- **Return:** `string`

---
