/*!****************************************************************************
@file EventBase.hpp
@author Tyler Robbins
\date       10/04/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief This file describes the base class from which all Events must derive.

@par Usage
To define a custom event, a class or structure must be made that extends
     EventBase like so:
@code
struct MyEvent: public EventBase {
    PARENT(EventBase); // Optional
};
@endcode
@par
Any additional information may be placed in <code>MyEvent</code>, however it
is not required. The use of PARENT(BaseEvent) is optional, but recommended,
as it is required for the event system to allow polymorphic events, for
example:
@code
struct WindowEvent: public EventBase {
    PARENT(EventBase);
};
struct WindowMoveEvent: public WindowEvent {
    PARENT(WindowEvent);
};
@endcode
The above code allows a <code>WindowMoveEvent</code> to be fired, and have
all <code>WindowEvent</code> and <code>EventBase</code> subscribers to be
called as well.

@par All content © 2018 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

#include <string> // std::string

namespace EventSystem3
{
    enum class Priority;

    class EventHandler;

    /**
     * @brief The class from which all custom Event types must derive from.
     */
    class EventBase
    {
        public:
            /**
             * @brief The parent type of this class, is void by default and can
             *        be overridden by derived classes.
             */
            using parent_t = void;

            EventBase();
            EventBase(EventBase&);
            EventBase(EventBase&&);
            virtual ~EventBase();

            Priority getPriority() const;
            EventHandler* getDestination() const;

            void setTracebackInformation(const std::string&, unsigned long);
            void setPriority(Priority);
            void setDestination(EventHandler*);

            const std::string& getFunction() const;
            unsigned long getLine() const;

            bool shouldSnooze() const;
            void setShouldSnooze(bool);

        private:
            //! The priority of this event.
            Priority m_priority;

            //! The function this event was fired from.
            std::string m_func;

            //! The line number this event was fired from.
            unsigned long m_line;

            //! The EventHandler this event should be sent to
            EventHandler* m_destination;

            //! Whether or not this event should be snoozed
            bool m_snooze;
    };
}

/**
 * @brief A helper macro wrapping around the specifics on how parent usages
 *        work.
 */
#define PARENT(P) using parent_t = P

