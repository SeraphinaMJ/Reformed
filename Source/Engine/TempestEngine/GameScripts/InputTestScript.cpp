/*!***************************************************************************************
\file       InputTestScript.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content ? 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputTestScript class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "InputTestScript.hpp"
#include "../Events/TestEvent.hpp"
//========1st Party Includes============================================================//
#include "../Components/InputComponent.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Camera.hpp"
#include <memory>
#include "../Components/RigidBody.hpp"
#include "../GamePadSystem.hpp"
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

inputTestScript::inputTestScript(typeRT p_data): scriptCPP(p_data)
{

	componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();

	auto l_tform = getGameObject().lock()->getComponent<transform>();

}

void inputTestScript::onUpdate()
{
    // get the input component
    componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();
    componentHandle<transform> l_tform = getGameObject().lock()->getComponent<transform>();
   // camera * l_comp = getGameObject().lock()->getComponent<camera>();

    componentHandle<rigidBody> l_rigidBody = getGameObject().lock()->getComponent<rigidBody>();
		// gamepadComponent* l_gamepad = getGameObject().lock()->getComponent<gamepadComponent>();

    float movementScale = 5.0f;
    float rotationScale = 5.0f;

    // check states and update transform
    if(l_input->getButtonState("backward"))
    {
        //l_tform->offsetPosZ(movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(0.0f, 0.0f, movementScale * dt()));
      //l_rigidBody->applyForce(vector3(0.0f, 0.0f, movementScale * getDt()));
    }
    if(l_input->getButtonState("forward"))
    {
        //l_tform->offsetPosZ(-movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(0.0f, 0.0f, -movementScale * dt()));
    }
    if(l_input->getButtonState("left"))
    {
        //l_tform->offsetPosX(-movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(-movementScale * dt(), 0.0f, 0.0f));
    }
    if(l_input->getButtonState("right"))
    {
        //l_tform->offsetPosX(movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(movementScale * dt(), 0.0f, 0.0f));
    }
    if(l_input->getButtonState("up"))
    {
        //l_tform->offsetPosY(movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(0.0f, movementScale * dt(), 0.0f));
    }
    if(l_input->getButtonState("down"))
    {
        //l_tform->offsetPosY(-movementScale * getDt());
        l_rigidBody->applyImpulse(vector3(0.0f, -movementScale * dt(), 0.0f));
    }


    if(l_input->getButtonState("scaleZUp"))
    {
        l_tform->offsetScaleZ(movementScale * dt());
    }
    if (l_input->getButtonState("scaleZDown"))
    {
        l_tform->offsetScaleZ(-movementScale * dt());
    }
    if (l_input->getButtonState("scaleXUp"))
    {
        l_tform->offsetScaleX(movementScale * dt());
    }
    if (l_input->getButtonState("scaleXDown"))
    {
        l_tform->offsetScaleX(-movementScale * dt());
    }
    if (l_input->getButtonState("scaleYUp"))
    {
        l_tform->offsetScaleY(movementScale * dt());
    }
    if (l_input->getButtonState("scaleYDown"))
    {
        l_tform->offsetScaleY(-movementScale * dt());
    }

   
    //if (l_input->getButtonState("play"))
    //{
    //    l_comp->play();
    //}
    //else
    //{
    //    //l_comp->stopPlaying();
    //}

    //if (l_input->getButtonState("record"))
    //{
    //    //l_comp->record();
    //}
    //else
    //{
    //    //l_comp->stopRecording();
    //}

    if(l_input->getButtonState("lmb"))
    {
		l_tform->offsetRotY(rotationScale * dt());
    }
    if(l_input->getButtonState("rmb"))
    {
        /*renderer * l_renderObj = getGameObject().lock()->getComponent<renderer>();
        l_renderObj->getMaterial("basic.mtl").setDiffuseColor(color::white);*/
		l_tform->offsetRotY(-rotationScale * dt());
    }


    auto l_pos = l_input->getCursorState("mouse");
    //logger("InputTestScript").info() << l_pos.first << ", " << l_pos.second;
    //getGameObject().lock()->getComponent<transform>()->printLocalData();


	if(l_input->getConnectionState("gamepadState") == false) //if controller disconnected, skip axis tests
	{
		return;
	}

	//axis test
	float moveX = l_input->getAxisState("moveX");
	float moveY = l_input->getAxisState("moveY");

	if(moveX < 0.4 || moveX > 0.6)
	{
		l_tform->offsetPosX((moveX - 0.5f) * 30.0f * dt());
	}
	if(moveY < 0.4 || moveY > 0.6)
	{
		l_tform->offsetPosZ(-(moveY - 0.5f) * 30.0f * dt());
	}
	gamepadSystem* gps = dynamic_cast<gamepadSystem*>(getSystemManager()->getSystem(gamepadSystem::getName()));
	float trig = l_input->getAxisState("trig");
	if(trig > 0.1)
	{
		gps->setFFB(0, trig, trig);
	}
	else
	{
		gps->setFFB(0, 0, 0);
	}

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

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


