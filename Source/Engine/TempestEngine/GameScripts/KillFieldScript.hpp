/*!***************************************************************************************
\file       KillFieldScript.hpp
\author     Alex Kudebeh
\date       12/7/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Kill things that drop into the abyss
*****************************************************************************************/
#pragma once
#include "ScriptCPP.hpp"

class killFieldScript : public scriptCPP
{
public:
	killFieldScript() = default;
	killFieldScript(typeRT p_data);
	componentType const type() const override { return killFieldScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<killFieldScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "killFieldScript", true, true, true); }

	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;
private:

};