/*!***************************************************************************************
\file       ScriptingSystem.hpp
\author     Aaron Damyen
\date       5/17/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the Scripting System.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"
#include "../SystemBase.hpp"
#include "../SystemManagement/SystemManagerSTA.hpp"
#include "../GameObjectGatherer.hpp"
//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: scriptingSystem
\brief   This is the definition of the scriptingSystem class for ensuring scripts are executed
*****************************************************************************************/
class scriptingSystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("scriptingSystem"); return n; }
	virtual const std::string& name() const override { return getName(); }


	template <typename componentType>
	std::vector<std::shared_ptr<gameObject>> findObjectsByComponent()
	{
		GameObjectFiltering::componentTypeSet l_pattern;
		l_pattern.setType(componentType::getType());
		return getSystemManager()->getGameObjectGatherer()->getList(l_pattern);
	}
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Handles the onInitialize event from system manager, initializing the system.
	*****************************************************************************************/
	void onInitialize() override;

	/*!***************************************************************************************
	\brief  Handles the onUpdate event from system manager, updating all scripts.
	*****************************************************************************************/
	void onUpdate() override;

	void onLevelUnload(const levelUnloadEvent & p_event) override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

