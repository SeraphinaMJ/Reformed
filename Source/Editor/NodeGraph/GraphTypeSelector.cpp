/*!***************************************************************************************
\file       GraphTypeSelector.cpp
\author     Charles Boudousquie
\date       10/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Object that displays a menu to select desired graph type.
*****************************************************************************************/
#include "GraphTypeSelector.hpp"

#include "imgui.h"

#include <map>
#include <string>

namespace Editor
{
    static const std::map<const char*, GraphType> typeMap =
    {
      {"Script", GraphType::SCRIPT},
        {"Behavior Tree", GraphType::BEHAVIORTREE}
    };

    void Editor::GraphTypeSelector::DisplayMenu()
    {

        ImGui::SameLine();
        std::string menuName("Graph Type");

        std::string graphTypeName;
        for (auto graphType : typeMap)
        {
            if (graphType.second == currentType)
            {
                graphTypeName = graphType.first;
            }
        }

        menuName += " (Currently Set To " + graphTypeName + ")";
        if (ImGui::BeginMenu(menuName.c_str()))
        {
            for (auto graphType : typeMap)
            {
                if (ImGui::MenuItem(graphType.first))
                {
                    currentType = graphType.second;
                }
            }
            ImGui::EndMenu();
        }
    }

    GraphType GraphTypeSelector::GetSelectedType()
    {
        return currentType;
    }


};

