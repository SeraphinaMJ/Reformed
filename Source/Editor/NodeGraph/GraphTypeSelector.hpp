/*!***************************************************************************************
\file       GraphTypeSelector.hpp
\author     Charles Boudousquie
\date       10/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      Object that displays a menu to select desired graph type.
*****************************************************************************************/
#pragma once

namespace Editor
{
  enum class GraphType{BEHAVIORTREE, SCRIPT};

  class GraphTypeSelector
  {
  
    GraphType currentType = GraphType::BEHAVIORTREE;
    
    public:
    
      void DisplayMenu();
      GraphType GetSelectedType();
    
  };
};

