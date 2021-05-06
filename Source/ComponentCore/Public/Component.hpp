/*!***************************************************************************************
\file       Component.hpp
\author     Aaron Damyen
\date       5/14/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  The definition of the component base class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "ComponentType.hpp"
#include <Reflection.hpp>
#include <iostream>
//========3rd Party Includes============================================================//
#include <memory>
#include <ComponentTypeDetails.hpp> // convenience for subclasses
#include <unordered_map> // component size map
#include <ComponentRegistrar.hpp>
//========Forward Declarations==========================================================//
class gameObject;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: component
\brief   This is the base class for all component types.
*****************************************************************************************/
class component
{
	////////======================================================================////////
	////////   Public                                                             ////////
	////////======================================================================////////
	public:
		//////======================================================================//////
		//////    (Non-)Static                                                      //////
		//////======================================================================//////

		/*!*******************************************************************************
		\brief  Retrieves the type of component this instance is. SceneSystem requirement.
		\return componentType - The type of this component.
		*********************************************************************************/
		virtual componentType const type() const = 0;

		/*!*******************************************************************************
		\brief  Allows inherited types to be properly destroyed.
		*********************************************************************************/
		virtual ~component() = default;

		/*!*******************************************************************************
		\brief  Converts this into a type
		\return Type equivalent.
		*********************************************************************************/
		virtual typeRT toTypeRT() const = 0;

		virtual void updateFromTypeRT(typeRT & p_type) = 0;

		virtual bool isTransient() { return false; }

		/*!*******************************************************************************
		\brief  Retrieves the reference to the game object this component is attached to.
		\return std::weak_ptr<gameObject> - Reference to the parent game object.
		*********************************************************************************/
		std::weak_ptr<gameObject> getGameObject();

		/*!*******************************************************************************
		\brief  Tells the total number of types of components in the entire engine.
		\return int - number of component types in the game engine.
		*********************************************************************************/
		static int getComponentTypes() { return globalComponentRegistrar::getComponentTypes();  }
		virtual int getTypeID() = 0;

    static std::map<int, size_t>& getComponentSizeMap()
    {
      return globalComponentRegistrar::getComponentSizeMap();
    }
    static std::map<int, std::string>& getComponentNameMap()
    {
      return globalComponentRegistrar::getComponentNameMap();
    }

    static std::map<int, size_t>& getComponentMemoryBlockSizeMap()
    {
      return globalComponentRegistrar::getComponentMemoryBlockSizeMap();
    }

    static std::map<int, int>& componentMemoryBlockOffsets()
    {
      return globalComponentRegistrar::componentMemoryBlockOffsets();
    }

    static std::map<int, int> componentBaseToMostDerivedOffsetMap()
    {
      return globalComponentRegistrar::componentBaseToMostDerivedOffsetMap();
    }

	////////======================================================================////////
	////////   Private                                                            ////////
	////////======================================================================////////
	private:
		/*!*******************************************************************************
		\brief  Retrieves the reference to the game object this component is attached to.
		\return std::weak_ptr<gameObject> - Reference to the parent game object.
		*********************************************************************************/
		/*!*******************************************************************************
		\brief  Reference to the parent game object.
		*********************************************************************************/
		std::weak_ptr<gameObject> parentGameObject;

		//////======================================================================//////
		//////    (Non-)Static                                                      //////
		//////======================================================================//////

		/*!*******************************************************************************
		\brief  Allows game object to reach into this to update parent properly.
		\relates gameObject
		*********************************************************************************/
		friend gameObject;
};

template <typename T>
class componentCRTP : public component
{
public:

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override {
		static componentType const s_type = componentType::forConcrete<T>();
		return s_type;
	}

	virtual int getTypeID() override {
		return componentID;
	}
	static int getStaticID()
	{
		return componentID;
	}
private:
	static int componentID;
};

template <typename T>
int componentCRTP<T>::componentID = componentRegistrar<T>::getID();

