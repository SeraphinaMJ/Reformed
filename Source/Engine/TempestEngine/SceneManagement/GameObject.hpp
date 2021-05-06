/*!***************************************************************************************
\file       GameObject.hpp
\author     Aaron Damyen
\date       5/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the interface for gameObject.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "Component.hpp"
#include "ComponentList.hpp"
#include "Scene.hpp"
#include "Space.hpp"
#include <IComponentRepository.hpp>
//#include "../Components/ScriptCPP.hpp"     !!!This causes a bug in the hierarchy selection keeper

//========3rd Party Includes============================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: gameObject
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class gameObject final : public std::enable_shared_from_this<gameObject>
{
public:

	objID getID() const { return m_id; }

	static std::shared_ptr<gameObject> read(persistenceRequest<gameObject>* p_persist_request);
	void write(persistenceRequest<gameObject>* p_persist_request);

	/*!***************************************************************************************
	\brief  Retrieves the name of the game object
	\return string - the name of the game object
	*****************************************************************************************/
	std::string getName() const;

	/*!***************************************************************************************
	\brief  Assigns a new name to the game object
	\param p_new_name - the new name for the game object
	*****************************************************************************************/
	void setName(std::string p_new_name);

	/*!*******************************************************************************
	\brief  Updates any existing components on this game object with the serialized data.  
			 NOTE: This will *not* delete any existing components.  This is not as
			 comprehensive as synchronizeGameObject() free function.
	param p_type - the serialized data to read from.
	*********************************************************************************/
	void applyFromTypeRT(typeRT & p_type);

	/*!*******************************************************************************
	\brief  Retrieves the componentTypeSet representing the current component make-up
			of this object.
	\return componentTypeSet - The current component configuration of this object.
	*********************************************************************************/
	GameObjectFiltering::componentTypeSet getComponentTypeSet() const;

	/*!*******************************************************************************
	\brief  Adds a component to the game object, updating the gameObjectFilter
			accordingly.
	\param p_component - the component to be added
	\return T * - The component that was added (for chaining)
	*********************************************************************************/
	// TODO(coseo) figure this out... using raw pointers very heavily
	template<typename T>
	componentHandle<T> addComponent()
	{
		GameObjectFiltering::componentTypeSet l_previous_type_set = getComponentTypeSet();
		std::pair<componentHandle<T>, bool> l_result = component_list.add<T>();
    componentHandle<T> l_component = l_result.first;

		if (std::get<1>(l_result))
			getSpace()->getScene()->getGameObjectCollector()->updateGameObject(shared_from_this(), l_previous_type_set);
		l_component->parentGameObject = shared_from_this();
		return l_component;
	}

	/*!*******************************************************************************
	\brief  Removes a component from the game object, updating the gameObjectFilter
			accordingly.
	\param p_component - the component to be removed
	\return std::shared_ptr<T> - Reference to the component that was removed.  As it
			is a shared pointer, the caller can decide what to do with it.  If they
			just want it deleted, the caller need not take any actions; the shared
			pointer will delete it automatically.
	*********************************************************************************/
	template<typename T>
	componentHandle<T> removeComponent(componentHandle<T> p_component)
	{
		GameObjectFiltering::componentTypeSet l_previous_type_set = getComponentTypeSet();
		if (component_list.remove<T>(p_component))
			getSpace()->getScene()->getGameObjectCollector()->updateGameObject(shared_from_this(), l_previous_type_set);
		p_component->parentGameObject.reset();
		return p_component;
	}

	/*!*******************************************************************************
	\brief  Removes the first component of a particular type
	*********************************************************************************/
	template<typename T>
	void removeComponent()
	{
		// TODO(coseo) make sure that memory is actually getting freed....
		auto l_comp = getComponent<T>();
		if (l_comp)
			removeComponent(l_comp);
	}


        void RemoveAllComponents();

	/*!*******************************************************************************
	\brief  Retrieves the first component of the specified type
	\return T * - The first component of the type specified
	*********************************************************************************/
	template<typename T>
	componentHandle<T> getComponent()
	{
		return component_list.getComponent<T>();
	}

	// TODO(coseo) remove this function, it doesn't make sense anymore 
	baseComponentHandle getComponent(componentType p_type)
	{
		return component_list.getComponentByType(p_type);
	}


	/*!*******************************************************************************
	\brief  Retrieves all components of the specified type
	\return std::vector<T *> - List of all components of the type specified
	*********************************************************************************/
	template<typename T>
	std::vector<componentHandle<T>> getComponents()
	{
		int size = component_list.getComponentCount<T>();
		std::vector<componentHandle<T>> result;
		result.reserve(size);
		auto original_list = component_list.getComponentsByType(T::getType());
		for (int i = 0; i < size; ++i) {
      result.push_back(castHandle<T>(original_list[i]));
		}
		return result;
	}

	std::vector<baseComponentHandle> getComponents(componentType p_type)
	{
		return component_list.getComponentsByType(p_type);
	}

	std::vector<baseComponentHandle> getComponents(std::string p_type)
	{
		return component_list.getComponentsByString(p_type);
	}
	//We cant use this funcion because of an inclusion bug in hierarchy selection window
		/*template<typename T>
		T * getScript()
		{
			for (auto i_script : getComponents<scriptCPP>())
			{
				if (i_script->sub_type() == T::getSubType())
				{
					return reinterpret_cast<T*>(i_script);
				}
			}
			return nullptr;
		}*/

		/*!*******************************************************************************
		\brief  Retrieves all components on the game object
		\return std::vector<component *> - List of all components on the game object
		*********************************************************************************/
	std::vector<baseComponentHandle> getAllComponents();

	/*!*******************************************************************************
	\brief  Retrieves all components on the game object
	\return std::vector<component *> - List of all components on the game object
	*********************************************************************************/
	std::vector<baseComponentHandle> getAllComponents() const;


	/*!*******************************************************************************
	\brief  Retrieves the number of components of a particular type on this object
	\return unsigned int - the number of components
	*********************************************************************************/
	template<typename T>
	unsigned int getComponentCount() const
	{
		return component_list.getComponentCount<T>();
	}

        /*!*******************************************************************************
        \brief  Tells you if the gameobject even has a parent.
        \return true if parent exists
        *********************************************************************************/
        bool hasParent() const;

		/*!*******************************************************************************
		\brief  Retrieves the reference of the parent game object
		\return std::weak_ptr<gameObject> - Reference to the parent game object, may not 
		        be valid (if no parent)
		*********************************************************************************/
		std::weak_ptr<gameObject> getParent() const;

	/*!*******************************************************************************
	\brief  Makes the game object a top-level object with no parent
	*********************************************************************************/
	void detatchFromParent();

	/*!*******************************************************************************
	\brief  Deleted function.  Parent can only be set inside the addChild method to
			keep references synchronized.
	\param p_new_parent - the reference to the new parent game object
	*********************************************************************************/
	void setParent(std::weak_ptr<gameObject> p_new_parent) = delete;

	/*!*******************************************************************************
	\brief  Determines if this object is at the root of the space, that is, not a
			child of another object.
	\return True if the object is not a child of another object, false otherwise
	*********************************************************************************/
	bool isTopLevelObject() const;

	/*!*******************************************************************************
	\brief  Retrieves the list of references to children game objects
	\return std::vector<std::shared_ptr<gameObject>> - Array of references to children
			game objects
	*********************************************************************************/
	std::vector<std::shared_ptr<gameObject>> getChildren();

	/*!*******************************************************************************
	\brief  Adds a game object to the list of children for this object.  Also updates
			the parent reference within the new child object.
	\param p_new_child - the child to be added to this object
	*********************************************************************************/
	void addChild(std::shared_ptr<gameObject> p_new_child);

	/*!*******************************************************************************
	\brief  Removes a game object child from this object.  Also updates the parent
			reference within the old child object
	\param p_old_child - the child to be removed from this object
	*********************************************************************************/
	void removeChild(std::shared_ptr<gameObject> p_old_child);

	/*!*******************************************************************************
	\brief  Retrieves the space that this game object is contained within
	\return space * - Space this game object is contained within
	*********************************************************************************/
	space* getSpace();

	/*!*******************************************************************************
	\brief  Frees any resources allocated to this gameObject.
	*********************************************************************************/
	virtual ~gameObject();

	std::shared_ptr<gameObject> cloneObject();

    // ask if object has been marked for deletion
    bool markedForDeletion() const;
    // set object to be deleted
    void markForDeletion();
private:

    // whether or not object has been marked for deletion
    bool shouldDelete;

	/*!***********************************************************************************
	\brief  Hidden constructor.  Because this class inherits from the
			enable_shared_from_this class, the constructor is hidden to prevent failure
			when constructing the objects without using a shared_ptr.
	\param p_space - the space to which this new game object will belong
	*************************************************************************************/
	gameObject(space * p_space);

	objID m_id;

	std::string m_name;

	/*!***********************************************************************************
	\brief  The list of components on this object
	*************************************************************************************/
	componentList component_list;

	/*!***********************************************************************************
	\brief  Reference to the parent to this object, if there is one
	*************************************************************************************/
	std::weak_ptr<gameObject> parent;

	/*!***********************************************************************************
	\brief  References to the children of this object
	*************************************************************************************/
	std::vector<std::shared_ptr<gameObject>> children;

	/*!***********************************************************************************
	\brief  Reference to the space this object is within
	*************************************************************************************/
	space* containing_space;


	/*!***********************************************************************************
	\brief  Allows the space class to look into the game objects (for maintaining
			parent-child relationship).
	\relates space
	*************************************************************************************/
	friend class space;

	/*!***********************************************************************************
	\brief  Allows the game object serialization free function to view into the game object.
	\relates typeRT toTypeRT(const std::shared_ptr<gameObject> &)
	*************************************************************************************/
	// TODO: Remove this when the GameObject class is moved to a separate project.
	friend typeRT toTypeRT(const std::shared_ptr<gameObject> &);

	/*!***********************************************************************************
	\brief  Allows the game object deserialization free function to view into the game object.
	\relates typeRT synchronizeGameObject(std::shared_ptr<gameObject> &, typeRT &)
	*************************************************************************************/
	// TODO: Remove this when the GameObject class is moved to a separate project.
	friend void synchronizeGameObject(std::shared_ptr<gameObject> &, typeRT &);

};

