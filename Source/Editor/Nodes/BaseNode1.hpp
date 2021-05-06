/*!***************************************************************************************
\file       BaseNode1.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief		Example node of a TemplateNode.
*****************************************************************************************/

#pragma once

#include <string>

#include "TemplateNode.hpp"


class BaseNode1 : public TemplateNode
{
public:
	BaseNode1();

	~BaseNode1();

	void renderBase() const override;

protected:

private:
};