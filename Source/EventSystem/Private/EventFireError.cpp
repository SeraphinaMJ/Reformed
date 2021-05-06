/**
 * @file EventFireError.cpp
 * @author Tyler Robbins
 *
 * @brief Defines all methods of EventSystem3::EventFireError as defined in
 *        EventFireError.h
 */

#include "EventFireError.hpp"

#include <string> // std::to_string, std::string

/**
 * @brief Constructs an EventFireError.
 *
 * @param e_type The error type of this error.
 * @param priority The priority of the event that errored.
 * @param time The delay that was on the event which errored.
 */
EventSystem3::EventFireError::EventFireError(Type e_type,
                                             Priority priority,
                                             float time):
    m_err_type(e_type),
    m_priority(priority),
    m_time(time)
{
}

/**
 * @brief Gets the error type.
 *
 * @return The error type.
 */
EventSystem3::EventFireError::Type EventSystem3::EventFireError::getType() const
{
    return m_err_type;
}

/**
 * @brief Gets the event priority which errored.
 *
 * @return The erroneous event's priority.
 */
EventSystem3::Priority EventSystem3::EventFireError::getPriority() const {
    return m_priority;
}

/**
 * @brief Gets the event delay which errored.
 *
 * @return The erroneous event's delay.
 */
float EventSystem3::EventFireError::getTime() const {
    return m_time;
}

/**
 * @brief Gets the error message.
 *
 * @return The error message.
 */
const char* EventSystem3::EventFireError::what() const noexcept {
    return (std::string("Event Fire: [") +
                    std::to_string(static_cast<int>(m_err_type)) + "|" +
                    std::to_string(static_cast<int>(m_priority)) + "|" +
                    std::to_string(m_time)).c_str();
}

