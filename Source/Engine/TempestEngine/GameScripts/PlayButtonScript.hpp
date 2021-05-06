/*!***************************************************************************************
\file       PlayButtonScript.hpp
\author     David Roberts
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../Components/Button.hpp"
#include "ButtonPressedEvent.hpp"

class ButtonPressedEvent;

class playButtonScript : public scriptCPP
{
public:
	playButtonScript();
	playButtonScript(typeRT p_data);

	componentType const type() const override { return playButtonScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<playButtonScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "playButtonScript", true, true, false); }

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;

    void HandleEvent(ButtonPressedEvent& ThisEvent);

	void setSceneName(std::string name);

    void setNeedsConfirmation(bool p_bool);

    bool getNeedsConfirmation() const { return m_needs_confirmation; }

protected:
	void onStart() override;
	void onUpdate() override;

private:
	std::string m_scene_name = "";
	componentHandle<button> m_button;
    bool m_needs_confirmation = false;
    bool m_in_confirmation_process = false;
    bool m_confirmation_passed = false;
};
