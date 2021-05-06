/*!***************************************************************************************
\file       LifetimeScript.hpp
\author     David Roberts
\date       10/26/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Script to kill an object after a set amount of time.
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class lifetimeScript : public scriptCPP
{
public:

	lifetimeScript() = default;
	lifetimeScript(typeRT p_data);
	componentType const type() const override { return lifetimeScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<lifetimeScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "lifetimeScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;
	void destroySelf();
	void setLifetime(float p_lifetime);
	float getLifetime();

private:
	float m_Timer = 0;
	float m_Lifetime = 3.0f;
};
