/*!***************************************************************************************
\file       InputRenderer.cpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the inputComponent inspector renderer functions
*****************************************************************************************/

//======== Self Include ================================================================//
#include "InputRenderer.hpp"

//======== 1st Party Includes ==========================================================//
#include "Reflection.hpp"

//======== 3rd Party Includes ==========================================================//
#include <imgui.h>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Static Declarations =========================================================//
static bool renderAsInputTextRaw(typeRT & p_type_data);
static bool renderAsUnsignedIntRaw(typeRT & p_type_data);

//======== Constructors & Destructor ===================================================//
Editor::inputRenderer::inputRenderer(editorWindow* p_parent_window)
	: componentRenderer(p_parent_window)
{
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
void Editor::inputRenderer::onRender(typeRT& p_type_data, objID p_editor_object_id)
{
	///// to the next maintainer
	///// I failed to make this work properly.  Because the maps are being rebuilt every frame (see updateFromTypeRT),
	///// and using the requests to do so, they may actually be a race condition between the updating of the lists from the editor
	///// and the next editor rendering frame.  In order to fix this issue, these maps need to be statically modifiable.
	///// That is, users of the input need to be able to read and write to the maps on demand (and any conversions need to be
	///// completely hidden from the user).  The letter-to-scancode conversion, in particular, should not be happening anywhere
	///// that is publicly visible.
	//bool l_changed = false;

	//if (p_type_data.members().find("context") == p_type_data.members().end())
	//	return false;  // bail if missing property
	//if (renderAsInputTextRaw(p_type_data.member("context")))
	//	l_changed = true;

	//if (p_type_data.members().find("controllerToNameMap") == p_type_data.members().end())
	//	return false; // bail early if missing property
	//const int l_entry_count_orig = p_type_data.member("controllerToNameMap").array().size();
	//int l_entry_count = l_entry_count_orig;
	//ImGui::InputInt("Size", &l_entry_count, 1, 1);
	//if (l_entry_count < 0)
	//	l_entry_count = l_entry_count_orig;
	//for(int l_count = l_entry_count_orig; l_count < l_entry_count; ++l_count)
	//{
	//	typeRT l_entry;
	//	l_entry.setTypeName("controllerToNameEntry");
	//	l_entry.insertMember(typeRT("controllerID", static_cast<unsigned int>(0)));
	//	l_entry.insertMember(typeRT("name", std::string("name")));
	//	p_type_data.member("controllerToNameMap").array().push_back(l_entry);
	//	l_changed = true;
	//}
	//for(int l_count = l_entry_count_orig; l_count > l_entry_count; --l_count)
	//{
	//	p_type_data.member("controllerToNameMap").array().pop_back();
	//	l_changed = true;
	//}
	//for(auto & l_entry : p_type_data.member("controllerToNameMap").array())
	//{
	//	if (renderAsUnsignedIntRaw(l_entry.member("controllerID")))
	//		l_changed = true;
	//	ImGui::SameLine();
	//	if (renderAsInputTextRaw(l_entry.member("name")))
	//		l_changed = true;
	//}
	//return l_changed;
  /// sorry, I tried :_(
}

//======== Functionality ===============================================================//

//======== Helper Functions ============================================================//
static bool renderAsInputTextRaw(typeRT & p_type_data)
{
	constexpr int buff_size = 2048;
	char buffer[buff_size] = { 0 };
	p_type_data.getString().copy(buffer, sizeof(buffer));
	buffer[buff_size - 1] = '\0';
	if (ImGui::InputText(p_type_data.getVariableName().c_str(), &buffer[0], buff_size, ImGuiInputTextFlags_EnterReturnsTrue))
	{
		std::string l_test(buffer);
		p_type_data.setString(l_test);
		return true;
	}
	return false;
}

static bool renderAsUnsignedIntRaw(typeRT & p_type_data)
{
	int l_value = p_type_data.getUInt();
	ImGui::InputInt(p_type_data.getVariableName().c_str(), &l_value);
	unsigned int l_unsigned_value = static_cast<unsigned int>(l_value);
	if(l_unsigned_value != p_type_data.getUInt())
	{
		p_type_data.setUInt(l_unsigned_value);
		return true;
	}
	return false;
}
