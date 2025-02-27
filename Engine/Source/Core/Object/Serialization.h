/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
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

class VersionedSerializer {
public:
    using MigrationFunc = std::function<void(HD_Object&)>;

    static void RegisterMigration(Version from, Version to, MigrationFunc func) {
        migrations[{from, to}] = std::move(func);
    }

    static void Migrate(HD_Object& obj, Version from, Version to) {
        if (from == to) return;

        // Find migration path
        auto path = FindMigrationPath(from, to);
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

    static std::vector<Version> FindMigrationPath(Version from, Version to) {
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
        (VersionedSerializer::RegisterMigration( \
            Version{FromMajor, FromMinor, FromPatch}, \
            Version{ToMajor, ToMinor, ToPatch}, \
            MigrationFunction), true)

} // namespace hd