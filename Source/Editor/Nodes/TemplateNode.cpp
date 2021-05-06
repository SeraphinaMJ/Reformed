/*!***************************************************************************************
\file       TemplateNode.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		This is a template node used for understading how to build a node class.
*****************************************************************************************/

#include "TemplateNode.hpp"

TemplateNode::TemplateNode() : m_type("Template")
{
	setColor(ImVec4(255, 10, 10, 255));
}

TemplateNode::~TemplateNode()
{

}

void TemplateNode::onRender() const
{
	renderBase();
}

std::pair<bool, std::string> TemplateNode::validate()
{
	return validateBase();
}

std::pair<bool, std::string> TemplateNode::validateBase()
{
	//Add any conditions this type of node needs to meet
	if (true)
	{
		return std::make_pair<bool, std::string>(true, "Success");
	}
}

