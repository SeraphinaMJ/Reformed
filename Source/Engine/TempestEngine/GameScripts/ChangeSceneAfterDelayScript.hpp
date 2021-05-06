/*!***************************************************************************************
\file       ChangeSceneAfterDelayScript.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"

class changeSceneAfterDelayScript : public scriptCPP
{
public:
	changeSceneAfterDelayScript();
	changeSceneAfterDelayScript(typeRT p_data);

	componentType const type() const override { return changeSceneAfterDelayScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<changeSceneAfterDelayScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "changeSceneAfterDelayScript", true, true, false); }

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;

protected:
	void onStart() override;
	void onUpdate() override;

private:
	float m_delay = 5.0f;
	float m_time_remaining;
	std::string m_scene_name = "";
};

