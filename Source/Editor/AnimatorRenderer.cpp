/*!**********************************************************************************************************
\file       AnimatorRenderer.cpp
\author     Henry Brobeck
\date       3/6/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief 
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "AnimatorRenderer.hpp"
#include "StyleKeeper.hpp"
#include "../AudioTool/ImGui/imgui.h"
#include "HierarchySelectionKeeper.hpp"
#include "EngineController.hpp"
#include <Reflection.hpp>

Editor::animatorRenderer::animatorRenderer(editorWindow* p_parent_window):
	componentRenderer(p_parent_window)
{
	auto & styles = getStyleKeeper();
	styles.setStyle("*", "currentAnimation", "readonly",1);
	styles.setStyle("*", "animations", "readonly", 1);

}

void Editor::animatorRenderer::onRender(typeRT& p_type_data, objID p_editor_object_id)
{
	constexpr int buff_size = 2048;
	static char tag_buffer[buff_size] = { 0 };
	static int start_frame = 0;
	static int end_frame = 0;
	static float fps = 24.0f;
	static bool loop = false;
	ImGui::InputInt("Start Frame", &start_frame);
	ImGui::InputInt("End Frame", &end_frame);
	ImGui::InputFloat("FPS", &fps);
	ImGui::InputText("Tag", tag_buffer, buff_size);
	ImGui::RadioButton("Loop", loop);
	if(ImGui::Button("New Clip"))
	{
		typeRT anim;
		std::string tag;
		tag.copy(tag_buffer, sizeof(tag_buffer));
		anim.insertMember(typeRT("tag",tag));
		anim.insertMember(typeRT("frameBegin", start_frame));
		anim.insertMember(typeRT("frameEnd", end_frame));
		anim.insertMember(typeRT("fps", fps));
		anim.insertMember(typeRT("loop", loop));
		auto& arr = p_type_data.member("animations").array();
		arr.push_back(anim);
		
	}
}
