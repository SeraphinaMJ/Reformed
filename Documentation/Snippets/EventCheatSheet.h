#pragma once
//Example of creating an event!!!

//Must include the eventSystem to inherit from the correct EventTemplate.

//MAKE SURE YOU DON'T ACCIDENTALLY USE THE AUDIO SYSTEM'S EVENT TEMPLATE!!!!!
#include "EventTemplate.hpp"
//or #include "Event/EventTemplate.hpp"

//Create a class with the name of the event and have it inherit from EventTemplate.
//Pass in the name of your class as the template parameter.
class exampleEvent : public EventTemplate<exampleEvent>
{
public:
	//You can take in whatever data you want here, construct the class like normal.
	//Typically everything is public as convention, but it doesn't have to be.
	exampleEvent(std::string p_message, int p_int) : m_message(p_message), m_int(p_int) {}
	std::string m_message;
	int m_int;
};


//This example class will inherit from systemBase, but this can also be done in scripts.
#include "../SystemBase.hpp"
class exampleClass : systemBase
{
	exampleClass() = default;

	//Example of registering a callback function!!!

	//This is a callback function that will be registered to the event system.
	//The function must be a member function that returns void and takes a pointer to an event.
	void functionToRegister(exampleEvent* p_event)
	{
		//Do something meaningful with the event once its thrown
		if(p_event->m_int)
		{
			return;
		}
		else
		{
			++p_event->m_int;
		}
	}

	void registrationExample()
	{
		/*
		You need to have access to the system manager to get the event system and register a callback function. Which means that you're a script or a system.
		Once you get the event system you call RegisterEventCallback. It takes the following parameters:

		template argument 1: The name of the class that will handle the event.
		template argument 2: The name of the event class that you want to handle.
		template argument 3: The member function you want to register. The syntax to get this is &myclass::function_i_want.
		function argument: This is a pointer to the specific object that will handle the event. Typically it's the "this" pointer.

		*/
		getSystemManager()->getSystem<eventSystem>()->RegisterEventCallback<exampleClass, exampleEvent, &exampleClass::functionToRegister>(this);


		/*
		If you're a script there is a slightly nicer helper function on the base script you can call instead.
		This function assumes you are using the "this" pointer. If you aren't you can still use the version above.
		It takes the same three template arguments, but doesn't need the object pointer, and doesn't require you to get the system manager/event system.
		*/
		
		//registerEventCallback<exampleClass, exampleEvent, &exampleClass::functionToRegister>();  This line wont compile because our example class isn't a script.
	}

	//Example of throwing/queuing an event!!!

	void throwEventFunction()
	{
		/*
		Get the event system, then throw the event.
		Call the function QueueEvent, and construct the event you are throwing.
		The event system will clean up the memory once the event is taken care of, but you have to use "new" when making the event.
		*/
		getSystemManager()->getSystem<eventSystem>()->QueueEvent(new exampleEvent("my test message", 5));
	}
};