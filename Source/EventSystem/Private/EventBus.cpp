/**
 * @file EventBus.cpp
 * @author Tyler Robbins
 *
 * @brief Defines all methods of EventSystem3::EventBus as defined in EventBus.h
 */

#include <iostream>

#include "EventBus.hpp"

/**
 * @brief Constructs the EventBus.
 */
EventSystem3::EventBus::EventBus(): m_event_queue(),
                                    m_handlers_map(),
                                    m_wait_list(),
                                    m_wait_mutex(),
                                    m_update_locked()
{
}

/**
 * @brief Gets the single instance of this EventBus.
 *
 * @return The single instance of this EventBus.
 */
EventSystem3::EventBus& EventSystem3::EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

/**
 * @brief Destroys this EventBus.
 * @details As EventBus is a singleton, in most circumstances this method will
 *          never be called and is simply here to showcase how an EventBus would
 *          be destroyed if it were not a singleton.
 */
EventSystem3::EventBus::~EventBus() {
    // Make sure that there are no events left in the queue.
    clear();
    for(auto& pair : m_handlers_map) {
        for(auto& fw : pair.second) {
            // Destroy ever function wrapper
            delete fw;
        }
        // TODO: Is this actually necessary?
        pair.second.clear();
    }
}

/**
 * @brief Clears the entire event queue.
 */
void EventSystem3::EventBus::clear() {
    while(!m_event_queue.empty()) {
        // Delete every EventBase from the event queue while removing them.
        EventBase* eb = m_event_queue.top();
        m_event_queue.pop();
        delete eb;
    }
}

/**
 * @brief The main update loop of the event bus which controls the entire logic
 *        of the bus.
 *
 * @param dt Change in time since the last update call.
 */
void EventSystem3::EventBus::update(float dt) {
    updateEventList();

    // Now we handle the waiting list, decreasing their wait times and moving
    //  them to the event queue if the time to wait is up.
    for(size_t i = 0; i < m_wait_list.size(); ++i) {
        if((std::get<1>(m_wait_list[i]) -= dt) <= 0.0f) {
            m_event_queue.push(std::get<0>(m_wait_list[i]));
            m_wait_list.erase(m_wait_list.begin() + i);
            --i;
        }
    }
}

/**
 * @brief Iterates over the entire list of events and updates each one.
 */
void EventSystem3::EventBus::updateEventList() {
    m_update_locked = true;

    // First we deal with every event in the queue.
    while(!m_event_queue.empty()) {
        EventBase* eb = m_event_queue.top();
        m_event_queue.pop();

        updateSingleEvent(eb);
    }

    m_update_locked = false;
}

/**
 * @brief Updates a single Event.
 *
 * @param eb A pointer to the event to update.
 */
void EventSystem3::EventBus::updateSingleEvent(EventBase* eb) {
    if(m_handlers_map.count(typeid(*eb)) > 0) {
        // Make sure to get a copy of it for thread safety
        HandlerContainer handlers = m_handlers_map.at(typeid(*eb));
        for(size_t i = 0; i < handlers.size(); ++i) {
            // Check if it is unsubscribed first, as with object handlers
            //  this could cause us to use an invalidated/deleted pointer.
            if(handlers[i]->unsubscribed) {
                delete handlers.at(i);
                m_handlers_map[typeid(*eb)].erase(m_handlers_map[typeid(*eb)].begin() + (i--));
                handlers = m_handlers_map[typeid(*eb)];
            } else {
                if(auto* d = eb->getDestination(); d == nullptr || d == handlers[i]->obj_ptr)
                {
                    handlers[i]->function(eb);
                }
            }

            // If the event gets snoozed, then it should not be handled by
            //  any further handlers.
            if(eb->shouldSnooze())
            {
                m_wait_list.push_back(std::make_tuple(eb, 0.0f));
                break;
            }
        }
    }

    // Don't delete the event if we are snoozing it, as we still want to
    //  use it again later
    if(!eb->shouldSnooze())
    {
        delete eb;
    }
    else
    {
        // Make sure that we mark it as not being snoozed anymore
        eb->setShouldSnooze(false);
    }
}

/**
 * @brief Gets the size of the event queue.
 *
 * @return The size of the event queue.
 */
size_t EventSystem3::EventBus::getEventQueueSize() const {
    return m_event_queue.size();
}

/**
 * @brief Gets the size of the waiting list.
 *
 * @return The size of the waiting list.
 */
size_t EventSystem3::EventBus::getWaitingEventListSize() const {
    return m_wait_list.size();
}

/**
 * @brief Gets if the event updater is locked.
 *
 * @return True if the event updater is locked, false otherwise.
 */
bool EventSystem3::EventBus::isEventUpdaterLocked() const {
    return m_update_locked;
}

/**
 * @brief Gets the number of event handlers for the given EventType represented
 *        as a <code>std::type_index</code>.
 *
 * @param idx The EventType to get all event handlers for.
 *
 * @return The number of event handlers for the given EventType.
 */
size_t EventSystem3::EventBus::getHandlersFor(std::type_index idx) const {
    return m_handlers_map.at(idx).size();
}

/**
 * @brief Subscribes the given function wrapper to the given type_index.
 *
 * @param idx The type_index to subscribe to
 * @param fw A pointer to the FunctionWrapper to subscribe.
 */
void EventSystem3::EventBus::subscribe(std::type_index idx, FunctionWrapper* fw)
{
    m_handlers_map[idx].push_back(fw);
    // Make sure the list of handlers stays sorted.
    std::sort(m_handlers_map[idx].begin(), m_handlers_map[idx].end(),
              std::greater<FunctionWrapper*>());
}

