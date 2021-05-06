/*!***************************************************************************************
\file       RepeatUntilN.cpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "RepeatUntilN.hpp"
#ifndef TESTING_NODES

const char maxSuccessStr[] = "maximumSuccesses";
const char runCounterStr[] = "RUN_counter";

const SpecialData RepeatUntilN::specialData =
{
    {{DATA_TYPE::INT, "maximumSuccesses", 0},
    {DATA_TYPE::UNSIGNED_INT, "RUN_counter", 0}}
};

void RepeatUntilN::updateFromFile(const rapidjson::Value & value)
{
    maximumSuccesses = value[maxSuccessStr].GetInt();
}

void RepeatUntilN::Init()
{
    // progressing and running, child is ready
    Decorator::Init();

    getBlackboard().setValue<unsigned>(runCounterStr, 0);
}

void RepeatUntilN::Update(float dt)
{
    auto task = getTask();
    unsigned int counter = getBlackboard().getValue<unsigned>(runCounterStr);


    // activate child behavior
    BehaviorResult childResult = task->GetResult();

    // if child succeeded then we are closer to success
    if (childResult == BehaviorResult::SUCCESS)
    {
        counter++;
        getBlackboard().setValue<unsigned>(runCounterStr, counter);
        if (counter == this->maximumSuccesses)
        {
            task->SetResult(BehaviorResult::SUCCESS);
            task->SetPhase(BehaviorPhase::DONE);
        }
        else
        {
            GiveToChild(task); // give task back to child again
        }
    }
    // if child failed then it means we could not complete a behavior N times
    else if (childResult == BehaviorResult::FAILURE)
    {
        task->SetResult(BehaviorResult::FAILURE);
        task->SetPhase(BehaviorPhase::DONE);
    }
}
#endif

