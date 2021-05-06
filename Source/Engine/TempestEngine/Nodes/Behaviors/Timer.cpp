/*!***************************************************************************************
\file       Timer.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node is a simple Decorator timer that activates its child node when
            timer hits 0.
*****************************************************************************************/
#include "Timer.hpp"
#ifndef TESTING_NODES

const char maxTimeStr[] = "Max Time";

const SpecialData Timer::specialData=
{
    {SpecialData::Entry{DATA_TYPE::FLOAT, "Max Time", -1.0f}}
};

void Timer::updateFromFile(const rapidjson::Value & value)
{
    this->maxTime = value[maxTimeStr].GetFloat();
}

void Timer::Init()
{
    InitDebug();

    // do not call decorator's init, since we do not give task to child immediately

    // reset timer
    getBlackboard().setValue<float>("currentTime", 0);

    // call update next frame
    getTask()->SetPhase(BehaviorPhase::PROGRESSING);
}

void Timer::Update(float dt)
{
    UpdateDebug();

    float currentTime = getBlackboard().getValue<float>("currentTime");
    currentTime += dt;
    getBlackboard().setValue<float>("currentTime", currentTime);

    // if timer finished then activate child node
    if (currentTime >= this->maxTime)
    {
        // we will be done when the child hands back the task to us
        getTask()->SetPhase(BehaviorPhase::DONE);

        // send task to child so child can take care of it
        GiveToChild(getTask());
    }
}

#endif
