/*!***************************************************************************************
\file       CharacterStateScript.cpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "CharacterStateScript.h"
#include "EnemyAIScript.h"


//static float lifetime = 10.0f;
characterStateScript::characterStateScript(typeRT p_data) : scriptCPP(p_data)
{
}

void characterStateScript::onUpdate()
{
	--m_currentHealth;
	if (m_currentHealth <= 0)
	{
		destroySelf();
	}

	m_currentStamina += m_staminaRechargeRate * dt();
	if(m_currentStamina > m_maxStamina)
	{
		m_currentStamina = m_maxStamina;
	}
}

void characterStateScript::takeDamage(int p_damage)
{
	m_currentHealth -= p_damage;
}

void characterStateScript::loseStamina(float p_lostStamina)
{
	m_currentStamina -= p_lostStamina;
}

float characterStateScript::getStamina()
{
	return m_currentStamina;
}

int characterStateScript::getHealth()
{
	return m_currentHealth;
}

void characterStateScript::destroySelf()
{
	if(m_isEnemy)
	{
		getComponent<enemyAIScript>()->setActivationState(false);
	}
}

void characterStateScript::setIsEnemy(bool p_status)
{
	m_isEnemy = p_status;
}

bool characterStateScript::getIsEnemy()
{
	return m_isEnemy;
}
