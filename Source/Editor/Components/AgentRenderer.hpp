/*!***************************************************************************************
\file       AgentRenderer.hpp
\author     Charles Boudousquie
\date       10/1/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the agent component.
*****************************************************************************************/
#pragma once
#include "ComponentRenderer.hpp"
#include <vector>
#include <string>

namespace Editor
{
    class agentRenderer : public componentRenderer
    {
    public:
        agentRenderer(editorWindow * p_parent_window);

    protected:
        void onRender(typeRT & p_type_data, objID p_editor_object_id) override;

    private:
        void LoadTreeList();

        std::vector<std::string> treeList;
    };
};
