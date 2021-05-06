/*!***************************************************************************************
\file       DynamiteScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class dynamiteScript : public scriptCPP
{
public:
	dynamiteScript() = default;
	dynamiteScript(typeRT p_data);
	componentType const type() const override { return dynamiteScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<dynamiteScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "dynamiteScript", true, true, false); }

private:
};