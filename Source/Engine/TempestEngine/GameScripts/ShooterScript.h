/*!***************************************************************************************
\file       ShooterScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

class shooterScript : public scriptCPP
{
public:
	shooterScript() = default;
	shooterScript(typeRT p_data);
	componentType const type() const override { return shooterScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<shooterScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "shooterScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;
	void shoot(typeRT p_objectData);

private:
	typeRT m_ObjectToShoot;
	float m_Timer = 5.0f;
	float m_ShootTime = 5.0f;
	float m_scaleFactor = 10000.0f;
};