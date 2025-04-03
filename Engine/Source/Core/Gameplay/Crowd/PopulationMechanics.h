/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PopulationMechanics.h
 * @brief Header file for the PopulationMechanics class.
 * 
 * ARCHITECTURAL NOTES:
 * - PopulationMechanics is a class that represents population mechanics in Hydragon.
 * - It is used to represent any population mechanics in the game world, such as population management, population communication, population objectives, etc.
 * - It supports interactive features, such as population management, population communication, population objectives, etc. Also supports two-way messaging with other population 
 * mechanics, environment, character, etc.
 * 
 * - In terms of our current design approach and parameters to try to model the dynamics of societal species populations:
 *      - First of all, we try to harmonize the functionality with the other systems' designs already in place, like considering character trajectories from a procedural 
 *      computational point-of-view (character trajectories as vectors, use of destiny integration tensors, etc), taking inspiration from physics dynamics (contained systems, 
 *      fields, etc). Not because we believe societies like the human society are that simple to model, but for practical reasons focused on gameplay and drama dynamics. So,
 *      we try to simplify, reduce the societal species' dynamics to some simple essential elements - worth noting: in a humble, unpretentious way.
 *      - Then we try to "demistify" and reduce even further elements normally analyzed as complex ones, and from a complex point-of-view, for this treating the population
 *      individual entities in a phenomenological way, let's say - as one more among the many incredible phenomena from our known cosmos. For instance, we consider most of the
 *      human concern and endeavour as a dynamic centered on the accumulation of memory and cyclical "re-feeding" of this accumulated memory in an ever more complexifying and
 *      constructive cycle. As if the human collective was trying to build upon, "climb" each time more in its own ideation construct - putting it other way, its rate of 
 *      idea-centric, abstractions evolution and accumulation of memory, information, is immensely steeper than its rate of physical material evolution.
 *      This characteristic is a bit hard to model for a small dev studio as we are, but we're going to try, for this resorting to memory management and optimization strategies.
 */
#pragma once
#include "Core.h"
#include <string>
#include <vector>
#include "Node.h"
#include "DataTable.h"
#include "Population.h"
#include "ProceduralManager.h"
#include "Pattern.h"
#include "MemoryBank.h"

namespace hd {

/**
 * @class PopulationMechanics
 * @brief Class representing population mechanics in Hydragon.
 */
class PopulationMechanics : public Node {
public:
    // === Structure Definitions ===
    enum class GroupTypeByBonds {   // A group can be seen as a system, an enclosed one, with its own dynamics. At the same time as a subsystem to larger groups
        Family,
        Friends,
        Acquaintances,
        Enterprise,     // Though a bit overlapping with band, this means any group of individuals linked eminently by a common enterprise
        Band,           // Can be used for a music band, but we mean a band of individuals linked by a common goal or activity, in a less focused way than an enterprise
        Comraderie,
        Academy,
        Movement,       // Art movements, ideological movements, social causes movements, scientifical movements, religious, and so and so on.
        Association,
        Cooperative,
        Council,
        Institution,
        Clan,
        Tribe,
        EthnicGroup,
        People,         // In the sense of a set of tribes, lineages. Kind of the same as an EthnicGroup, but with possibly many ethnic strains, less autoctonous
        Nationals,      // Same as Countrymen
        Army,
        Squad,
        Team,
        Staff,
        Crew,
        Citizens,       // For city-states this is the topmost group, politically (literally, as 'polis' is the greek word for city, city-estate)
        Netizens,
        Villagers,
        Guild,          // We almost included Craftsmen, but we can use Guild for them
        Brotherhood,    // These include clandestine ones, secret societies, etc.
        Community,
        Other
    };

    // === Allocation, Initialization, Loading === 
    explicit PopulationMechanics(const PopulationMechanicsInfo& info = PopulationMechanicsInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    DataTable populationMechanicsData;
    std::vector<Population> populations;
    /** Values from zero to one. Low qualitative values mean that the population is not well-off, leaning more towards the antitheticals of the definitions, like unemployment, 
     * injustice, addiction (low health), homelessness, ignorance (lack of culture, civilization heritage, education, among others), sadness, poverty, etc.
     * A good deal of these qualitative variables are specific to species societies with human-average brain complexity and therefore more complex dynamics than others. 
     * In the same manner, it's worth noting that to model civilizations smarter than humans, more complex dynamics and more variables may be necessary to properly model them. 
     */
    float growth = 0.0f;
    float improvementRate = 0.0f;
    float development = 0.0f;
    float density = 0.0f;
    float diversity = 0.0f;         // Not as much a qualitative value as a quantitative one.
    float equality = 0.0f;
    float freedom = 0.0f;
    float security = 0.0f;
    float prosperity = 0.0f;
    float tolerance = 0.0f;
    float stability = 0.0f;
    float peace = 0.0f;
    float harmony = 0.0f;
    float justice = 0.0f;
    float ethics = 0.0f;    // Ethics is a deeper value than morality, with philosophical consciousness and concerns, righteousness (not necessarily noticeable), etc.
    float morality = 0.0f;  // Morality is a more surface-level, era-dependent, collective opinion dependent value. e.g. Hunt witches motivated by a era-related collective morality.
    float socialMobility = 0.0f;
    float meritocracy = 0.0f;
    float health = 0.0f;
    float happiness = 0.0f;
    float education = 0.0f;
    float materialDignity = 0.0f;
    float respectForIndividualRights = 0.0f;
    float respectForOthers = 0.0f;
    float employment = 0.0f;
    float income = 0.0f;
    float safety = 0.0f;
    float legality = 0.0f;
    float selfImprovement = 0.0f;	    // Self-improvement is a deeper value than education, with personal development, self-actualization, etc.
    float healthyEnvironment = 0.0f;
    float housing = 0.0f;
    float transportation = 0.0f;
    float infrastructure = 0.0f;
    float culture = 0.0f;
    float civilizationHeritage = 0.0f;
    float entertainment = 0.0f;
    float religion = 0.0f;          // Religion is different than spirituality in which it has more dogmas, more specified rules, is more social, focused on social bonds, values guidance, etc.
    float spirituality = 0.0f;      // Spirituality in its turn is more personal, intimate, related to self-consciousness, hope, faith in the human spirit, man-nature bond, man-cosmos bond.
    float politics = 0.0f;
    
    float activityRadius = 0.0f;

    // === Processing ===
    void processNode() override { }
    void addPopulation(const std::string& populationName);
    void removePopulation(const std::string& populationName);
    void randomizePopulation();
    void harmonizePopulation();
    void balancePopulationStats();
    // Unbalance population stats to simulate a population in crisis. May sound a bit ill-intentioned, but for drama, conflict dynamics, gameplay dynamics (intention tensor
    // towards resolution) story progression, etc. may be necessary.
    void unbalancePopulationStats();
    void processPopulationMechanics();
    void enableDynamicPopulationMechanics(bool enable);
    void enableDynamicSchedules(bool enable);           // Still sketch function. Needs longer consideration.
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~PopulationMechanics() = default;     // Default destructor
};

} // namespace hd

