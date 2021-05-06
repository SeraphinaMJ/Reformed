/*!***************************************************************************************
\file       NodeManipulator.cpp
\author     Ryan Booth
\date       9/20/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "NodeManipulator.hpp"
#include "../Nodes/NodeFactory.hpp"

//#include "BehaviorSystem/BehaviorVisitor.hpp"
#include "BehaviorSystem/BehaviorInterpreter.hpp"

nodeManipulator::nodeManipulator(engine* p_engine, systemManagerInterface* p_system_manager) : m_engine(p_engine), m_system_manager(p_system_manager)
{
}

typeRT nodeManipulator::createRenderedNode(const std::pair<std::string, std::string> & p_node_type)
{
    typeRT l_result = BehaviorInterpreter::GetInstance().GetDefaultTypeRT(p_node_type.first);
    return l_result;
}

std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> nodeManipulator::getNodeDefinitions()
{
    return NodeFactory::getNodeDefinitions();
}
