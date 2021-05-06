/*!***************************************************************************************
\file       AudioScript.h
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

class audioScript : public scriptCPP
{
public:
		//Default constructor.
    audioScript() = default;

		//typeRT constructor for serialization.
    audioScript(typeRT p_data);


		componentType const type() const override { return audioScript::getType(); }
		static componentType const getType() { static componentType const s_type = componentType::forInherited<audioScript, scriptCPP>(); return s_type; }
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "audioScript", true, true, false); }

		//Class methods.
		void onStart() override;
		void onUpdate() override;

private:
	//Add data here...
};
