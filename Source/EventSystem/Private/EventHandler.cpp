/**
 * @file EventHandler.cpp
 * @author Tyler Robbins
 *
 * @brief Defines all methods in EventHandler.h
 */

#include "EventHandler.hpp"
#include "EventBus.hpp" // EVENT_BUS

#include <iostream> // std::cerr, std::endl

#include "Logger.hpp"

/**
 * @brief Constructs an EventHandler object.
 *
 * @param type_name The name of the type that extends this class.
 */
EventSystem3::EventHandler::EventHandler(const std::string& type_name):
    m_type_name(type_name)
{
}

/**
 * @brief Initializes the EventHandler after the child class's constructor has
 *        finished. Calling multiple times does nothing.
 */
void EventSystem3::EventHandler::init()
{
    // Don't re-run if we have already been initialized.
    if(!m_initialized) {
        // First check if we even have any subscription initializer functions
        //  associated with us.
        if(getInitSubscriptions().count(typeid(*this)) != 0) {
            // Run every subscription initializer function
            for(const auto& f : getInitSubscriptions()[typeid(*this)]) {
                f(this);
            }
        }
        m_initialized = true;
    }
}

/**
 * @brief Destroys an EventHandler object.
 */
EventSystem3::EventHandler::~EventHandler()
{
    // Mark ever FunctionWrapper we have as unsubscribed.
    for(auto& wrapper : m_fwrappers) {
        if(wrapper != nullptr) {
            wrapper->unsubscribed = true;
        }
    }
}

/**
 * @brief Gets the type name.
 *
 * @return The type name.
 */
const std::string& EventSystem3::EventHandler::getTypeName() const
{
    return m_type_name;
}

/**
 * @brief Gets the subscription initializer function mapping.
 *        Written like a singleton class's GetInstance function to get around
 *        the Static Initialization Order Fiasco problem.
 *
 * @return The subscription initializer function mapping.
 */
std::map<std::type_index, std::vector<EventSystem3::EventHandler::InitSubscription>>&
    EventSystem3::EventHandler::getInitSubscriptions()
{
    static std::map<std::type_index, std::vector<InitSubscription>> s_init_subscriptions;

    return s_init_subscriptions;
}

/**
 * @brief Adds a FunctionWrapper to this object.
 *
 * @param fw The FunctionWrapper object to add.
 */
void EventSystem3::EventHandler::setFWrapper(EventSystem3::FunctionWrapper* fw)
{
    if(fw == nullptr) { // TODO: Throw an exception here.
        logger("EventSystem").error() << "NULL FunctionWrapper GIVEN!" << std::endl;
        return;
    }
    m_fwrappers.push_back(fw);
}

