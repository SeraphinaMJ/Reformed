/*!**********************************************************************************************************
\file       InstantReplaySystem.hpp
\author     Cole Astaire
\date       10/30/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Declaration of instant replay system to play back gameplay during gameplay
************************************************************************************************************/
#pragma once

//========  1st Party Includes  ===========================================================================//
#include "../SystemBase.hpp"
#include <vector>
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

class instantReplaySystem final : public systemBase
{
public:
	//====  Static  =======================================================================================//

	//====  Pure-Virtual  =================================================================================//
	static const std::string& getName() { static const std::string n("instantReplaySystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	//====  Constructor/Destructor  =======================================================================//

	//====  Accessor Methods  =============================================================================//
	//====  Virtual/Overrides  ============================================================================//
	//====  Operators  ====================================================================================//
	//====  Interface Declarations (For each interface Implemented)  ======================================//
	//====  Everything Else  ==============================================================================//

	float GetCurrentTimeStamp() { return m_current_time_stamp; }


protected:

	void onInitialize() override;

	void onUpdate() override;

	void onShutdown() override;

private:
	//====  Status  =========================================================================================//
	float m_current_time_stamp = 0.0f;

	//====  Data  =========================================================================================//

	//====  Data  =========================================================================================//
	float test_duration = 1.0f;
	float test_timer = 0.0f;
	
};

// namespace <namespace name>

