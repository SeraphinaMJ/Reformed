/*!***************************************************************************************
\file       ComponentFactory.hpp
\author     David Roberts
\date       10/12/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Class that serializes components from typeRTs.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
//#include "SceneManagement/GameObject.hpp"
#include <Reflection.hpp>
class gameObject;
//========3rd Party Includes============================================================//
#include <string>
#include <memory> // std::shared_ptr
#include <vector> // std::vector
#include <ComponentTypeDetails.hpp>
#include "../EngineInterfaces/Public/IComponentRepository.hpp"

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: componentFactory
\brief   Class that serializes components from typeRTs.
*****************************************************************************************/
class componentFactory
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	static const std::vector<componentTypeDetails> & getComponentTypeDetails();
	static const std::vector<componentTypeDetails> & getScriptTypeDetails();
	static void deserializeComponents(std::shared_ptr<gameObject> p_object, typeRT & p_data);
	static void addComponentByType(std::shared_ptr<gameObject> p_object, const std::string & p_type);
	static void removeComponentByType(std::shared_ptr<gameObject> & p_object, const std::string & p_type);
	static const componentTypeDetails & getComponentTypeDetails(const std::string & p_name);
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

////////========================================================================////////
////////   Private                                                              ////////
////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};


