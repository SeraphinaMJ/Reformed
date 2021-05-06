/*!***************************************************************************************
\file       ComponentType.hpp
\author     Aaron Damyen
\date       5/17/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the componentType datatype.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <typeindex>
#include <string>
#include <ComponentRegistrar.hpp>

////////==========================================================================////////
////////  Definitions                                                             ////////
////////==========================================================================////////
/*!***************************************************************************************
\par type: componentType
\brief   Creates a component type that is actually just the reflection type_index.
*****************************************************************************************/
//typedef std::type_index componentType;

class componentType
{
public:

	template<typename T>
	static componentType forConcrete()
	{
		return componentType(typeid(T), componentRegistrar<T>::getID());
	}

	template<typename T>
  static componentType forAbstract();

	template<typename Tchild, typename Tparent>
	static componentType forInherited()
	{
		auto l_result = componentType(typeid(Tchild), componentRegistrar<Tchild>::getID());
		l_result.parent_getType = &Tparent::getType;
		return l_result;
	}

	std::string name() const { return m_type.name(); }
	std::size_t hash_code() const { return m_type.hash_code(); }

	bool operator==(const componentType & p_other) const { return m_type == p_other.m_type; }
	bool operator<=(const componentType & p_other) const { return m_type == p_other.m_type || (p_other.hasParent() && *this <= p_other.getParentType()); }

	bool hasParent() const { return parent_getType != nullptr; }
	componentType const getParentType() const { return parent_getType(); }
  int getComponentID() const
  {
    return id;
  }

private:
	componentType(std::type_index p_type, int id_) : m_type{ p_type }, parent_getType{ nullptr }, id(id_) {}

	std::type_index m_type;
	componentType const (*parent_getType)() = nullptr;
  int id;
};


template <> componentType componentType::forAbstract<void>();

template<typename T>
componentType componentType::forAbstract()
{
	return componentType(typeid(T), componentRegistrar<T>::getID());
}


namespace std
{
	/*!*************************************************************************************
	\par struct: hash<componentType>
	\brief   Implementation of the has<componentType> template specialization so the componentType
			 class can be used as an STL container key value type.
	***************************************************************************************/
	template<>
	struct hash<componentType>
	{
		/*!***************************************************************************************
		\brief  Executes the hash value calculation.
		\param p_key - The set to calculate the hash value for.
		\return std::size_t - The calculated hash value.
		*****************************************************************************************/
		std::size_t operator()(const componentType & p_key) const
		{
			return p_key.hash_code();
		}
	};
} // namespace std
