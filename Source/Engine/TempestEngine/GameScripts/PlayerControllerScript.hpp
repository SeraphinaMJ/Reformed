/*!***************************************************************************************
\file       PlayerControllerScript.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/RigidBody.hpp"
#include <Vector3.hpp>

#include <queue>

class playerControllerScript: public scriptCPP
{
public:
	playerControllerScript();
	~playerControllerScript();
	playerControllerScript(typeRT p_data);
	componentType const type() const override { return playerControllerScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<playerControllerScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "playerControllerScript", true, true, false); }

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;

	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;
	void updateTimers();
	void updateDefault();
	vector3 getInput();
	void updateMovement();
	void updateFacing();
	static int getTotalPlayerNumber();


private:

	static int s_totalPlayerNumber;
	int m_playerNumber;

	float m_MoveAccelerationAir = 2;
	float m_MoveAcceleration = 3;
	float m_SlamSpeed = 25;

	//These are only 2d movement and doesn't effect fall speed
	float m_MaxMoveSpeedGround = 7;
	float m_MaxMoveSpeedAir = 9;

	float m_DashSpeed = 35;
	float m_DashingTime = 0.3f;
	float m_DashCooldown = 1;
	float m_JumpStrength = 17;
	float m_SlamRadius = 3;
	float m_SlamStrength = 8;
	float m_ThrowSpeed = 10;
	float m_lariatRadius = 4.0f;
	float m_lariatStrength = 8.f;

	// The amount of the time that the player can spend gaining vertical
	// momentum by holding the jump button
	float m_JumpUpwardsMomentumTime = 0.5f;

	// The amount of time that the player spends in the reflecting state
	float m_ReflectTime = 0.4f;

	/// ///////////////////////////////////////////////////////////////////////
	/// Timers

	float m_JumpAgainWait = 0.3f;
	float m_JumpAgainTimer = 0;
	float m_DashCooldownTimer = 0;
	float m_JumpUpwardsMomentumTimer = 0;

	/// ///////////////////////////////////////////////////////////////////////

	 /// ///////////////////////////////////////////////////////////////////////
	/// Data that various states need to remember

	vector3 m_DashDir;
	vector3 m_startPos;

	std::shared_ptr<gameObject> m_GrabbedObj;

	float m_GrabPosY;

	vector3 m_LastLeftJoy = vector3(0,0,0);
	vector3 m_CurrentLeftJoy = vector3(0, 0, 0);

	bool m_UpwardsMomentum = false;

	enum state
	{
		default_state,
		reflect,
		jump,
		dash,
		slam,
		carry,
		carry_jumping,
		spinning,
		spin_jumping,

		TOTAL
	};

	struct update_request
	{
		update_request(state p_state, int p_priority): m_state(p_state), m_priority(p_priority){}
		state m_state;
		int m_priority;
	};

	class CompFunctor
	{
	public:
		bool operator()(const update_request& a, const update_request& b) { return a.m_priority < b.m_priority; }
	};

	std::map<state, void (playerControllerScript::*)()> m_updates;
	std::priority_queue<update_request,std::vector<update_request>,CompFunctor> m_queue;

	//Number to multiply by to get appropriately large numbers for applying forces
	float m_scaleFactor = 10000;
	float m_xoffset = 0;
	float m_yoffset = 0;
	float m_grabDistance = 10;
	collisionLayer m_grabableLayers = static_cast<collisionLayer>(collisionLayer::enm_player | collisionLayer::enm_enemy);
	bool m_grabbing = false;

	void updateStateType(state p_state);
	bool checkJump();
	bool checkDash();
	bool checkGrab();
	bool checkLariat();
	bool checkThrow();
};
