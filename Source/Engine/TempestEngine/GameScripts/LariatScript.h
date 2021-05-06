/*!***************************************************************************************
\file       LariatScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"


class lariatScript : public scriptCPP
{
public:
	lariatScript() = default;
	lariatScript(typeRT p_data);
	componentType const type() const override { return lariatScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<lariatScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "lariatScript", true, true, false); }

	void onUpdate() override;

};