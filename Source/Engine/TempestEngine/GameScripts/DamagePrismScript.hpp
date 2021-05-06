/*!***************************************************************************************
\file       DamagePrismScript.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include <ComponentTypeDetails.hpp>

class damagePrismScript : public scriptCPP
{
public:

	damagePrismScript() = default;
	damagePrismScript(typeRT p_data);
	componentType const type() const override { return damagePrismScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<damagePrismScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "damagePrismScript", true, true, false); }

	void damage(float p_amount);
	void changeColor(float p_amount);
	void updateState();


private:
	int m_rgbMod = 3;
	bool m_up = false;
};