/*!****************************************************************************
@file EventHandler.h
@author Tyler Robbins

@brief Defines the base class from which all Event Handler object types must
       inherit.

@details This class enables the ability to tie event handlers to objects. To
         do so, one needs to first define their own custom class which
         inherits from <code>EventSystem3::EventHandler</code>. The simplest
         case requires only the extension, and a constructor which calls the
         base class's constructor. To subscribe a method tied to an object,
         simply use the normal subscription tools, with the following
         differences:
 - When passing in the function itself, it must be done as
   <code>&HandlerType::HandlerFunc</code>.
 - An object must be passed in as the third/first parameter (The placement
   depends on the exact method used: For the <code>SUBSCRIBE</code> macro, it
   is the 3rd parameter, whereas for <code>EVENT_BUS.Subscribe</code> it is
   the first). The object can be one of two things:
   + The <code>this</code> pointer or <code>&someObject</code>.
     * Subscribes the method for just the given object pointer.
   + <code>EHCLASS(HandlerType)</code>
     * Subscribes the method for the HandlerType, so that all objects created
       from it will have the method subscribed for them.

@par Note:
All examples will be using the <code>SUBSCRIBE</code> macro defined in
 EventSubscribe.h. For information on using <code>EVENT_BUS.Subscribe</code>,
 see EventBus.h.

@par Usage:
@code
class MyEventHandler: public EventSystem3::EventHandler {
    public:
        MyEventHandler(): EventSystem3::EventHandler(this) {
            SUBSCRIBE(MyEvent, &MyEventHandler::myOtherHandler, this,);

            // This is only required if the 'INCLASS SUBSCRIBE' feature has
            //  been used.
            EH_INITIALIZE();
        }

        void myOtherHandler(MyEvent&);

        // This is the simplest way to subscribe a method to an event for all
        //  objects created from MyEventHandler.
        void handler(MyEvent&);
        INCLASS SUBSCRIBE(MyEvent, &MyEventHandler::handler, EHCLASS(MyEventHandler),);
};
@endcode

@par All content © 2018 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include <typeinfo> // typeid
#include <string> // std::string
#include <vector> // std::vector
#include <functional> // std::function
#include <typeindex> // std::type_index
#include <map> // std::map

namespace EventSystem3
{
    struct FunctionWrapper;

    enum class Priority;

    /**
     * @brief The base Event Handler class from which all Event Handler object
     *        types must inherit from.
     */
    class EventHandler
    {
        public:
            //! An alias for a subscription initialization function.
            using InitSubscription = std::function<void(EventHandler*)>;

            EventHandler(const std::string&);

            /**
             * @brief Initializes EventHandler with an automatically determined
             *        name of this type.
             */
            template<typename T>
            EventHandler(T*) :
                EventHandler(std::string(typeid(T).name()))
            {}

            virtual ~EventHandler();

            void init();

            const std::string& getTypeName() const;

            /**
             * @brief Adds a subscription initializatino function for type T.
             *
             * @tparam T The type to add the subscription initialization
             *           function to.
             *
             * @param func The subscription initialization function.
             */
            template<typename T>
            static void addInitSubscribe(T*, InitSubscription func)
            {
                getInitSubscriptions()[typeid(T)].push_back(func);
            }

            void setFWrapper(FunctionWrapper*);

        protected:
            static std::map<std::type_index, std::vector<InitSubscription>>& getInitSubscriptions();

        private:
            //! The name of the child class.
            std::string m_type_name;

            //! All function wrappers belonging to this object.
            std::vector<FunctionWrapper*> m_fwrappers;

            //! True if this object has been initialized, false otherwise.
            bool m_initialized = false;
    };
}

/**
 * @brief Initializes the Event Handler.
 */
#define EH_INITIALIZE() EventHandler::init();

