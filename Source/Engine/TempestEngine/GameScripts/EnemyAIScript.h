/*!***************************************************************************************
\file       EnemyAIScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../Components/RigidBody.hpp"
#include "../SceneManagement/Transform.hpp"


class enemyAIScript : public scriptCPP
{
public:
	enemyAIScript() = default;
	enemyAIScript(typeRT p_data);
	componentType const type() const override { return enemyAIScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<enemyAIScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "enemyAIScript", true, true, false); }

	void onStart() override;
	void onUpdate() override;

	bool getActivationState();
	void setActivationState(bool p_state);

private:
	std::vector<std::shared_ptr<gameObject>> m_targets;
	float m_shortestDistance = FLT_MAX;
	float m_atkDistance = 3.f;
	componentHandle<transform> m_myTransform;
	componentHandle<rigidBody> m_myRigidBody;
	std::shared_ptr<gameObject> m_currentTarget = nullptr;
	vector3 m_currentDirection;
	float m_scaleFactor = 100;
	float m_maxSpeed = 10;
	bool m_ActivationState = true;
	bool m_attackState = false;
};
