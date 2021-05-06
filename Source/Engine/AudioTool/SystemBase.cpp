/*!***************************************************************************************
\file       SystemBase.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "SystemBase.h"
#include "SystemManager.h"

SystemManager* SystemBase::systemManager;

SystemBase::SystemBase() : systemName("Default")
{
}

SystemBase::~SystemBase()
{
}

const std::string& SystemBase::getSystemName() const
{
	return systemName;
}

void SystemBase::setSystemManager(SystemManager* manager)
{
	systemManager = manager;
}

SystemManager* SystemBase::getSystemManager()
{ 
	return systemManager; 
}