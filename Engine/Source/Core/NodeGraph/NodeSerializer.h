/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeSerializer.h
 * @brief NodeSerializer is responsible for serializing and deserializing nodes in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - NodeSerializer handles the conversion of nodes to and from various formats.
 * 
 * TODO:
 * - This is a placeholder for the actual implementation, practically a design sketch. We must, before anything,
 * check how it relates to, integrates to the Object and Node design, and the whole Node Graph subsystem. Starting
 * with:
 *      - How will be the final design and implementation of the reflection and serialization subsystems.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */

#pragma once
#include "Node.h"

namespace hd {

class NodeSerializer {
public:
    static void toEditor(const Node& node, EditorContext& ctx) {
        // Automatically reflect properties to editor widgets
        ctx.addProperty("position", node.getPosition(), PropertyType::Vector3);
        ctx.addProperty("mass", node.getMass(), PropertyType::Float);
        ctx.addProperty("material", node.getMaterial(), PropertyType::Resource);
    }

    static std::string toCode(const Node& node) {
        return fmt::format(
            "create<{}>(\"{}\")\n"
            "    .position({})\n"
            "    .mass({})\n"
            "    .setMaterial(\"{}\")",
            node.getTypeName(),
            node.getName(),
            vectorToString(node.getPosition()),
            node.getMass(),
            node.getMaterial()
        );
    }
};

} // namespace hd