/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EmotionMechanics.h
 * @brief Header file for the EmotionMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - EmotionMechanics is a class that represents emotion mechanics in Hydragon.
 * - It is used to represent any emotion mechanics in the game world, such as mood, stress, etc.
 * - It supports interactive features, such as mood, stress, and socialization. Also supports two-way messaging with other emotion mechanics, environment, character, etc.
 * 
 * @todo Design the mechanics of character memories (both between characters and between characters and environment), relationships (same: both between characters and 
 * between characters and environment), socialization. Almost certainly use MemoryBank, DataTable, mappings, maybe some dedicated Tensors to calculate these, reduce
 * vectors to scalars, etc.
 */
#pragma once
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace hd {
struct EmotionMechanicsInfo : public NodeInfo {
    EmotionMechanicsInfo() {
        nodeType = "Gameplay/EmotionMechanics";
        
        inputs = {
            "emotionData",        // Emotion data
            "environment",        // Environment data
            "characterData",      // Character data
            "emotionState"        // Emotion state
        };
        
        outputs = {
            "emotionStatus",      // Emotion status
            "emotionMetrics"      // Emotion performance metrics
        };
    }
};

/**
 * @class EmotionMechanics
 * @brief Class representing emotion mechanics in Hydragon.
 */
class EmotionMechanics : public Node {
public:
    enum class Emotion {
        Happy,
        Sad,
        Angry,
        Fearful,
        Surprised,
        Disgusted,
        Neutral
    };

    struct EmotionData {
        Emotion emotion;
        float intensity;
    };

    struct EmotionLink {
        EmotionMechanics* target;
        float intensity;
        bool syncLevel;
    };

    // === Allocation, Initialization, Loading ===
    explicit EmotionMechanics(const EmotionMechanicsInfo& info = EmotionMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable emotionMechanicsData;
    Emotion emotion = Emotion::Neutral;
    float stress = 0.0f;
    float socialization = 0.0f;
    float happiness = 0.0f;
    float mood = 0.0f;
    float anger = 0.0f;
    float fear = 0.0f;
    float surprise = 0.0f;
    float disgust = 0.0f;
    std::vector<EmotionLink> links;         // Links to other emotion mechanics

    // === Processing ===
    void processNode() override { }
    void addEmotion(const std::string& emotionName);
    void removeEmotion(const std::string& emotionName);
    void processEmotion();
    void link(const std::string& targetName);  // Link to another emotion mechanics node by name
    void unlink(const std::string& targetName);  // Unlink from another emotion mechanics node by name
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EmotionMechanics() = default;     // Default destructor
};

} // namespace hd