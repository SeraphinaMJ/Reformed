#include "Leaf.hpp"

BehaviorPtr Leaf::getCurrentChild()
{
    return nullptr;
}

BehaviorType Leaf::GetType()
{
    return BehaviorType::LEAF;
}

typeRT Leaf::leafOnRender()
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

    typeRT l_outputs("Outputs", int(0));
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

    typeRT l_type_name("Type", (std::string)"Leaf");
    l_render_data.insertMember(l_type_name);
    l_data.insertMember(l_render_data);


    return l_data;
}

std::vector<BehaviorPtr> Leaf::GetChildren()
{
    return {}; // no children
}

void Leaf::addChild(BehaviorPtr)
{
    throw std::runtime_error("Tried to add child to leaf node. Not allowed.");
}

void Leaf::Init()
{
    InitDebug();

    // initialize phase to progressing by default
    getTask()->SetPhase(BehaviorPhase::PROGRESSING);
}
