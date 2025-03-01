/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PhysicsSystem.h
 * @brief Header file for the PhysicsSystem class.
 * 
 * ARCHITECTURAL NOTES:
 * - PhysicsSystem is a singleton class that manages the physics simulation.
 * - It is responsible for updating the physics simulation and resolving collisions.
 * - It's initially both a wrapper for third-party physics libraries and a custom physics engine, to be develop
 *   in parallel.
 * - It uses the Bullet Physics library, Taichi, and other libraries for the physics simulation.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */