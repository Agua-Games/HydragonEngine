# Classes
---

## UnitTestSystem
---




# Variables
---

### `enableParallelExecution`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCoverage`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBenchmarking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMocking`

- **Type:** `bool`

- **Default Value:** `true`



### `maxParallelTests`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `testTimeout`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `outputDirectory`

- **Type:** `string`

- **Default Value:** `"TestResults"`



### `config`

- **Type:** `const UnitTestConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `UnitTestConfig`



### `m_Stats`

- **Type:** `TestStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `UnitTestConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static UnitTestSystem&`

---

## Initialize



- **Parameters:** `const UnitTestConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## RegisterTest



- **Parameters:** `const std::string& suite`, `const std::string& name`, `const TestCallback& callback`

- **Return:** `void`

---

## RegisterFixture



- **Parameters:** `const std::string& suite`, `const TestFixture& fixture`

- **Return:** `void`

---

## RunAll



- **Parameters:** ``

- **Return:** `void`

---

## RunSuite



- **Parameters:** `const std::string& suite`

- **Return:** `void`

---

## RunTest



- **Parameters:** `const std::string& suite`, `const std::string& name`

- **Return:** `void`

---

## SetupMock



- **Parameters:** `const std::string& name`, `const MockCallback& callback`

- **Return:** `void`

---

## ClearMocks



- **Parameters:** ``

- **Return:** `void`

---

## GenerateReport



- **Parameters:** `const std::string& path`

- **Return:** `void`

---
