/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Flexible attribute system for data management
 */

#pragma once
#include "Core/Memory/Management/Strategies/DataMemoryStrategy.h"

namespace Hydragon {
namespace Data {

class AttributeSystem {
public:
    // Flexible attribute container inspired by Houdini
    template<typename T>
    class Attribute {
    public:
        // Support any data type, including custom structures
        void createAttribute(const char* name, AttributeOwner owner = AttributeOwner::Point) {
            m_Storage.allocate<T>(name, owner);
        }

        // Python-friendly attribute access
        template<typename ValueType>
        void setAttributeValue(const char* name, size_t index, ValueType value) {
            m_Storage.setValue(name, index, value);
        }
    };

    // Support for groups (like Houdini's point/prim groups)
    class GroupSystem {
    public:
        // Create dynamic groups
        GroupHandle createGroup(const char* name, GroupType type) {
            return m_GroupManager.create(name, type);
        }
    };
};

}} // namespace Hydragon::Data 