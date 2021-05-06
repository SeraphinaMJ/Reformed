/*!***************************************************************************************
\file       BaseNode1.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#include "BaseNode1.hpp"

BaseNode1::BaseNode1()
{
  setName("BaseNode1");

  setIntputs(1);
  setOutputs(1);
}

BaseNode1::~BaseNode1()
{
  
}

void BaseNode1::renderBase() const
{
  ImGui::BeginGroup(); // Lock horizontal position
  ImGui::Text("My name is %s", getName().c_str());
  ImGui::Text("My type is %s", getType().c_str());
  ImGui::Text("ID: %i", getId());
  ImGui::EndGroup();
}
