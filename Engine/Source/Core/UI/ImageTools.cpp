/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ImageTools.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowImageTools(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Image Tools", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::CollapsingHeader("Image")) {
            ImGui::Button("Import Image");
            ImGui::Button("New Image");
            ImGui::Button("Save Image");
            ImGui::Button("Load Image");
            ImGui::Button("Export Image");
            ImGui::Button("Share Image");
        }
        if (ImGui::CollapsingHeader("Layers")) {
            ImGui::Button("Add Image as Layer");
            ImGui::Button("Add Layer");
            ImGui::Button("Duplicate Layer");
            ImGui::Button("Rename Layer");
            ImGui::Button("Delete Layer");
            ImGui::Button("Layer Settings");
        }
        if (ImGui::CollapsingHeader("Channels")) {
            ImGui::Button("Add Image as Channel");
            ImGui::Button("Add Channel");
            ImGui::Button("Duplicate Channel");
            ImGui::Button("Rename Channel");
        }
        if (ImGui::CollapsingHeader("Procedurals")) {
            ImGui::Button("Generate Procedural");
            ImGui::Button("Add Image as Procedural");
            ImGui::Button("Add Procedural");
            ImGui::Button("Duplicate Procedural");
            ImGui::Button("Rename Procedural");
            ImGui::Button("Delete Procedural");
            ImGui::Button("Procedural Settings");
        }
        if (ImGui::CollapsingHeader("Packed Texture")) {
            ImGui::Button("Pack Layers as Packed Texture");
            ImGui::Button("Pack Layers as Texture Set Node");
        }
        if (ImGui::CollapsingHeader("Adjustments")) {
            ImGui::Button("Add Adjustment");
            ImGui::Button("Duplicate Adjustment");
            ImGui::Button("Rename Adjustment");
            ImGui::Button("Delete Adjustment");
        }
        if (ImGui::CollapsingHeader("Image Generation")) {
            ImGui::Button("Generate Image");
            ImGui::Button("Upscale Image");
            ImGui::Button("Send to Texture Set Node");
            ImGui::Button("Upscale to Packed Texture");
            ImGui::Button("Send to Layer");
            ImGui::Button("Send to Channel");
            ImGui::Button("Send to Procedural");
            ImGui::Button("Synthesis Texture Set Node");
            ImGui::Button("Synthesis Packed Texture");
        }
        if (ImGui::CollapsingHeader("Texture Settings")) {
            ImGui::Button("Maximum Size");
            ImGui::Button("Mipmaps");
            ImGui::Button("Generate Mipmaps");
            ImGui::Button("Filtering");
            ImGui::Button("Anisotropic Filtering");
        }
        if (ImGui::CollapsingHeader("Texture Compression")) {
            ImGui::Button("Lossless Compression");
            ImGui::Button("Lossy Compression");
        }
        if (ImGui::CollapsingHeader("Image Sequence & Atlas")) {
            ImGui::Button("Import Image Sequence");
            ImGui::Button("Convert Layers to Atlas");
            ImGui::Button("Add Image to Atlas");
            ImGui::Button("Remove Image from Atlas");
        }
    }
    ImGui::End();
}
} // namespace hdImgui