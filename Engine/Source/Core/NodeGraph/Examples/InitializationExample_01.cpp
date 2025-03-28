/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 *  - Initialization of nodes and connections
 */
#pragma once
#include "Node.h"
#include "Drone.h"
#include "NavigationManager.h"
#include "AerialManager.h"

class GameSystem {
    Drone* drone;
    NavigationManager* nav;
    AerialManager* flight;

    void init() {
        // Direct references, no lookups
        drone = new Drone();
        nav = new NavigationManager();
        flight = new AerialManager();

        // Direct connections
        drone->connect(nav, "position")
             ->connect(flight, "altitude");

        // Or create-and-connect style
        drone->connect<NavigationManager>("position")
             ->connect<AerialManager>("altitude");

        // Direct member variable connections
        flight->altitude = &drone->height; // Simplified example
    }
};