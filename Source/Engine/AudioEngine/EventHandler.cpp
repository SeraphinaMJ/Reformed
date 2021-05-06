/*!***************************************************************************************
\file       EventHandler.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "EventHandler.hpp"

Audio::EventID Audio::EventBase::numTypes = 0;

Audio::EventManager::EventManager()
{
	Listeners.resize(1);
	//Listeners[0].resize(1, NULL);
}

void Audio::EventManager::QueueEvent(EventBase* incomingEvent)
{
	if (incomingEvent)
	{
		EventQueue.push(incomingEvent);
	}
}

void Audio::EventManager::ParseAllEvents()
{
	size_t numListenerTypes = Listeners.size();
	while (!EventQueue.empty())
	{
		EventBase* topEvent = EventQueue.front();
		EventQueue.pop();

		EventID EventType = topEvent->GetTypeID();

		if (numListenerTypes >= EventType)
		{
			std::vector<EventCallback*>& eventListeners = Listeners[EventType];

			for (size_t i = 0, size = eventListeners.size(); i < size; ++i)
			{
				EventCallback* callback = eventListeners[i];
				(*callback)(topEvent);
			}

			delete topEvent;
		}
		else
		{
			// handle out of bounds access on event listeners
		}
	}
}

