/*!***************************************************************************************
\file       main.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#include "UI_System.h"
#include "AudioSystem.h"
#include "SystemManager.h"

int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	SystemManager* manager = new SystemManager();

	

	// Add systems to manager
	AudioSystem* mySystem = new AudioSystem();
	manager->addSystem(mySystem);

	manager->addSystem(new UI_System(mySystem->getBusInterface()));

	manager->intializeSystems();

	// Create internal loop
	while (!manager->getSystemByName<UI_System>("UISystem")->windowCloseCommand())
	{
		manager->updateSystems();
	}

	manager->shutdownSystems();

	delete manager;

	return 0;
}