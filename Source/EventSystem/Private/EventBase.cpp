/**
 * @file EventBase.cpp
 * @author Tyler Robbins
 *
 * @brief Defines all methods for EventSystem3::EventBase as described in
 *        EventBase.h.
 */

#include "EventBase.hpp"
#include "EventBus.hpp"

/**
 * @brief Constructs this event with NORMAL priority.
 */
EventSystem3::EventBase::EventBase(): m_priority(Priority::NORMAL),
                                      m_func("<INVALID FUNCTION NAME>"),
                                      m_line(0), m_destination(nullptr),
                                      m_snooze(false)
{
}

/**
 * @brief Copies data from another event.
 */
EventSystem3::EventBase::EventBase(EventBase& e): m_priority(e.m_priority),
                                                  m_func(e.m_func),
                                                  m_line(e.m_line),
                                                  m_destination(e.m_destination),
                                                  m_snooze(e.m_snooze)
{
}

/**
 * @brief Moves data from another event to this one.
 *
 * @param other The other event to copy from.
 */
EventSystem3::EventBase::EventBase(EventBase&& other):
    m_priority(std::move(other.m_priority)), m_func(std::move(other.m_func)),
    m_line(std::move(other.m_line)),
    m_destination(std::move(other.m_destination)),
    m_snooze(std::move(other.m_snooze))
{
}

/**
 * @brief Destroys this event.
 */
EventSystem3::EventBase::~EventBase() { }

/**
 * @brief Gets the priority of this event.
 *
 * @return This event's priority.
 */
EventSystem3::Priority EventSystem3::EventBase::getPriority() const {
    return m_priority;
}

/**
 * @brief Sets the priority of this event.
 *
 * @param priority The priority to set this event's priority to.
 */
void EventSystem3::EventBase::setPriority(EventSystem3::Priority priority)
{
    m_priority = priority;
}

/**
 * @brief Sets the destination of this event.
 *
 * @param dest The destination for this event.
 */
void EventSystem3::EventBase::setDestination(EventSystem3::EventHandler* dest)
{
    m_destination = dest;
}

/**
 * @brief Gets the destination of this event.
 *
 * @return This event's destinatiok
 */
EventSystem3::EventHandler* EventSystem3::EventBase::getDestination() const
{
    return m_destination;
}

/**
 * @brief Sets the traceback information of this event.
 *
 * @param func The name of the function this event was fired from.
 * @param line The line number this event was fired on.
 */
void EventSystem3::EventBase::setTracebackInformation(const std::string& func,
                                                      unsigned long line)
{
    if(!func.empty()) m_func = func;
    if(line > 0) m_line = line;
}

/**
 * @brief Gets the function name this event was fired from.
 *
 * @return The name of the function this event was fired from.
 */
const std::string& EventSystem3::EventBase::getFunction() const {
    return m_func;
}

/**
 * @brief Gets the line nubmer this event was fired on.
 *
 * @return The line number this event was fired on.
 */
unsigned long EventSystem3::EventBase::getLine() const {
    return m_line;
}


/**
 * @brief Gets if this event should be snoozed.
 *
 * @return If this event should be snoozed.
 */
bool EventSystem3::EventBase::shouldSnooze() const {
    return m_snooze;
}


/**
 * @brief Sets if this event should be snoozed.
 *
 * @param snooze true if this event should be snoozed, false otherwise.
 */
void EventSystem3::EventBase::setShouldSnooze(bool snooze) {
    m_snooze = snooze;
}

