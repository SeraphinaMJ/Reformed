/*!***************************************************************************************
\file       UI_System.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include "SystemBase.h"
#include "UIEventTypes.h"
#include "SystemManager.h"
#include "BusInterface.h"

class UI_ComponentsBase;

class UI_System : public SystemBase
{
public:
	UI_System(Audio::BusInterface* busInterface_p);
	~UI_System();

	bool onInitialize() override;
	void onUpdate() override;
	void onShutdown() override;

	void addComponents(UI_ComponentsBase* newComponent);
	bool windowCloseCommand();
	const std::string& getSystemName() const override { return systemName; }

	void sendEvent(Audio::EventBase* eventbase) { UI_EventManager->QueueEvent(eventbase); }

private:
	GLFWwindow* AppWindow;
	std::string systemName;

	Audio::BusInterface* bInterface;

	Audio::EventManager* UI_EventManager;
	std::vector<UI_ComponentsBase*> UI_elements;
};

class UI_ComponentsBase
{
public:
	UI_ComponentsBase() = default;

	void setSystemPointer(UI_System* owner) { ownerSystem = owner; }
	void sendGlobalEvent(Audio::EventBase* eventbase) { ownerSystem->getSystemManager()->postEvent(eventbase); }
	void sendLocalEvent(Audio::EventBase* eventbase)  { ownerSystem->sendEvent(eventbase); }

	virtual ~UI_ComponentsBase() = default;
	virtual void onUpdate() = 0;

private:
	UI_System* ownerSystem;
};
