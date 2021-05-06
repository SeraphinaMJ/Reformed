/*!***************************************************************************************
\file       EventBus.hpp
\author     Tyler Robbins
\date       10/04/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This file describes the main controller for the Event System.
@details In order to interact with the Event System, this file must be
         included. Usage of the EventSystem is described below.

@par Every loop of the application, the Event System's update loop must be
     called with <code>EVENT_BUS.update(dt);</code>, where <code>dt</code> is
     the change in time since the last update loop was called. If this is set
     to 0, then delayed events will never fire.

@par To fire an event, either the Event Bus's fire method can be called
     directly, or the EVENT_BUS_FIRE macro can be called instead.
@code
// Fire MyEvent using the fire method.
EVENT_BUS.fire(new MyEvent(), nullptr, false, EventSystem3::Priority::HIGH);
// Fire MyEvent using the EVENT_BUS_FIRE macro:
EVENT_BUS_FIRE(new MyEvent(), HIGH,);
@endcode

@par The EVENT_BUS_FIRE macro is recommended as it includes traceback
     information automatically and handles namespace resolution for priority
     specifiers. However, do not that when using the EVENT_BUS_FIRE macro,
     two commas are required, even if the final optional TIME parameter is
     ommitted.

@par To subscribe a handler to an event, it is recommended to use the
     SUBSCRIBE macro defined in inc/EventSubscribe.h, however should that
     macro be unable to work, the subscribe method can be used as such:
@code
// Assume a class MyEventHandler exists which inherits from EventHandler
// and has a handler method `void handle(MyEvent&);`
MyEventHandler myHandlerObj;
EVENT_BUS.Subscribe(&myHandlerObj, &MyEventHandler::handle, Priority::HIGH,
                    "handler");

// Assume a function myHandler exists of signature `void myHandler(MyEvent&);
EVENT_BUS.Subscribe(myHandler, Priority::HIGH, "myHandler");
@endcode

@par To unsubscribe a handler from an event, it is recommended to use the
     UNSUBSCRIBE macro defined in inc/EventSubscribe.h, however should that
     macro be unable to work, the unsubscribe method can be used as such:
@code
// Assume the previous code sample is still subscribed.
EVENT_BUS.Unsubscribe<MyEvent>(&myHandlerObj, "handler");
EVENT_BUS.Unsubscribe<MyEvent>("myHandler");
@endcode
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//

#include "EventBase.hpp" // EventBase
#include "EventFireError.hpp" // EventFireError
#include "EventHandler.hpp" // EventHandler
#include "EventPriority.hpp" // Priority

//======== 3rd Party Includes ==========================================================//

#include <iostream> // DEBUGGING
#include <algorithm> // std::sort
#include <queue> // std::priority_queue
#include <functional> // std::function
#include <vector> // std::vector
#include <map> // std::map
#include <typeinfo> // typeid
#include <typeindex> // std::type_index
#include <mutex> // std::mutex

#include "TypeTraits.hpp"

//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

namespace EventSystem3
{
    /**
     * @brief A wrapper around an event subscriber, containing information about
     *        the subscriber.
     */
    struct FunctionWrapper
    {
        //! The function being wrapped around.
        std::function<void(EventBase*)> function;
        //! Whether this function has been unsubscribed.
        bool unsubscribed = false;
        //! The priority of this event subscriber.
        Priority priority;
        //! The name of this event subscriber.
        std::string name;
        /**
         * @brief A pointer to the object that this subscriber is a method of
         *         if applicable.
         */
        const void* obj_ptr;
    };
}

namespace std
{
    /**
     * @brief An overload of greater for FunctionWrapper pointers.
     */
    template<>
    struct greater<EventSystem3::FunctionWrapper*>
    {
        /**
         * @brief Compares two FunctionWrapper* by priority.
         *
         * @param f1 The first FunctionWrapper to check.
         * @param f2 The second FunctionWrapper to check.
         *
         * @return True if f1 has a higher priority than f2, false otherwise.
         */
        bool operator()(const EventSystem3::FunctionWrapper* f1,
            const EventSystem3::FunctionWrapper* f2) const
        {
            return f1->priority > f2->priority;
        }
    };
}

namespace EventSystem3
{
    /**
     * @brief The main controller of the Event System.
     */
    class EventBus final
    {
        private:
            //! A container of handlers.
            using HandlerContainer = std::vector<FunctionWrapper*>;
            //! A mapping of types to a container for Handlers.
            using HandlerMap = std::map<std::type_index, HandlerContainer>;
            //! The data structure for holding the event queue.
            using EventQueue = std::priority_queue<EventBase*,
                std::vector<EventBase*>,
                std::greater<EventBase*>>;
        public:
            static EventBus& getInstance();

            ~EventBus();

            EventBus(const EventBus&) = delete;
            EventBus(EventBus&&) = delete;

            EventBus& operator=(const EventBus&) = delete;
            EventBus& operator=(EventBus&&) = delete;

            /**
             * @brief Fires an event.
             * @details If T extends any other event type, then a new event will
             *          be fired for each type down the hierarchy.
             *
             * @tparam T The event type to fire.
             * @param event Any type which extends EventBase at some point. Must
             *              be dynamically allocated with the <code>new</code>
             *              keyword.
             * @param destination The EventHandler object this event should be
             *                    fired at, or nullptr to fire at all available
             *                    handlers. Defaults to nullptr.
             * @param immediately Whether this event should be handled
             *                    immediately or not. Defaults to false.
             * @param priority The priority to fire the event with. Defaults to
             *                 NORMAL.
             * @param time The amount of time to wait before firing this event
             *             in seconds. Defaults to 0s.
             * @param func The name of the function this was called from. Used
             *             for debugging. Defaults to nothing.
             * @param line The line number this was called from. Used for
             *             debugging. Defaults to 0.
             */
            template<typename T>
            void fire(T* event, EventHandler* destination = nullptr,
                      bool immediately = false,
                      Priority priority = Priority::NORMAL, float time = 0.0f,
                      const std::string& func = "", unsigned long line = 0)
            {
                // Make sure that T extends from EventBase.
                static_assert(std::is_base_of<EventBase, T>::value,
                    "Cannot fire an object which does not extend "
                    "from EventBase.");

                // TODO: Is this needed?
                // Make sure T is not void
                if constexpr (!std::is_same<T, void>::value) {
                    // Do not fire a nullptr
                    if (event == nullptr)
                        throw EventFireError(EventFireError::Type::NULLPTR,
                                             priority, time);

                    event->setTracebackInformation(func, line);
                    event->setPriority(priority);
                    event->setDestination(destination);

                    // Ensure thread safety
                    if (m_update_locked || time != 0.0f) {
                        m_wait_mutex.lock();
                        m_wait_list.push_back(std::make_tuple(event, time));
                        m_wait_mutex.unlock();
                        return;
                    }

                    // TODO: Place thread safety stuff here.
                    if(immediately)
                    {
                        updateSingleEvent(event);
                    }
                    else
                    {
                        m_event_queue.push(event);
                    }

                    // If we haven't reached the bottom of the hierarchy, then
                    //  fire another event of parent_t
                    if constexpr (!std::is_same<typename T::parent_t, void>::value)
                    {
                        fire(new typename T::parent_t(*event), destination,
                             immediately, priority, time);
                    }
                }
            }

            void clear(); // DEBUGGING PURPOSES ONLY
            void update(float);

            /**
             * @brief Subscribes a function pointer to an event.
             *
             * @tparam E The event type to subscribe.
             * @param handler The function pointer to subscribe as a handler.
             * @param p The priority to give this handler.
             * @param name The name of the handler.
             */
            template<typename E>
            void subscribe(void(*handler)(E&), Priority p, const std::string& name)
            {
                // Add the function pointer to the map of handlers
                auto fw = new FunctionWrapper{
                    // The function stored is a light lambda wrapper around
                    //  the original function.
                    [handler](EventBase* event) {
                        handler(*reinterpret_cast<E*>(event));
                    }, false, p, name, 0
                };
                subscribe(typeid(E), fw);
            }

            /**
             * @brief Subscribes a function object to an event.
             *
             * @tparam E The event type to subscribe.
             * @param handler The function to subscribe as a handler.
             * @param p The priority to give this handler.
             * @param name The name of the handler.
             */
            template<typename E>
            void subscribe(std::function<void(E&)>& handler, Priority p,
                const std::string& name)
            {
                auto fw = new FunctionWrapper {
                    // The function stored is a light lambda wrapper around
                    //  the original function.
                    [handler](EventBase* event) {
                        handler(*reinterpret_cast<E*>(event));
                    }, false, p, name, 0
                };
                subscribe(typeid(E), fw);
            }

            /**
             * @brief Subscribes a method to an event.
             *
             * @tparam T The object type to subscribe
             * @tparam E The event type to subscribe to.
             *
             * @param obj The EventHandler object that owns the method.
             * @param handler The method to subscribe as a handler.
             * @param p The priority to give this handler.
             * @param name The name of the handler.
             */
            template<typename T, typename E>
            void subscribe(EventHandler* obj, void(T::*handler)(E&), Priority p,
                const std::string& name)
            {
                // TODO: Add static_assert check for inheritance on EventHandler and T
                FunctionWrapper* fw = new FunctionWrapper{
                         [obj, handler](EventBase* event) {
                             (static_cast<T*>(obj)->*(handler))(*reinterpret_cast<E*>(event));
                         },
                         false,
                         p,
                         name,
                         obj
                };
                subscribe(typeid(E), fw);
                obj->setFWrapper(fw);
            }

            /**
             * @brief Unsubscribes a named handler from an event.
             *
             * @tparam E The event type to unsubscribe from.
             * @param name The name of the handler to unsubscribe.
             */
            template<typename E>
            void unsubscribe(const std::string& name)
            {
                unsubscribe<E>(nullptr, name);
            }

            /**
             * @brief Unsubscribes a name method from an event.
             *
             * @tparam E The event type to unsubscribe from.
             * @param ptr The object which should be unsubscribed
             * @param name The name of the handler to unsubscribe.
             */
            template<typename E>
            void unsubscribe(void* ptr, const std::string& name)
            {
                // TODO: Add range checking.
                for (auto& fw : m_handlers_map[typeid(E)])
                {
                    if (fw->obj_ptr == ptr && fw->name == name)
                    {
                        fw->unsubscribed = true;
                    }
                }
            }

            size_t getEventQueueSize() const;
            size_t getWaitingEventListSize() const;
            bool isEventUpdaterLocked() const;
            size_t getHandlersFor(std::type_index) const;

            void subscribe(std::type_index, FunctionWrapper*);
        private:
            EventBus();

            void updateEventList();
            void updateSingleEvent(EventBase*);

            //! The queue of active events.
            EventQueue m_event_queue;

            //! A mapping of all subscribed handlers.
            HandlerMap m_handlers_map;

            // TODO: Priority?
            //! A list of all events waiting on timers.
            std::vector<std::tuple<EventBase*, float>> m_wait_list;

            //! A mutex to allow thread safety on the wait list.
            std::mutex m_wait_mutex;

            //! A boolean on whether event updating is locked
            bool m_update_locked;
    };
}

/**
 * @brief Returns the value given.
 * @details used for EVENT_BUS_FIRE.
 * @param v An integer value.
 * @return The value given.
 */
[[maybe_unused]] static int __0_if_no_value(int v) { return v; }
/**
 * @brief Returns 0
 * @details used for EVENT_BUS_FIRE.
 * @return 0
 */
[[maybe_unused]] static int __0_if_no_value() { return 0; }

/**
 * @brief A light wrapper around the single instance of the EventBus.
 */
#define EVENT_BUS EventSystem3::EventBus::getInstance()

 /**
  * @brief A wrapper around firing an event.
  * @details All 3 parameters must exist, but the TIME parameter can be left
  *          blank: <code>EVENT_BUS_FIRE(new EventBase(), HIGH,);</code>
  *
  * @param EVENT The event object to fire.
  * @param DEST The EventHandler* to fire the object at. May be left blank.
  * @param IMMEDIATE Whether the event should be handled immediately. May be
  *                  left blank.
  * @param PRIORITY The priority to fire. May be left blank.
  * @param TIME The delay on the event. May be left blank.
  */
#define EVENT_BUS_FIRE(EVENT, DEST, IMMEDIATE, PRIORITY, TIME)             \
    EVENT_BUS .fire(EVENT, __as_voidptr_if_no_value(DEST),                 \
                    __0_if_no_value(IMMEDIATE),                            \
                    EventSystem3::Priority:: CONCAT(ES3_PRI_, PRIORITY)(), \
                    __0_if_no_value(TIME), __FUNCTION__, __LINE__)

