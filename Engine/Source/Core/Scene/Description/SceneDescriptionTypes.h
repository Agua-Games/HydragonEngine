/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene description type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <variant>

namespace Hydragon::Scene {

using NodeHandle = uint64_t;

enum class NodeType {
    Transform,
    Geometry,
    Material,
    Light,
    Camera,
    Shader,
    Parameter,
    Group,
    Custom
};

enum class ParameterType {
    Bool,
    Int,
    Float,
    Vector2,
    Vector3,
    Vector4,
    Matrix,
    String,
    Asset,
    Array,
    Expression
};

struct Parameter {
    ParameterType type;
    std::variant<
        bool, int, float,
        Vector2, Vector3, Vector4,
        Matrix4x4, std::string,
        AssetReference, std::vector<Parameter>,
        Expression
    > value;
    bool isLazy = false;
};

struct NodeDesc {
    NodeType type;
    std::string name;
    std::vector<Parameter> parameters;
    bool isProcedural = false;
    bool isStatic = false;
};

struct ConnectionDesc {
    std::string fromPort;
    std::string toPort;
    bool isLazy = false;
};

struct DescriptionStats {
    uint32_t totalNodes;
    uint32_t totalConnections;
    uint32_t evaluatedNodes;
    uint32_t cachedResults;
    size_t memoryUsed;
    float evaluationTimeMs;
};

} // namespace Hydragon::Scene 