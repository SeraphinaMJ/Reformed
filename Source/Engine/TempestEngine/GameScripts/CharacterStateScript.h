/*!***************************************************************************************
\file       CharacterStateScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class characterStateScript : public scriptCPP
{
public:
	characterStateScript() = default;
	characterStateScript(typeRT p_data);
	componentType const type() const override { return characterStateScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<characterStateScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "characterStateScript", true, true, false); }

	void onUpdate() override;
	void takeDamage(int p_damage);
	void loseStamina(float p_lostStamina);
	float getStamina();
	int getHealth();
	void destroySelf();
	void setIsEnemy(bool p_status);
	bool getIsEnemy();

private:
	int m_maxHealth = 10;
	int m_currentHealth = 10;

	float m_maxStamina = 10;
	float m_currentStamina = 10;
	float m_staminaRechargeRate = 3.0f;

	bool m_isEnemy = true;
};
