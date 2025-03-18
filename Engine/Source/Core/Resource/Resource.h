/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Resource.h
 * @brief Resource is an abstract base class for raw resource data in Hydragon. Resource is a type of Object.
 * They should not be confused with ResourceNode, which is a node that can load and manage resources.
 * 
 * ARCHITECTURAL NOTES:
 * - It provides a common interface for all raw resources in the engine, like textures, models, shaders, audio, fonts, etc.
 * - Some resources may be loaded from disk, while others may be generated at runtime, procedurally or otherwise.
 * - Resources may be shared between multiple objects.
 * - Resources may be cached and asynchronously streamed to improve performance.
 * - The interface is designed around the core phases of a resource's lifecycle in hardware: initialization, update, and cleanup:
 * - Resource inherits from Object, which provides the core features of all objects in the system. See Object.h to understand the core architecture 
 * of Hydragon's data objects, and the Object interface.
 * 
 * TODO:
 * - Check with assistant: explain it and check if this is the right place for this class, how much it overlaps with Object and Node, how much of its 
 * functionality is actually needed considering the other core classes (Object, Node, etc.)
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 */
#pragma once
#include <memory>
#include <string>

#include "Object.h"

namespace hd {

/**
 * @brief Base class for all resources in the engine.
 */
class Resource : public Object {
public:
    // (...)
};

} // namespace hd