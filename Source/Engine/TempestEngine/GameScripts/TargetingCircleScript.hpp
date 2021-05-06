/*!***************************************************************************************
\file       TargetingCircleScript.hpp
\author     Cody Cannell
\date       10/03/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief   Script to create the redical
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"
#include <Vector3.hpp>
#include <unordered_map>
#include "InputComponent.hpp"
#include "IComponentRepository.hpp"



enum class spawnPrefabType : int {
	type_1 = 0,
	type_2 = 1,
	type_3 = 2,
	count,
};

enum class spawnPrefabGroup : int {
	group_1 = 0,
	group_2 = 1,
	group_3 = 2,
	group_4 = 3,
	group_5 = 4,
	group_6 = 5,
	group_7 = 6,
	group_8 = 7,
	group_9 = 8,
	group_10 = 9,
	count,
};
//========3rd Party Includes============================================================//

using  group = std::vector<std::shared_ptr<gameObject>>;
////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: mousePickingScript
\brief   Uses mouse picking manipulator and graphics system to create a circle on the
		ground
*****************************************************************************************/
class targetingCircleScript final : public scriptCPP
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Handles the update event to process input controls.
	*****************************************************************************************/
	targetingCircleScript() = default;
	targetingCircleScript(typeRT p_data);
	componentType const type() const override { return targetingCircleScript::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forInherited<targetingCircleScript, scriptCPP>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "targetingCircleScript", true, true, true); }

	void onStart() override;
	void onUpdate() override;
	void onShutdown() override;

	void pauseMenuShown(bool isShown) { m_pauseShown = isShown; }

	void setEnable(bool p_enable) { m_enabled = p_enable; }
	
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	bool m_enabled = false;

	bool m_pauseShown = false;
	
	void spawnMinionAt(int p_count, vector3 p_position);

	void pushNewPathToObjects(bool resetPath);
	//std::vector<std::shared_ptr<gameObject>> findClosestObjectsToPoint(unsigned p_ammount, vector3 const& origin);
	void setEnemysPathToPlayer();

	void setMinionType(const componentHandle<inputComponent> p_input);
	void setMinionGroup(const componentHandle<inputComponent> p_input);

	void buildMinionType(std::shared_ptr<gameObject> p_minion);
	void addMinionGroupComponent(std::shared_ptr<gameObject> p_minion);

	void targetingCircleScript::SetGroupAsSelected();

	group targetingCircleScript::getCurrentSelectedGroup();

	void spawnMeteor(vector3 spawnPoint);

	vector3 m_lastClickArray[3][10];

	vector3 lastClickPoint, currentClickPoint;
	spawnPrefabType currentActiveType = spawnPrefabType::type_1;
	spawnPrefabGroup currentActiveGroup = spawnPrefabGroup::group_1;

	std::string m_typeName = "Defender";
	std::string m_groupName = "Group_1";

	bool m_isFirstGroup = true;

};


