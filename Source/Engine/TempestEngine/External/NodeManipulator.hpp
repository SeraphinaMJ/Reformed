/*!***************************************************************************************
\file       NodeManipulator.hpp
\author     Ryan Booth
\date       9/20/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include "NodeManipulatorInterface.hpp"

class engine;
class systemManagerInterface;

class nodeManipulator : public nodeManipulatorInterface
{
public:
  nodeManipulator(engine * p_engine, systemManagerInterface * p_system_manager);

  virtual ~nodeManipulator() = default;

  typeRT createRenderedNode(const std::pair<std::string, std::string> & p_node_type) override;

  std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> getNodeDefinitions() override;

private:
  engine * m_engine;
  systemManagerInterface * m_system_manager;
};
