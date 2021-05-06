/*!**********************************************************************************************************
\file       GamePadSystem.hpp
\author     Henry Brobeck
\date       10/16/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#include "SystemBase.hpp"
#include "Input/GamepadInputAdapter.hpp"

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
#include <Logger.hpp>
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//


//========  Helper Structs  ===============================================================================//

class gamepadSystem final : public systemBase
{
public:
	//====  Static  =======================================================================================//
	//====  Pure-Virtual  =================================================================================//
	//====  Constructor/Destructor  =======================================================================//
	gamepadSystem();
	//====  Accessor Methods  =============================================================================//
	//====  Virtual/Overrides  ============================================================================//
	//====  Operators  ====================================================================================//
	//====  Interface Declarations (For each interface Implemented)  ======================================//
	//====  Everything Else  ==============================================================================//
	~gamepadSystem() override;

protected:
	void onInitialize() override;
	void onStartFrame() override;
	void onShutdown() override;
public:
	static std::string& getName();
	const std::string& name() const override;
	gamepadInputAdapter* getInputAdapter();
	void setFFB(unsigned short p_id, float p_left, float p_right);
private:
	static std::string m_name;
	//====  Data  =========================================================================================//
	logger log;
	gamepadInputAdapter* m_inputAdapter;
};



// namespace <namespace name>

