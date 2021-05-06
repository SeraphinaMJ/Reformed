/*!***************************************************************************************
\file       ExitGameButton.hpp
\author     Alex Kudebeh
\date       11/26/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Exit game button script
*****************************************************************************************/
#pragma once
#include "../Components/ScriptCPP.hpp"
#include "../Components/Button.hpp"

class ButtonPressedEvent;

class exitGameButtonScript : public scriptCPP
{
public:
	exitGameButtonScript();
	exitGameButtonScript(typeRT p_data);

	componentType const type() const override { return exitGameButtonScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<exitGameButtonScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "exitGameButtonScript", true, true, false); }

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;

    void HandleEvent(ButtonPressedEvent& ThisEvent);

protected:
	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;

private:
	std::string m_scene_name = "";
	componentHandle<button> m_button;
    bool m_inside_conformation = false;

};
