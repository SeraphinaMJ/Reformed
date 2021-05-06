/*!***************************************************************************************
\file       PersistenceManipulatorInterface.hpp
\author     David Roberts
\date       4/13/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include <string>

class persistenceManipulatorInterface
{
public:
	virtual void saveCurrentScene() = 0;
	virtual void saveCurrentSceneAs(std::string p_name) = 0;
	virtual void load(std::string p_name) = 0;
	virtual void new_scene() = 0;

	virtual void quickStoreTo() = 0;
	virtual void quickRestoreFrom() = 0;
};
