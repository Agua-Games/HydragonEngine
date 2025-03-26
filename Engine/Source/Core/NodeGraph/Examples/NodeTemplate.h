/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief Template to check against or copy-paste from, for when creating new nodes.
 * Try to keep it up-to-date.
 */
#pragma once
#include "Node.h"

namespace hd {

/**
 * @brief ExampleNodeInfo struct.
 * Always document the main struct and approved methods, so that they appear in IDEs popups, auto-doc, etc.
 * Other than nodeType, inputs and outputs, the rest is optional (for it to work, I mean).
 * Also, always declare the input and output names with the exact same name as the member variables, including case.
 */
struct ExampleNodeInfo : public NodeInfo {
    ExampleNodeInfo() {
        name = "MyCustomNode";
        nodeType = "Example/ExampleNode";
        description = "Performs specific operation X";
        category = "Processing";  // e.g., Transform, Physics, Audio, etc.
        version = "1.0.0";
        metadata = {
            "serializable": true,
            "editable_in_editor": true,
            "procedural": false,
            "stateless": false,
            "cache_enabled": true
        };
        
        inputs = {
            "someValue",  // Description of InputA
            "someVector",  // Description of InputB
            // ... other inputs
        };
        
        outputs = {
            "result",  // Description of Result
            // ... other outputs
        };
    }
};

/**
 * @brief ExampleNode class.
 * Always document the main node-derived class and approved methods, so that they appear in IDEs popups, auto-doc, etc.
 * The category comments pattern (// === Category === //) repeats all over the codebase. It's based on code objects lifecyles in hardware, inspired by Vulkan's 
 * streamlined architecture and usage. Also this order of declarations inspired by Vulkan and hardware.
 * 
 * There are many more methods in the base Node class. I tried to include the ones you'll be using the most, and avoided including the ones that may break expected
 * behavior if you override them (like compilation, etc, which the engine will expect to be properly implemented). Check Node.h to see the full interface.
 * Most times you won't need to override most or all of them.
 */
class ExampleNode : public Node {
public:
    // === Structure Definitions ===
    // Define any enums, structs, or other types specific to this node
    enum class ExampleEnum {
        Value1,
        Value2
    };

    struct ExampleStruct {
        int value;
        std::string text;
    };

    // === Resource Management ===
    // Generally will go inside of private scope, but may appear here also
    bool initializeNodeResources() {
        // Initialize any node-specific resources
        return true;
    }

    // === Allocation, Initialization, Loading ===
    explicit ExampleNode(const ExampleNodeInfo& info = ExampleNodeInfo())
        : Node(info) {}
    
    // Initialize default values
    float someValue = 0.0f;
    std::string someText = "Default";
    Vector3 someVector = Vector3(0.0f, 0.0f, 0.0f);
    
    // Initialize and load the node
    initialize() override {}
    load() override {}

    // === Property Management ===
    // Define any properties specific to this node
    ExampleEnum exampleEnum = ExampleEnum::Value1;
    ExampleStruct exampleStruct = { 0, "Default" };

    // === Port Management ===
    
    // === [Node-specific] Management ===
    // Define any node-specific methods or properties

    // === Procedural Generation ===
    void setupProcedural() {
        // Set up procedural generation parameters
    }

    // === Caching & Optimization ===
    bool canCache() const override { return true; }

    // === Streaming ===
    void stream() override {
        // Stream data asynchronously
    }

    // === Processing ===
    void processNode() override {
        // 1. Get all inputs - getInputValue() uses hashing for faster lookups, so unfortunately we must assign the inputs manually
        someValue = getInputValue<float>("someValue");
        someVector = getInputValue<Vector3>("someVector");
        
        // 2. Process core logic
        auto result = computeResult(someValue, someVector);
        
        // 3. Set outputs
        // setOutputValue() uses hashing for faster lookups, so we must assign the outputs manually
        setOutputValue("result", result);
    }

    // === Execution ===
    void onResume() override {
        // Resume execution
    }

    // === Compilation ===

    // === Serialization ===

    // === Visualization ===

    // === Debug/Development ===

    // === Scripting ===

    // === AI Agent Integration ===

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ExampleNode() = default;

private:
    // Repeat the same pattern for all protected and private methods. Using only the category comments needed.

    // === Processing ===
    auto computeResult(float a, const Vector3& b) {
        // Core computation logic
    }
};

} // namespace hd