/*!***************************************************************************************
\file       BombScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/RigidBody.hpp"
#include <Vector3.hpp>

class bombScript : public scriptCPP
{
public:

	bombScript() = default;
	bombScript(typeRT p_data);
	componentType const type() const override { return bombScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<bombScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "bombScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;

	void detonate();
	void explode();
	float m_detonationTimer;
	float m_detonationTime = 10;
	float m_blink = 0.5;
	float m_strength = 8;
	float m_radius = 1;
	bool m_isActivated = false;

private:
	vector3 m_originalSize;
	float m_scaleFactor = 10000.0f;
};