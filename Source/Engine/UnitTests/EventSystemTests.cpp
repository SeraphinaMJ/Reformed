/*!***************************************************************************************
\file       EventSystemTests.cpp
\author     Tyler Robbins
\date       10/08/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "gtest/gtest.h"

#include "EventBase.hpp"
#include "EventBus.hpp"
#include "EventSubscribe.hpp"

struct MyEvent: public EventSystem3::EventBase {
    PARENT(EventSystem3::EventBase);

    MyEvent(int _a, float _b, double _c): a(_a), b(_b), c(_c) { }

    int a;
    float b;
    double c;
};

struct MyHandler: public EventSystem3::EventHandler {
    MyHandler(): EventSystem3::EventHandler("MyHandler") {
        EH_INITIALIZE();
    }

    void setCallback(std::function<void(MyEvent&)> callback) {
        m_callback = callback;
    }

    void handleMyEvent(MyEvent& e) {
        m_callback(e);
    }
    INCLASS SUBSCRIBE(MyEvent, &MyHandler::handleMyEvent, EHCLASS(MyHandler),);

    std::function<void(MyEvent&)> m_callback;
};

struct MyOtherHandler: public EventSystem3::EventHandler {
    MyOtherHandler(): EventSystem3::EventHandler("MyOtherHandler") {
        EH_INITIALIZE();
    }

    void setCallback(std::function<void(MyEvent&)> callback) {
        m_callback = callback;
    }

    void handleMyEvent(MyEvent& e) { m_callback(e); }
    INCLASS SUBSCRIBE(MyEvent, &MyOtherHandler::handleMyEvent, EHCLASS(MyOtherHandler),);

    std::function<void(MyEvent&)> m_callback;
};

TEST(EventSystemTests, EventBusSingleton) {
    ASSERT_EQ(&EventSystem3::EventBus::getInstance(), &EventSystem3::EventBus::getInstance()) << "EventBus::getInstance() does not equal itself.";
}

TEST(EventSystemTests, EventBusFire) {
    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1});

    ASSERT_GT(EVENT_BUS.getEventQueueSize(), 0) << "Event did not get fired.";

    EVENT_BUS.update(0.0f);

    ASSERT_EQ(EVENT_BUS.getEventQueueSize(), 0) << "Event was not cleared out of update queue.";
}

TEST(EventSystemTests, EventBusFireWait) {
    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1}, nullptr, false, EventSystem3::Priority::NORMAL, 1.0f);

    ASSERT_EQ(EVENT_BUS.getWaitingEventListSize(), 1) << "Event did not get fired into waiting list.";

    EVENT_BUS.update(1.0f);

    ASSERT_EQ(EVENT_BUS.getWaitingEventListSize(), 0) << "Event did not get fired into waiting list.";
}

TEST(EventSystemTests, EventHandlerHandleEvent) {
    bool l_called = false;
    {
        MyHandler handler;
        handler.setCallback([&](MyEvent&) {
            std::cout << "Handler got Event" << std::endl;
            l_called = true;
        });

        EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1});

        EVENT_BUS.update(0.0f);

        ASSERT_EQ(l_called, true) << "Event was never handled!";

        // Reset l_called
        l_called = false;
    }

    // now that handler got destructed, it should not get the event again

    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1});

    EVENT_BUS.update(0.0f);

    ASSERT_EQ(l_called, false) << "Event was handled again after destruction of the handler object!";
}

TEST(EventSystemTests, EventHandlerTargetEvent) {
    MyHandler handler;
    bool l_called1 = false;
    handler.setCallback([&](MyEvent&) {
            std::cout << "Handler1 got Event" << std::endl;
            l_called1 = true;
        });

    MyOtherHandler handler2;
    bool l_called2 = false;
    handler2.setCallback([&](MyEvent&) {
            std::cout << "Handler2 got Event" << std::endl;
            l_called2 = true;
        });

    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1}, &handler);

    EVENT_BUS.update(0.0f);

    ASSERT_EQ(l_called1, true) << "Event was never handled!";
    ASSERT_EQ(l_called2, false) << "Event was handled by a handler that was not targeted!";
}

TEST(EventSystemTests, EventSnoozed) {
    MyHandler handler;
    int times_called = 0;
    handler.setCallback([&](MyEvent& event) {
            std::cout << "Handler got event " << times_called << " times." << std::endl;
            event.setShouldSnooze(true);
            ++times_called;
        });
    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1});
    EVENT_BUS.update(0.0f);
    EVENT_BUS.update(0.0f);

    ASSERT_EQ(times_called, 2) << "Event was not handled 2 times!";
}

TEST(EventSystemTests, EventImmeidateHandling) {
    MyHandler handler;
    bool l_called = false;
    handler.setCallback([&](MyEvent&) {
        std::cout << "Handler got Event" << std::endl;
        l_called = true;
    });

    EVENT_BUS.fire(new MyEvent{5, 5.3f, 6.1}, nullptr, true);

    ASSERT_EQ(l_called, true) << "Event was never handled!";
}

