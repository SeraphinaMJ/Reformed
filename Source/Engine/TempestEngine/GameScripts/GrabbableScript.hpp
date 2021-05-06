/*!***************************************************************************************
\file       GrabbableScript.hpp
\author     David Roberts
\date       10/23/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Script to make enemies grabbable.
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../Components/RigidBody.hpp"
#include <Vector3.hpp>


class grabbableScript : public scriptCPP
{
public:

	grabbableScript() = default;
	grabbableScript(typeRT p_data);
	componentType const type() const override { return grabbableScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<grabbableScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "grabbableScript", true, true, false); }


	void onStart() override;
	void onUpdate() override;
	void launch();
	void slam();
	void onCollisionEnter();


private:
	void disableGravity();
	float m_LethalSpeed = 5.0f;
	bool m_BeingThrown = false;
	bool m_BeingSlammed = false;
	std::shared_ptr<gameObject> m_ThePlayer;
	vector3 m_PreviousVelocity = vector3(0, 0, 0);
	float m_OBounce; //Bounciness
	float m_OFriction; //Friction
	std::shared_ptr<rigidBody> m_rigidBody;

};
