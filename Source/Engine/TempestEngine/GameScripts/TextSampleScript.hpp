/*!***************************************************************************************
\file       TextSampleScript.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

class textSampleScript final : public scriptCPP
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
	public:

		textSampleScript() = default;
		textSampleScript(typeRT p_data);

		componentType const type() const override { return textSampleScript::getType(); }
		static componentType const getType() { static componentType const s_type = componentType::forInherited<textSampleScript, scriptCPP>(); return s_type; }
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "textSampleScript", true, true, false); }

		void onStart() override;
		void onUpdate() override;

	private:

		

};

