/*!***************************************************************************************
\file       Composite.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node in a behavior tree that has multiple child nodes.
*****************************************************************************************/
#include "Composite.hpp"

BehaviorType Composite::GetType()
{
    return BehaviorType::COMPOSITE;
}

BehaviorPtr Composite::getCurrentChild()
{
    auto task = getTask();
    // if task has some history then we can access child nodes
    if (task->NoHistory() == false)
    {
        return childNodes[task->GetChildIndex()];
    }
    else
    {
        return nullptr;
    }
}

typeRT Composite::compositeOnRender()
{
    // final typert data to return
    typeRT l_data;

    // typeRT data that is not displayed but does affect functionality of node
    std::vector<typeRT> l_non_render_members;

    typeRT l_non_render_data;
    l_non_render_data.setTypeName("Node Non Render Data");
    l_non_render_data.setVariableName("Node Non Render Data");

    typeRT l_inputs("Inputs", int(1));
    l_non_render_members.push_back(l_inputs);

    typeRT l_outputs("Outputs", int(6));  // ask ryan about implementing max child nodes
    l_non_render_members.push_back(l_outputs);

    //Push back non render data
    for (auto i_member : l_non_render_members)
    {
        l_non_render_data.insertMember(i_member);
    }

    l_data.insertMember(l_non_render_data);


    // now for render data
    typeRT l_render_data;
    l_render_data.setTypeName("Node Render Data");
    l_render_data.setVariableName("Node Render Data");

    typeRT l_type_name("Type", (std::string)"Composite");
    l_render_data.insertMember(l_type_name);
    l_data.insertMember(l_render_data);


    return l_data;
}

void Composite::Init()
{
    InitDebug();

    assert(!childNodes.empty());
    assert(getTask()->GetChildIndex() == 0);
    // set our phase to progressing
    getTask()->SetPhase(BehaviorPhase::PROGRESSING);
    // give task to child
    Behavior::GiveToChild(getTask());
}

Composite::Composite()
{}

std::vector<BehaviorPtr> Composite::GetChildren()
{
    return childNodes;
}

void Composite::addChild(BehaviorPtr newChild)
{
    childNodes.push_back(newChild);
}

void Composite::removeChild(BehaviorPtr child)
{
    auto it = std::find(childNodes.begin(), childNodes.end(), child);
    childNodes.erase(it);
}

void Composite::clearChildren()
{
    childNodes = {}; // clears it
}
