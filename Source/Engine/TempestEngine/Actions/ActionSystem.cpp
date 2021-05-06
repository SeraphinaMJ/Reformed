/*!***************************************************************************************
\file       ActionSystem.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the ActionSystem class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "ActionSystem.hpp"
//========1st Party Includes============================================================//
#include "DelayAction.hpp"
#include "LerpAction.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

actionHandle actionSystem::enqueueLerpAction(float p_start_value, float p_end_value, float p_duration,
    std::function<void(float)> p_update_function, actionHandle p_parent_action)
{
    actionHandle l_handle = getNextHandle();
    action * l_action = new lerpAction<float>(l_handle, p_start_value, p_end_value, p_duration, p_update_function);
    actions.emplace(l_handle, l_action);

    if(p_parent_action != 0)
        dependencies.emplace(p_parent_action, l_handle);
    else   // if there is no parent, start it immediately
        l_action->start();

    return l_handle;
}

actionHandle actionSystem::enqueueDelayAction(float p_delay, std::function<void()> p_complete_function, actionHandle p_parent_action)
{
    actionHandle l_handle = getNextHandle();
    actions.emplace(l_handle,
        new delayAction(l_handle, p_delay, p_complete_function));

    if(p_parent_action != 0)
        dependencies.emplace(p_parent_action, l_handle);

    return l_handle;
}

void actionSystem::onUpdate()
{
    std::vector<actionHandle> l_remove_list;
    std::vector<actionHandle> l_child_start_list;

    for(auto const & l_action_pair : actions)
    {
        auto & l_action = l_action_pair.second;
        l_action->update(getDt());
        if(l_action->isComplete())
        {
            l_child_start_list.push_back(l_action->getHandle());
            l_remove_list.push_back(l_action->getHandle());
        }
    }

    for(auto & l_handle : l_remove_list)
    {
        removeAction(l_handle);
    }

    // children starting need to happen after all updates
    //   otherwise, they will process with the same dt value,
    //     which they should not accumulate
    for(auto & l_handle : l_child_start_list)
        startChildren(l_handle);
}

void actionSystem::onShutdown()
{
    dependencies.clear();
    while(!actions.empty())
    {
		removeAction(actions.begin()->first);
    }
}

void actionSystem::removeAction(actionHandle p_action_handle)
{
    action * l_action_ptr = actions[p_action_handle];
    actions.erase(p_action_handle);
    delete l_action_ptr;
}

void actionSystem::startChildren(actionHandle p_parent_handle)
{
    auto l_children = dependencies.equal_range(p_parent_handle);
    for(auto & l_iterator = l_children.first; l_iterator != l_children.second; ++l_iterator)
    {
        actions[l_iterator->second]->start();
    }

    dependencies.erase(p_parent_handle);
}

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

actionHandle actionSystem::getNextHandle()
{
    static unsigned int s_counter = 1; // leave zero for empty
    return ++s_counter;
}


/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


