/*!***************************************************************************************
\file       SystemBase.hpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include <string>
//#include "SystemManager.h"

class SystemManager;

class SystemBase
{
public:
	SystemBase();
	virtual ~SystemBase();

	virtual bool onInitialize() = 0;
	virtual void onUpdate() = 0;
	virtual void onShutdown() = 0;

	virtual const std::string& getSystemName() const;

	static void setSystemManager(SystemManager* manager);
	static SystemManager* getSystemManager();
	
private:
	std::string systemName;
	static SystemManager* systemManager;
	//
};