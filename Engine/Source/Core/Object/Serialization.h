/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Serialization.h
 * @brief Header file for the Versionedserializer class.
 * 
 * ARCHITECTURAL NOTES:
 * - Versionedserializer is a utility class for handling versioned serialization and migration of objects.
 * 
 * TODO:
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#pragma once
#include <functional>

namespace hd {

struct Version {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;

    bool operator<(const Version& other) const {
        if (major != other.major) return major < other.major;
        if (minor != other.minor) return minor < other.minor;
        return patch < other.patch;
    }
};

class Versionedserializer {
public:
    using MigrationFunc = std::function<void(Object&)>;

    static void registerMigration(Version from, Version to, MigrationFunc func) {
        migrations[{from, to}] = std::move(func);
    }

    static void migrate(Object& obj, Version from, Version to) {
        if (from == to) return;

        // Find migration path
        auto path = findMigrationPath(from, to);
        if (path.empty()) {
            throw std::runtime_error("No migration path found");
        }

        // Apply migrations in sequence
        for (size_t i = 0; i < path.size() - 1; ++i) {
            auto it = migrations.find({path[i], path[i + 1]});
            if (it != migrations.end()) {
                it->second(obj);
            }
        }
    }

private:
    struct VersionPair {
        Version from;
        Version to;

        bool operator==(const VersionPair& other) const {
            return from == other.from && to == other.to;
        }
    };

    struct VersionPairHash {
        size_t operator()(const VersionPair& p) const {
            return std::hash<uint64_t>()((uint64_t)p.from.major << 48 | 
                                       (uint64_t)p.from.minor << 32 |
                                       (uint64_t)p.from.patch << 16 |
                                       (uint64_t)p.to.major << 8 |
                                       (uint64_t)p.to.minor << 4 |
                                       (uint64_t)p.to.patch);
        }
    };

    static inline std::unordered_map<VersionPair, MigrationFunc, VersionPairHash> migrations;

    static std::vector<Version> findMigrationPath(Version from, Version to) {
        // Implement A* or Dijkstra's algorithm to find shortest migration path
        // This is a simplified placeholder
        return {from, to};
    }
};

// Example usage:
#define REGISTER_MIGRATION(FromMajor, FromMinor, FromPatch, \
                         ToMajor, ToMinor, ToPatch, \
                         MigrationFunction) \
    static bool registered_##FromMajor##_##FromMinor##_##FromPatch##_to_##ToMajor##_##ToMinor##_##ToPatch = \
        (Versionedserializer::registerMigration( \
            Version{FromMajor, FromMinor, FromPatch}, \
            Version{ToMajor, ToMinor, ToPatch}, \
            MigrationFunction), true)

} // namespace hd