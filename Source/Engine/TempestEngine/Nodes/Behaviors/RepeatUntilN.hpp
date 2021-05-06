/*!***************************************************************************************
\file       RepeatUntilN.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This node fully processes its child exactly N times aka N SUCCESSes.
*****************************************************************************************/
#include "Decorator.hpp"

#ifndef TESTING_NODES

#include "BehaviorSystem/BehaviorSpecialData.hpp"

class RepeatUntilN : public Decorator
{
private:

    unsigned int maximumSuccesses;
public:

    const static SpecialData specialData;

    void updateFromFile(const rapidjson::Value &) override;

    void Init()override;
    void Update(float) override;
};
#endif
