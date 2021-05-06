/*!***************************************************************************************
\file       JumpPadScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class jumpPadScript : public scriptCPP
{
public:
	jumpPadScript() = default;
	jumpPadScript(typeRT p_data);
	componentType const type() const override { return jumpPadScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<jumpPadScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "jumpPadScript", true, true, false); }

	void onUpdate() override;

private:
	float m_radius = 5.0f;
	float m_upwardStrength = 25.0f;
	float m_scaleFactor = 10000.0f;
};