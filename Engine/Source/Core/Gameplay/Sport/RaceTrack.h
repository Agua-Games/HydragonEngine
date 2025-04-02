/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RaceTrack.h
 * @brief Header file for the RaceTrack class.
 * 
 * ARCHITECTURAL NOTES:
 * - RaceTrack is a class that represents a race track in Hydragon.
 * - It is used to represent any race track in the game world, such as race tracks, race courses, etc.
 * - It supports splines for placement, with terrain integration, procedural erosion, etc.
 * - It supports procedural features, such as race track generation, race track growth, and race track "death" (cracks, debris, crumbling). 
 * Also supports two-way messaging with other race tracks, environment, character, etc.
 * - It supports interactive features, such as racing, speed, and acceleration. 
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include "PhysicsMaterial.h"

namespace hd {

enum class RaceTrackType {
    RaceTrack,
    RaceCourse,
    Other
};

struct RaceTrackInfo : public NodeInfo {
    RaceTrackInfo() {
        nodeType = "Gameplay/RaceTrack";
        
        inputs = {
            "raceTrackType",  // Type of race track (race track, race course, etc.)
            "raceTrackData",  // Race track data
            "environment",    // Environment data
            "characterData",  // Character data
            "raceTrackState"  // Race track state
        };
        
        outputs = {
            "raceTrackStatus", // Race track status
            "raceTrackMetrics" // Race track performance metrics
        };
    }
};

class RaceTrack : public Node {
public:
    // === Structure Definitions ===
    struct RaceTrackCheckpoint {
        vec3 position;
        float radius;
    };

    // === Allocation, Initialization, Loading ===
    explicit RaceTrack(const RaceTrackInfo& info = RaceTrackInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    RaceTrackType type = RaceTrackType::Other;
    DataTable raceTrackData;
    DataTable raceTrackState;
    int laps = 0;
    std::vector<RaceTrackCheckpoint> checkpoints; // Checkpoints for lap completion
    bool ghostCar = false;
    std::vector<PhysicsMaterial> surfaces;       // Temporary: the physics materials should be automatically set from tags in the mesh materials

    // === Processing ===
    void processNode() override { }
    void addSurface(const std::string& surfaceName);
    void removeSurface(const std::string& surfaceName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~RaceTrack() = default;     // Default destructor
};

} // namespace hd
