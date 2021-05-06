/*!***************************************************************************************
\file       ComponentFactory.cpp
\author     David Roberts
\date       10/12/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Class that serializes components from typeRTs.
*****************************************************************************************/

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "ComponentFactory.hpp"
#include "ComponentLists.h"
#include "SceneManagement/Transform.hpp"



//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//

		//==============================================================================//
		//        Destructor                                                            //
		//==============================================================================//

		//==============================================================================//
		//        Getters & Setters                                                     //
		//==============================================================================//

		////==========================================================================////
		////      Non-Virtual                                                         ////
		////==========================================================================////

		//////======================================================================//////
		//////    Non-Static                                                        //////
		//////======================================================================//////

const std::vector<componentTypeDetails>& componentFactory::getComponentTypeDetails()
{
	static std::vector<componentTypeDetails> l_allDetails{

#define COMPONENT_DETAILS(type) type::getTypeDetail(),
#define X(type) COMPONENT_DETAILS(type)
	COMPONENT_LIST
#undef X
	};
	return l_allDetails;
}


const std::vector<componentTypeDetails>& componentFactory::getScriptTypeDetails()
{
	static std::vector<componentTypeDetails> l_scriptDetails{

#define SCRIPT_DETAILS(type) type::getTypeDetail(),
#define X(type) SCRIPT_DETAILS(type)
	SCRIPT_LIST
#undef X
	};
	return l_scriptDetails;
}

/////========================================================================/////
		/////     Functions                                                          /////
		/////========================================================================/////
void componentFactory::deserializeComponents(std::shared_ptr<gameObject> p_object,
	typeRT & p_data)
{

	#define COMPONENT_DESERIALIZE(type) \
		else if(l_type_name == "class " #type /*type::getType().name()*/) \
		{ \
			auto l_comp = p_object->addComponent<type>(); \
			l_comp->updateFromTypeRT(i_component); \
		}

	for(auto i_component : p_data.member("components").array())
	{
		std::string l_type_name = i_component.getTypeName();
		if(l_type_name == "class transform") 
		{ 
			p_object->getComponent<transform>()->updateFromTypeRT(i_component);
		}

		#define X(type) COMPONENT_DESERIALIZE(type)
		COMPONENT_LIST
		#undef X

		else
		{
			logger("ComponentFactory").error() << "Unhandled Component Type " << l_type_name;
		}
	}
}

void componentFactory::addComponentByType(std::shared_ptr<gameObject> p_object, const std::string & p_type)
{
#define COMPONENT_ADD(type) \
	else if(p_type == type::getType().name()) \
	{ \
		if(p_object->getComponentCount<type>() == 0  /* don't have any */ \
			|| getComponentTypeDetails(type::getType().name()).allowMultipleInstances) /* or allow multiples */ \
		{ \
			p_object->addComponent<type>(); \
			return; /* add it */ \
		} \
	}

	if(p_type == "class transform") { /* special case */ }
#define X(type) COMPONENT_ADD(type)
	COMPONENT_LIST
#undef X

}

void componentFactory::removeComponentByType(std::shared_ptr<gameObject>& p_object, const std::string& p_type)
{
	//TODO: Change this function to remove a component by component ID

#define COMPONENT_REMOVE(type) \
	else if(p_type == type::getType().name()) \
	{ \
		if(p_object->getComponentCount<type>() != 0) \
		  p_object->removeComponent<type>(); \
	}


	if(p_type == "class transform") { /* special case */ }

#define X(type) COMPONENT_REMOVE(type)
	COMPONENT_LIST
#undef X
}

const componentTypeDetails& componentFactory::getComponentTypeDetails(const std::string & p_name)
{
	static componentTypeDetails s_unknown_object(componentType::forConcrete<componentFactory>(), "unknown", "unknown", false, false, false);

	const std::vector<componentTypeDetails> & l_list = getComponentTypeDetails();
	auto i_iter = std::find_if(l_list.begin(), l_list.end(), [&p_name](const componentTypeDetails & p_detail)->bool
	{
		return p_detail.name == p_name;
	});

	if (i_iter != l_list.end())
		return *i_iter;

	logger("ComponentFactory").error() << "Cannot find componentTypeDetails for component '" << p_name << "'";
	return s_unknown_object;
}

