/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcOrchestratorEditor.h
 * @brief The Procedural Orchestrator Editor is a specialized view for procedural pattern orchestration.
 * 
 * ARCHITECTURAL NOTES:
 * - This sub-editor is the UI hook to the engine's procedural pattern orchestration system ("Maestro", consolidated 
 *   as ProceduralOrchestrator, in the Procedural module). As any of the other sub-editors, its a consolidated, in a 
 *   single place, of otherwise scattered UI elements.
 * - The Procedural Orchestrator ("Maestro") has parameters to blend the amount of control/orchestration, so that
 *   the whole system doesn't feel obtrusive, limiting to the gameplay experience. 
 *   
 *   In the case of using the engine to generate simpler apps (e.g. a car visualizer/customizer), the procedural 
 *   pattern orchestration can be more aggressive. In this case, the app developer and end user use it as a some sort of 
 *   practical overarching lookdev/"feeldev" tool, to experiment with variations quickly - which go hands in hands 
 *   with usd's variations system.
 * 
 *   An important note is that the Procedural Orchestrator's main purpose is to enhance the experience (at the same
 *   time making it more practical to the developer to fine-tune overarching/pervasive aspects, like "mood"), not
 *   to limit nor enforce the user's emergent experience. It is meant to lead to creativity, emergent behaviors
 *   and harmony (as a maestro does with a symphony), coherence, not to work against these things.
 */
#pragma once
#include <imgui.h>

#include "hdImgui.h"

namespace hdImgui {

/**
 * @brief Renders the main procedural orchestrator window.
 * @param p_open Pointer to boolean controlling window visibility
 * @param windowData Editor window data containing global settings
 */
void ShowProcOrchestrator(bool* p_open, HdEditorWindowData* windowData);

} // namespace hdImgui