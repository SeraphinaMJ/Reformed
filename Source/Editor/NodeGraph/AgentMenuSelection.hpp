/*!***************************************************************************************
\file       AgentMenuSelection.hpp
\author     Charles Boudousquie
\date       10/18/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Helper object to display imgui menu for all agents in a scene.
*****************************************************************************************/
#pragma once
#include <memory>
#include <string>

class gameObject;
typedef std::weak_ptr<gameObject> GameObjectPtr;

namespace Editor
{
    
    class engineController;
    class AgentMenuSelection
    {
        bool selectionChanged;
        GameObjectPtr selectedActor = std::weak_ptr<gameObject>();
        engineController* controller;
    public:

        AgentMenuSelection(engineController*);

        // decide which agent will be displayed in the node graph editor
        void Display();

        bool selectionNotNull();

        // gets the name of the tree that the agent is linked to
        std::string getSelectionTreeName();

        // user picked a new actor
        bool userChangedSelection();

        // set changed status to false, continue to look for
        // newly selected agents
        void rewatch();

        // get whatever game object the user chose from the menu
        GameObjectPtr getSelectedObject();
    };
};

