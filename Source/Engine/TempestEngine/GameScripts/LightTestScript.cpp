/*!***************************************************************************************
\file       InputTestScript.cpp
\author     Cody Cannell
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the InputTestScript class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "LightTestScript.hpp"
//========1st Party Includes============================================================//
#include "../Components/InputComponent.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../Components/Renderer.hpp"
#include "../Components/Light.hpp"
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

lightTestScript::lightTestScript(typeRT p_data): scriptCPP(p_data)
{
}

void lightTestScript::onUpdate()
{
    // get the input component
    componentHandle<inputComponent> l_input = getGameObject().lock()->getComponent<inputComponent>();
    componentHandle<light> l_light = getGameObject().lock()->getComponent<light>();
	componentHandle<transform> l_transform = getGameObject().lock()->getComponent<transform>();

    if (l_input->getButtonState("directional"))
    {
        l_light->setLightType(lighttype::enm_directional);
    }
    if (l_input->getButtonState("point"))
    {
        l_light->setLightType(lighttype::enm_point);
    }
    if (l_input->getButtonState("spot"))
    {
        l_light->setLightType(lighttype::enm_spot);
    }

    // check states and update transform
    if (l_input->getButtonState("rotateZForward"))
    {
				l_transform->offsetRotZ(2.0f * dt());
        //l_light->setDirectionZ(2.0f * getDt());
    }
    if (l_input->getButtonState("rotateZBackward"))
    {
				l_transform->offsetRotZ(-2.0f * dt());
        //l_light->setDirectionZ(-2.0f * getDt());
    }
    if (l_input->getButtonState("rotateXForward"))
    {
				l_transform->offsetRotX(2.0f * dt());
        //l_light->setDirectionX(2.0f * getDt());
    }
    if (l_input->getButtonState("rotateXBackward"))
    {
				l_transform->offsetRotX(-2.0f * dt());
        //l_light->setDirectionX(-2.0f * getDt());
    }
    if (l_input->getButtonState("rotateYForward"))
    {
				l_transform->offsetRotY(2.0f * dt());
        //l_light->setDirectionY(2.0f * getDt());
    }
    if (l_input->getButtonState("rotateYBackward"))
    {
				l_transform->offsetRotY(-2.0f * dt());
        //l_light->setDirectionY(-2.0f * getDt());
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


