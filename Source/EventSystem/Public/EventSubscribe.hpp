/*!****************************************************************************
@file EventSubscribe.h
@author Tyler Robbins

@brief Defines helpers for managing the subscription of handlers to events.

@par All content © 2018 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include "EventBus.hpp" // EVENT_BUS
#include "EventPriority.hpp" // Priority

#include "Preprocessor.hpp"
#include "TypeTraits.hpp"

namespace EventSystem3 {
    /**
     * @brief Simple base class to allow storing Class structures in a container.
     */
    struct IClass
    {};

    /**
     * @brief Wrapper around a type.
     */
    template<typename T>
    struct Class : public IClass
    {
        //! The type.
        using type = T;

        /**
         * @brief Creates a NULL Class<T> object.
         *
         * @return A NULL Class<T> object.
         */
        static Class<T>* Get()
        {
            return reinterpret_cast<Class<T>*>(0);
        }
    };

    /**
     * @brief A class to enable the pre-initialization ability to subscribe
     *        functions/methods to events at the very start of the program.
     *
     * @tparam E The event type to subscribe to.
     * @tparam F The function/method type to subscribe.
     * @tparam O A type extending EventHandler. If O is void, then F is treated
     *           as a function. If O is of type Class<T>, where T is the type
     *           extending EventHandler, then the function is subscribed for all
     *           objects constructed from T.
     */
    template<typename E, typename F, typename O = void>
    struct Subscriber final
    {
        /**
         * @brief Subscribes a function with a given name to an event.
         *
         * @param func The function/method to subscribe.
         * @param name A unique identifier to give the function. Used to search
         *             for the subscribed function.
         * @param obj The object func is a method of, if applicable. Ignored if
         *            O is void.
         * @param priority The priority with which this function should be
         *                 called as opposed to other functions. Defautls to
         *                 NORMAL.
         */
        Subscriber(F func, const std::string& name, O* obj,
            Priority priority = Priority::NORMAL)
        {
            (void)(obj); // Potentially unused parameter
            // If O is not void, then we have a method
            if constexpr (!std::is_same<O, void>::value)
            {
                // If O is of base IClass, then we add an initial subscription
                //  function to its type so that all objects constructed from
                //  it get func subscribed for them
                if constexpr (std::is_base_of<IClass, O>::value)
                {
                    // Add an initial subscription function.
                    O::type::addInitSubscribe(reinterpret_cast<typename O::type*>(0),
                        [func, priority, name](EventHandler* eh)
                    {
                        EVENT_BUS.subscribe<typename O::type>(dynamic_cast<typename O::type*>(eh),
                            func, priority,
                            name);
                    });
                }
                else
                {
                    // If O is just a normal type, then we subscribe func for
                    //  just the given object.
                    EVENT_BUS.subscribe<O>(obj, func, priority, name);
                }
            }
            else
            {
                // If O is void, then we are subscribing a function, not a
                //  method.
                EVENT_BUS.subscribe(func, priority, name);
            }
        }
    };

    /**
     * @brief A class to enable the pre-initialization ability to unsubscribe
     *        functions/methods from events at the very start of the program.
     *
     * @tparam E The event type to unsubscribe from.
     * @tparam O A type extending EventHandler. If O is void, then only the pool
     *           of function subscriptions is searched. Otherwise, the given
     *           object is searched.
     */
    template<typename E, typename O = void>
    struct UnSubscriber final
    {
        /**
         * @brief Unsubscribes a function/method of name.
         *
         * @param name The name to search for.
         * @param obj The object to unsubscribe the method for. Ignored if O is
         *            void.
         */
        UnSubscriber(const std::string& name, O* obj)
        {
            if constexpr (!std::is_same<O, void>::value)
            {
                EVENT_BUS.unsubscribe<E>(obj, name);
            }
            else
            {
                (void)obj; // Unused in this case
                EVENT_BUS.unsubscribe<E>(name);
            }
        }
    };
}

/**********\
 * MACROS *
\**********/

/**
 * @brief Expands to a unique name for use by the Event system.
 * @param PREFIX A prefix to append the unique name to
 */
#define ES3_UNIQUE_NAME(PREFIX) CONCAT(CONCAT(CONCAT(PREFIX, _L),__LINE__), \
                                CONCAT(_C,__COUNTER__))

/**
 * @brief Expands to a Subscriber type. A nicety to make the subsequent macros
 *        nicer to read.
 *
 * @param EVENT The event type this Subscriber is being defined for.
 * @param FUNC The function this Subscriber is being defined for.
 * @param OBJ The object this Subscriber is being defined for (can be blank).
 */
#define ES3_SUBSCR_TYPE(EVENT, FUNC, OBJ)                       \
    EventSystem3::Subscriber<EVENT, decltype(FUNC),             \
                             std::remove_pointer<decltype(::__void_if_no_value(OBJ))>::type>

/**
 * @brief Expands to a UnSubscriber type. A nicety to make the subsequent macros
 *        nicer to read.
 *
 * @param EVENT The event type this UnSubscriber is being defined for.
 * @param OBJ The object this UnSubscriber is being defined for (can be blank).
 */
#define ES3_UNSUBSCR_TYPE(EVENT, OBJ)                       \
    EventSystem3::UnSubscriber<EVENT,                       \
                               std::remove_pointer<decltype(__void_if_no_value(OBJ))>::type>

/**
 * @brief Subscribes a function or method to an event.
 * @details
 *
 * @param EVENT The event to subscribe to.
 * @param FUNC The function to subscribe.
 * @param OBJ The object of a type which extends EventHandler that the function
 *            belongs to (if applicable, can be left blank otherwise and
 *            defaults to nullptr).
 * @param PRIORITY The priority this function should have (if applicable, can be
 *                 left blank otherwise, and defaults to NORMAL).
 */
#define SUBSCRIBE(EVENT, FUNC, OBJ, PRIORITY)                                  \
    static ES3_SUBSCR_TYPE(EVENT, FUNC, OBJ) ES3_UNIQUE_NAME(SUBSCRIBED) =         \
        ES3_SUBSCR_TYPE(EVENT, FUNC, OBJ) (FUNC, "EH_" STR(EVENT) "_" STR(FUNC), \
                                           __as_voidptr_if_no_value(OBJ),       \
                                           EventSystem3::Priority:: CONCAT(ES3_PRI_, PRIORITY)())
       //! An alias to make the SUBSCRIBE macro
#define INCLASS inline

/**
 * @brief Creates a Class<T> from TYPE.
 *
 * @param TYPE The type to create a Class<T> from.
 */
#define EHCLASS(TYPE) *EventSystem3::Class<TYPE>::Get()

/**
 * @brief Unsubscribes FUNC from EVENT.
 *
 * @param EVENT The event to unsubscribe from
 * @param FUNC The function/method name to unsubscribe (must match how it was
 *             passed in for the SUBSCRIBE macro).
 * @param OBJ The object that FUNC is a method of. Can be left blank, defaults
 *            to nullptr.
 */
#define UNSUBSCRIBE(EVENT, FUNC, OBJ)                                  \
    static ES3_UNSUBSCR_TYPE(EVENT, OBJ) ES3_UNIQUE_NAME(UNSUBSCRIBED) =   \
        ES3_UNSUBSCR_TYPE(EVENT, OBJ) ("EH_" STR(EVENT) "_" STR(FUNC), \
                                __as_voidptr_if_no_value(OBJ))

