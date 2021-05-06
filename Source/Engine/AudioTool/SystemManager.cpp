/*!***************************************************************************************
\file       SystemManager.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "SystemManager.h"

SystemManager::SystemManager() : eventManager(new Audio::EventManager())
{
}

SystemManager::~SystemManager()
{
	for (unsigned i = 0; i < SystemList.size(); i++)
		delete SystemList[i];
}

bool SystemManager::intializeSystems()
{
	for (unsigned i = 0; i < SystemList.size(); i++)
		if (!SystemList[i]->onInitialize()) return false;

	return true;
}

void SystemManager::updateSystems()
{
	for (unsigned i = 0; i < SystemList.size(); i++)
		SystemList[i]->onUpdate();

	eventManager->ParseAllEvents();
}

void SystemManager::shutdownSystems()
{
	for (unsigned i = 0; i < SystemList.size(); i++)
		SystemList[i]->onShutdown();
}

void SystemManager::addSystem(SystemBase* newSystem)
{
	if (newSystem)
	{
		newSystem->setSystemManager(this);
		SystemList.push_back(newSystem);
	}
}

void SystemManager::postEvent(Audio::EventBase* sourceEvent)
{
	eventManager->QueueEvent(sourceEvent);
}