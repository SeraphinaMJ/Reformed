/*!***************************************************************************************
\file       AgentMenuSelection.cpp
\author     Charles Boudousquie
\date       10/18/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Helper object to display imgui menu for all agents in a scene.
*****************************************************************************************/
#include "AgentMenuSelection.hpp"
#include "../EngineController.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "BehaviorTreeManipulatorInterface.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "Agent.hpp"

#include "IComponentRepository.hpp"
#include "BehaviorTreeManipulator.hpp"

#include "imgui.h"

Editor::AgentMenuSelection::AgentMenuSelection(engineController *ec) : selectionChanged(false), controller(ec)
{
}

void Editor::AgentMenuSelection::Display()
{
    GameObjectPtr previous = this->selectedActor;

    ImGui::Text("List of agents with behavior trees: ");

    std::string menu_text = "Agents (Current Agent ID: ";
    if (!this->selectedActor.expired())
    {
        menu_text += std::to_string(this->selectedActor.lock()->getID());
    }

    menu_text += ")";

    if (ImGui::BeginMenu(menu_text.c_str()))
    {
        auto behaviorManipulator = controller->getEngineRunner()->getEngine()->getBehaviorManipulator().lock();
        auto allAgentObjects = behaviorManipulator->getObjectsWithAgents();

        // for every single object with an agent component
        for (auto agentObject : allAgentObjects)
        {
            auto name = agentObject->getName();

            // if a particular agent is chosen
            if (ImGui::MenuItem(name.c_str()))
            {
                // set current agent chosen as agentObject
                this->selectedActor = agentObject;

                // if this actor was not what was previously selected
                if(!this->selectedActor.expired() && !previous.expired())
                {
                    if (this->selectedActor.lock() != previous.lock())
                    {
                        selectionChanged = true; // then we have changed selection
                    }

                    previous = agentObject; // update previous
                }
                
            }
        }
        ImGui::EndMenu();
    }
}

bool Editor::AgentMenuSelection::selectionNotNull()
{
    return selectedActor.expired();
}

std::string Editor::AgentMenuSelection::getSelectionTreeName()
{
    if(!selectedActor.expired())
    {
        return selectedActor.lock()->getComponent<Agent>()->getTreeName();
    }

    return std::string();
}

bool Editor::AgentMenuSelection::userChangedSelection()
{
    return selectionChanged;
}

void Editor::AgentMenuSelection::rewatch()
{
    selectionChanged = false;
}

GameObjectPtr Editor::AgentMenuSelection::getSelectedObject()
{
    return selectedActor;
}
