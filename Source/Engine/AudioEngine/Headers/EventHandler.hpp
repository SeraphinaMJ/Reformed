/*!***************************************************************************************
\file       EventHandler.hpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include <vector>
#include <queue>

namespace Audio {
	// =================================================
	//                    TO DO:
	// =================================================

	// - Make sure this is thread safe

	typedef unsigned short EventID;

	class EventBase
	{
	public:
        virtual ~EventBase() = default;
		static EventID numTypes;
		virtual EventID GetTypeID() const = 0;
	};

	template <typename EventType>
	class EventTemplate : public EventBase
	{
	public:
		EventID GetTypeID() const override
		{
			return TypeID;
		}

		static const EventID TypeID;
	};

	class EventCallback
	{
	public:
		template <typename classType>
		EventCallback(classType* instance, EventID TypeID) : EventTypeID(TypeID), classInstance(instance)
		{
		}

		EventID GetEventTypeID() { return EventTypeID; }

		virtual void operator()(EventBase* InputEvent) = 0;

	private:
		EventID EventTypeID;
		void* classInstance;
	};

	template <typename EventType, typename ClassType>
	class EventCallbackTemplate : public EventCallback
	{
	public:

		EventCallbackTemplate(EventID TypeID, ClassType* ClassInstance, void(ClassType::*MemberFunc)(EventType*))
			: EventCallback(ClassInstance, TypeID), ClassInstance(ClassInstance), MemberFunc(MemberFunc)
		{
			// Intentionally left blank
		}

		EventID GetEventID() { return GetEventTypeID(); }

		void operator()(EventBase* InputEvent) override
		{
			EventType* OutputEvent = reinterpret_cast<EventType*>(InputEvent);
			(ClassInstance->*MemberFunc)(OutputEvent);
		}

	private:

		ClassType* ClassInstance;
		void(ClassType::*MemberFunc)(EventType*);
	};

	class EventManager
	{
	public:

		EventManager();

		void ParseAllEvents();

		void StartParsing() { runningFlag = true; }
		void StopParsing() { runningFlag = false; }

		template <typename ClassType, typename EventType, void (ClassType::*Function)(EventType*)>
		void RegisterEventCallback(ClassType* ObjectInstance);

		void QueueEvent(EventBase* incomingEvent);

	private:

		bool runningFlag;
		std::queue<EventBase*> EventQueue;
		//std::vector<EventCallback*> Listeners;
		std::vector<std::vector<EventCallback*>> Listeners;
	};

	// Note: if there was a previous event callback for a specific event type this will overwrite the old listener
	template <typename ClassType, typename EventType, void (ClassType::*Function)(EventType*)>
	void EventManager::RegisterEventCallback(ClassType * ObjectInstance)
	{
		EventCallback* newCallback = new EventCallbackTemplate<EventType, ClassType>(EventType::TypeID, ObjectInstance, Function);

		EventID CallbackType = newCallback->GetEventTypeID();

		if (CallbackType + (unsigned)1 >= Listeners.size())
		{
			Listeners.resize(CallbackType + 1);
			//Listeners[CallbackType].push_back();
		}


		std::vector<EventCallback*>& callbackTypeListeners = Listeners[CallbackType];
		callbackTypeListeners.push_back(newCallback);

		//EventCallback* oldListener = Listeners[CallbackType];
		//if (oldListener)
		//	delete oldListener;

		//Listeners[CallbackType] = newCallback;
	}

	template <typename T> const EventID EventTemplate<T>::TypeID = EventBase::numTypes++;
}

