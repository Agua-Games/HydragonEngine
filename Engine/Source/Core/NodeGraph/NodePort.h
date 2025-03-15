/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <typeindex>
#include <any>
#include <functional>

namespace hd {

class NodePort {
public:
    enum class Direction {
        Input,
        Output
    };

    struct TypeInfo {
        std::type_index type;
        std::string name;
        std::function<bool(const std::any&)> validator;
    };

    NodePort(const std::string& name, Direction direction, const TypeInfo& typeInfo)
        : name(name)
        , direction(direction)
        , typeInfo(typeInfo) {}

    template<typename T>
    static TypeInfo CreateTypeInfo() {
        return TypeInfo{
            std::type_index(typeid(T)),
            typeid(T).name(),
            [](const std::any& value) -> bool {
                try {
                    std::any_cast<T>(value);
                    return true;
                } catch (const std::bad_any_cast&) {
                    return false;
                }
            }
        };
    }

    const std::string& GetName() const { return name; }
    Direction GetDirection() const { return direction; }
    const TypeInfo& GetTypeInfo() const { return typeInfo; }

    bool IsCompatibleWith(const NodePort& other) const {
        return typeInfo.type == other.typeInfo.type;
    }

    template<typename T>
    bool IsType() const {
        return typeInfo.type == std::type_index(typeid(T));
    }

    template<typename T>
    bool SetValue(const T& value) {
        if (!IsType<T>()) return false;
        this->value = value;
        return true;
    }

    template<typename T>
    bool GetValue(T& outValue) const {
        if (!IsType<T>()) return false;
        try {
            outValue = std::any_cast<T>(value);
            return true;
        } catch (const std::bad_any_cast&) {
            return false;
        }
    }

private:
    std::string name;
    Direction direction;
    TypeInfo typeInfo;
    std::any value;
};

} // namespace hd
```

And the corresponding changes to `Node.h`:

<augment_code_snippet path="Engine/Source/Core/NodeGraph/Node.h" mode="EDIT">
```cpp
// ... (previous includes)
#include "NodePort.h"

namespace hd {

// ... (NodeInfo remains the same)

class Node : public Object {
public:
    explicit Node(const NodeInfo& info) : Object(info), NodeInfo(info) {
        InitializePorts();
    }

    // ... (previous methods remain)

    // Enhanced port system
    template<typename T>
    bool AddInputPort(const std::string& name) {
        auto typeInfo = NodePort::CreateTypeInfo<T>();
        inputPorts.emplace(name, NodePort(name, NodePort::Direction::Input, typeInfo));
        return true;
    }

    template<typename T>
    bool AddOutputPort(const std::string& name) {
        auto typeInfo = NodePort::CreateTypeInfo<T>();
        outputPorts.emplace(name, NodePort(name, NodePort::Direction::Output, typeInfo));
        return true;
    }

    template<typename T>
    bool SetInputValue(const std::string& name, const T& value) {
        auto it = inputPorts.find(name);
        if (it == inputPorts.end()) return false;
        return it->second.SetValue<T>(value);
    }

    template<typename T>
    bool GetOutputValue(const std::string& name, T& outValue) const {
        auto it = outputPorts.find(name);
        if (it == outputPorts.end()) return false;
        return it->second.GetValue<T>(outValue);
    }

    const NodePort* GetInputPort(const std::string& name) const {
        auto it = inputPorts.find(name);
        return it != inputPorts.end() ? &it->second : nullptr;
    }

    const NodePort* GetOutputPort(const std::string& name) const {
        auto it = outputPorts.find(name);
        return it != outputPorts.end() ? &it->second : nullptr;
    }

protected:
    virtual void InitializePorts() = 0;

    std::unordered_map<std::string, NodePort> inputPorts;
    std::unordered_map<std::string, NodePort> outputPorts;

    // ... (rest of the protected members)
};

} // namespace hd