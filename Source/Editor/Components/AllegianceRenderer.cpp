///*!***************************************************************************************
//\file       AllegianceRenderer.cpp
//\author     Charles Boudousquie
//\date       11/5/19
//\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
//\par        Project: Boomerang
//\brief      Renders the allegiance component.
//*****************************************************************************************/
//#include "AllegianceRenderer.hpp"
//#include "Allegiance.hpp"
//#include "Factions.hpp"
//
//#include <map>
//
//#include <imgui.h>
//
//Editor::allegianceRenderer::allegianceRenderer(editorWindow * p_parent_window) 
//    : componentRenderer(p_parent_window)
//{
//}
//
//void Editor::allegianceRenderer::onRender(typeRT & p_type_data, objID p_editor_object_id)
//{
//
//    // create imgui combo
//    if (ImGui::BeginMenu("Factions"))
//    {
//        const auto& factions = GetFactionList();
//        for (auto faction : factions)
//        {
//            if (ImGui::MenuItem(faction.second))
//            {
//                p_type_data.member("faction").setInt((int)faction.first);
//            }
//        }
//
//        ImGui::EndMenu();
//    }
//
//}
