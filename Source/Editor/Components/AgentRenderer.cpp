/*!***************************************************************************************
\file       AgentRenderer.cpp
\author     Charles Boudousquie
\date       10/1/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the agent component.
*****************************************************************************************/
#include "AgentRenderer.hpp"
#include "../EngineController.hpp"
#include "../NodeGraph/NGE_Watchdog.hpp"

#include <EngineRunner.hpp>
#include <Engine.hpp>

#include <imgui.h>

Editor::agentRenderer::agentRenderer(editorWindow * p_parent_window)
    : componentRenderer(p_parent_window)
{
}

void Editor::agentRenderer::LoadTreeList()
{
    auto assetList = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->assetList();

    treeList.clear();

    // fill up tree list, by filtering out irrelevant files
    for (auto asset : assetList)
    {
        auto string = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->stringFromWide(asset);

        auto extensionPos = string.find(".bht");
        if (extensionPos != std::string::npos)
        {
            auto filename = std::filesystem::path(string).stem().string();
            treeList.push_back(filename);
        }
    }
}

void Editor::agentRenderer::onRender(typeRT & p_type_data, objID id)
{

    // create imgui combo
    if (ImGui::BeginMenu("Trees"))
    {
        // load trees from disk
        LoadTreeList(); // should probably make this more efficient TODO

        for (auto tree : treeList)
        {
            // user chooses what tree they want
            if (ImGui::MenuItem(tree.c_str()))
            {
                // mark object that changed to Node Graph Window knows about it
                NGE_Watchdog::GetInstance().Alert(id);
                // set tree name for type rt
                p_type_data.member("parentTree").setString(tree);
            }
        }

        ImGui::EndMenu();
    }

}
