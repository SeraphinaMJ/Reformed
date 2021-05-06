/*!***************************************************************************************
\file       NodeFactory.hpp
\author     Ryan Booth
\date       9/20/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include <Reflection.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

class NodeFactory
{
public:

  static void parseNodesDefinitions();

  static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> getNodeDefinitions();

private:

  //List of types of nodes and their names
  //Key = behavior tree, script, shader, etc. pair = class name, friendly name (can be the same)
  static std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> m_node_factory_list;
};
