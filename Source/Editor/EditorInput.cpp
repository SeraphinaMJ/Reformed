/*!***************************************************************************************
\file       EditorInput.cpp
\author     Charles Boudousquie
\date       9/9/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation for the editor's delegate object that handles special user input.
*****************************************************************************************/

//======== Self Include ================================================================//
#include "EditorInput.hpp"
//======== 1st Party Includes ==========================================================//
#include "UndoRedo.hpp"
#include "EngineController.hpp"
//======== 3rd Party Includes ==========================================================//
#include "imgui.h"

Editor::EditorInput::EditorInput(engineController * eController) : controller(eController)
{}

void Editor::EditorInput::Update()
{
    // check if user pressed ctrl
    auto & io = ImGui::GetIO();

    //if ctrl pressed
    if (io.KeyCtrl == true)
    {
        // check for ctrl z
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Z)))
        {
            UndoRedoManager::GetInstance().Undo(controller);
        }
        else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Y)))
        {
            UndoRedoManager::GetInstance().Redo(controller);
        }
    }
}
