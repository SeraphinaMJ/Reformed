/*!***************************************************************************************
\file       RepeatUntilFailure.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node that continually activates its child until 
            it fails.
*****************************************************************************************/
#include "Decorator.hpp"
#ifndef TESTING_NODES

class RepeatUntilFailure : public Decorator
{
public:

    void Update(float)override;
};

#endif