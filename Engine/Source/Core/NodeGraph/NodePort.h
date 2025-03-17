/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * NodePort represents a port on a node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - NodePort handles the connection and data flow between nodes.
 * 
 * TODO:
 * - Maybe get rid of the class and this file completely, and use the Property class instead. At maximum, declare a
 * Port struct in the Node class.
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
    static TypeInfo createTypeInfo() {
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

    const std::string& getName() const { return name; }
    Direction getDirection() const { return direction; }
    const TypeInfo& getTypeInfo() const { return typeInfo; }

    bool isCompatibleWith(const NodePort& other) const {
        return typeInfo.type == other.typeInfo.type;
    }

    template<typename T>
    bool isType() const {
        return typeInfo.type == std::type_index(typeid(T));
    }

    template<typename T>
    bool setValue(const T& value) {
        if (!IsType<T>()) return false;
        this->value = value;
        return true;
    }

    template<typename T>
    bool getValue(T& outValue) const {
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
