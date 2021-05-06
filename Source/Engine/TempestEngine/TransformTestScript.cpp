/*!***************************************************************************************
\file       TransformTestScript.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the TransformTestScript class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "TransformTestScript.hpp"
//========1st Party Includes============================================================//
#include "SceneManagement/GameObject.hpp"
#include "SceneManagement/Transform.hpp"
#include "SystemManagement/SystemManagerInterface.hpp"
#include "Actions/ActionSystem.hpp"
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

transformTestScript::transformTestScript(typeRT p_data) : scriptCPP(p_data)
{
}

void transformTestScript::onStart()
{
    auto l_action_system = getSystemManager()->getSystem<actionSystem>();

    // create the actions to perform the transformation
  /*  auto l_x_pos_offset_handle =  l_action_system->enqueueLerpAction(
        0.0f, 3.0f, 1.0f,
        [&](float p_xpos) { getGameObject().lock()->getComponent<transform>()->setPosX(p_xpos); });*/

    auto l_x_rot_offset_handle = l_action_system->enqueueLerpAction(
            0.0f, 2.0f * 3.14f, 10.0f, // 360 degree rotation
            [&](float p_xrot)
    {
			(void)p_xrot;
			getGameObject().lock()->getComponent<transform>()->setRotY(p_xrot);
    });

    //auto l_x_neg_offset_handle = l_action_system->enqueueLerpAction(
    //    3.0f, 0.0f, 1.0f,
    //    [&](float p_xpos) { getGameObject().lock()->getComponent<transform>()->setPosX(p_xpos); },
    //    l_x_pos_offset_handle);
    //auto l_y_pos_offset_handle = l_action_system->enqueueLerpAction(
    //    0.0f, 3.0f, 1.0f,
    //    [&](float p_ypos) { getGameObject().lock()->getComponent<transform>()->setPosY(p_ypos); },
    //    l_x_neg_offset_handle);
    //auto l_y_neg_offset_handle = l_action_system->enqueueLerpAction(
    //    3.0f, 0.0f, 1.0f,
    //    [&](float p_ypos) { getGameObject().lock()->getComponent<transform>()->setPosY(p_ypos); },
    //    l_y_pos_offset_handle);
    //auto l_z_pos_offset_handle = l_action_system->enqueueLerpAction(
    //    0.0f, 3.0f, 1.0f,
    //    [&](float p_zpos) { getGameObject().lock()->getComponent<transform>()->setPosZ(p_zpos); },
    //    l_y_neg_offset_handle);
    //auto l_z_neg_offset_handle = l_action_system->enqueueLerpAction(
    //    3.0f, 0.0f, 1.0f,
    //    [&](float p_zpos) { getGameObject().lock()->getComponent<transform>()->setPosZ(p_zpos); },
    //    l_z_pos_offset_handle);
    //auto l_x_rot_offset_handle = l_action_system->enqueueLerpAction(
    //    0.0f, 1.0472f, 10.0f, // 60 degree rotation
    //    [&](float p_xrot) { getGameObject().lock()->getComponent<transform>()->offsetRotX(p_xrot); },
    //    l_z_neg_offset_handle);
    /*auto x_complete_handle =*/ l_action_system->enqueueDelayAction(
        0.0f,
        [&]() { onStart(); },
        l_x_rot_offset_handle);

}

void transformTestScript::onUpdate()
{
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

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


