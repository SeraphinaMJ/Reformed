/*!***************************************************************************************
\file       StressTestScript.hpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the script for configuring and running the stress test scene
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

class stressTestScript : public scriptCPP
{
public:
	stressTestScript() = default;
	stressTestScript(typeRT p_data);
	componentType const type() const override { return stressTestScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<stressTestScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "stressTestScript", true, true, false); }
private:
	void onStart() override;
	void onUpdate() override;
};

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

