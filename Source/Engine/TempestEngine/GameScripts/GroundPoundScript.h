/*!***************************************************************************************
\file       GroundPoundScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../SceneManagement/GameObject.hpp"

class groundPoundScript : public scriptCPP
{
public:
	groundPoundScript() = default;
	groundPoundScript(typeRT p_data);
	bool checkGroundPound();
	componentType const type() const override { return groundPoundScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<groundPoundScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "groundPoundScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;

private:
	bool m_pounding = false;
	float m_poundStrength = 20.0f;
	float m_scaleFactor = 10000.0f;
	float m_radius = 7.5f;
};