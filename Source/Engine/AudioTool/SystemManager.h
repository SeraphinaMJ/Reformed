/*!***************************************************************************************
\file       SystemManager.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "SystemBase.h"
#include <vector>
#include "AE_Events.hpp"

class SystemManager
{
public:
	SystemManager();
	~SystemManager();
	

	bool intializeSystems();
	void updateSystems();
	void shutdownSystems();

	void addSystem(SystemBase* newSystem);
	
	template <typename T>
	T* getSystemByName(std::string systemName);
	void postEvent(Audio::EventBase* sourceEvent);
	
	template <typename classType, typename eventType, void(classType::*function)(eventType*)>
	void registerEventCallback(classType* instance);

private:
	Audio::EventManager* eventManager;
	std::vector<SystemBase*> SystemList;
};

template <typename T>
T* SystemManager::getSystemByName(std::string systemName)
{
	for (unsigned i = 0; i < SystemList.size(); i++)
	{
		SystemBase* systemPtr = SystemList[i];
		if (systemPtr->getSystemName() == systemName)
			return reinterpret_cast<T*>(systemPtr);
	}

	return nullptr;
}

template <typename classType, typename eventType, void(classType::*function)(eventType*)>
void SystemManager::registerEventCallback(classType* instance)
{
	eventManager->RegisterEventCallback<classType, eventType, function>(instance);
}