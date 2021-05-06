/*!***************************************************************************************
\file       NGE_Watchdog.hpp
\author     Charles Boudousquie
\date       10/23/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Checks to see if user changed something in inspector window that Node
            Graph Window would want to know about.
*****************************************************************************************/

#include <DataTypes.hpp>

// an object that watches out for special user changes that
// the Node Graph Editor must know about
class NGE_Watchdog
{
    bool inspectorChanged;

    objID changedActor;

    NGE_Watchdog() : inspectorChanged(false)
    {
    }
public:

    static NGE_Watchdog& GetInstance()
    {
        static NGE_Watchdog instance;
        return instance;
    }

    // in inspector window user decided to change
    // what tree an agent should be using
    bool inspectorChangedTree();

    // tell watchdog to look out for new changes
    void Reset();

    // alert watchdog that agent changed tree name
    void Alert(objID actor);

    // get agent that actually changed
    objID getChangedAgent();

};

