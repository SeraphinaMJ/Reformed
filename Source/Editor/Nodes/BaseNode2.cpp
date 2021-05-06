/*!***************************************************************************************
\file       BaseNode2.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#include "BaseNode2.hpp"

BaseNode2::BaseNode2()
{
  setName("Cool Node");
  setIntputs(2);
  setOutputs(4);
}

BaseNode2::~BaseNode2()
{

}

void BaseNode2::renderBase() const
{
  ImGui::BeginGroup(); // Lock horizontal position
  ImGui::Text("My name is %s", getName().c_str());
  ImGui::Text("My type is %s", getType().c_str());
  ImGui::Text("ID: %i", getId());
  ImGui::EndGroup();
}

std::pair<bool, std::string> BaseNode2::validateBase()
{
  //Add any rules here that are specific to this node
  if (getNumChildren() >= 1)
  {
    return std::make_pair<bool, std::string>(true, "Base Success");
  }

  return std::make_pair<bool, std::string>(false, "Need at least one child.");
}
