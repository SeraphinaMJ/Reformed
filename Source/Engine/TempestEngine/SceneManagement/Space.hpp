/*!***************************************************************************************
\file       Space.hpp
\author     Aaron Damyen
\date       5/26/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the space class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../PersistenceRequest.hpp"
//========3rd Party Includes============================================================//
#include <mutex>
#include <vector>
#include <DataTypes.hpp>
#include "../DLLControl.hpp"

// forward declarations
class gameObject;
class scene;

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: space
\brief   Represents a space within a scene.  Manages game objects.
*****************************************************************************************/
class space final
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
	public:

		/*!***************************************************************************************
		\brief  Constructs a space within the scene
		\param p_scene - the parent scene to the new space
		\param p_name - the name of the new space
		*****************************************************************************************/
		space(scene * p_scene, std::string p_name);
		space(typeRT& p_type_rt);

		/*!***************************************************************************************
		\brief  Frees resources associated with the space
		*****************************************************************************************/
		virtual ~space();

		/*!***************************************************************************************
		\brief  Retrieves the number of game objects current in the space
		\return size_t - number of game objects in the space
		*****************************************************************************************/
		size_t getObjectCount() const;

		/*!***************************************************************************************
		\brief  Retrieves the current enabled status of the space
		\return bool - true if the space is enabled, false otherwise
		*****************************************************************************************/
		bool isEnabled() const;

	objID getID() const { return m_id; }

	typeRT toTypeRT() const;

	/*!***************************************************************************************
	\brief  Retrieves the name of the space
	\return string - the name of the space
	*****************************************************************************************/
    std::string getName() const;
	
	/*!***************************************************************************************
	\brief  Assigns a new name to the space
	\param p_new_name - the new name for the space
	*****************************************************************************************/
	void setName(std::string p_new_name);

		/*!***************************************************************************************
		\brief  Retrieves the parent scene that this space belongs to
		\return scene * - Pointer to the parent scene.
		*****************************************************************************************/
		scene * getScene() const;

		/*!***************************************************************************************
			\brief  Retrieves the parent scene that this space belongs to
			\return scene * - Pointer to the parent scene.
			*****************************************************************************************/
		void setScene(scene* p_scene);

		void moveGameObject(space* p_new_space, std::shared_ptr<gameObject> p_gameObject);

		/*!***************************************************************************************
		\brief  Instantiates a new game object within this space
		\return shared_ptr<gameObject> - a new, empty game object
		*****************************************************************************************/
		std::shared_ptr<gameObject> instantiateGameObject();
		std::shared_ptr<gameObject> instantiateGameObject(std::string p_name);
		std::shared_ptr<gameObject> instantiateGameObject(typeRT p_data);
		
		/*!***************************************************************************************
		\brief  Destroys an existing game object, if it belongs to this space
		\param p_game_object - the game object that should be destroyed, if it belongs to this space
		\return bool - returns true if the object was destroyed, false otherwise
		*****************************************************************************************/
		bool destroyGameObject(std::shared_ptr<gameObject> const & p_game_object);

        ///*!***************************************************************************************
        //\brief  Delete gameobject from disk
        //\param p_game_object - the game object that should be destroyed, if it belongs to this space
        //\return bool - returns true if the object was destroyed and deleted from disk, false otherwise
        //*****************************************************************************************/
        //bool deleteGameObjectFromDisk(std::shared_ptr<gameObject> const & p_game_object);
		
		/*!***************************************************************************************
		\brief  Moves a game object from this space to another space
		\param p_game_object - the game object that should be moved
		\param p_destination_space - the new space for the game object
		\return bool - returns true if the object was moved, false otherwise
		*****************************************************************************************/
		bool moveGameObject(std::shared_ptr<gameObject> & p_game_object, space * p_destination_space);
		
		/*!***************************************************************************************
		\brief  Retrieves a list of all game objects in the space
		\return std::vector<std::shared_ptr<gameObject>> - returns a list of all game objects in
		        the space
		*****************************************************************************************/
        std::vector<std::shared_ptr<gameObject>> const getGameObjectList() const;

		std::shared_ptr<gameObject> getGameObjectByID(objID p_objID);
		std::shared_ptr<gameObject> getGameObjectByName(const std::string& p_name);

		/*!***************************************************************************************
		\brief  Retrieves a list of all top level game objects in the space
		\return std::vector<std::shared_ptr<gameObject>> - returns a list of all top level game 
		        objects in the space
		*****************************************************************************************/
        std::vector<std::shared_ptr<gameObject>> const getTopLevelGameObjectList() const;

		/*!***************************************************************************************
		\brief  Marks a space so it is retained when a new scene is loaded
		*****************************************************************************************/
		void dontDeleteOnLoad();

    /*!***************************************************************************************
      \brief  Marks a space so it is not retained when a new scene is loaded
      *****************************************************************************************/
    void deleteOnLoad();

		/*!***************************************************************************************
		\brief  Retrieves the status of the getDeleteOnLoad flag.
		\return bool - Returns false if the space should be retained from scene to scene, true otherwise
		*****************************************************************************************/
		bool getDeleteOnLoad() const { return m_shouldDeleteOnLoad; }

		/*!***************************************************************************************
		\brief  Sets the space's enabled status
		\param p_is_enabled - new enabled status of this space
		*****************************************************************************************/
		void setEnabled(bool p_is_enabled);

		static std::shared_ptr<space> read(persistenceRequest<space>* p_persist_request);
		void write(persistenceRequest<space>* p_persist_request);

    bool getShouldSerialize() const { return m_should_serialize; }
    void setShouldSerialize(bool p_should_serialize) { m_should_serialize = p_should_serialize; }


	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
	private:

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////
		 objID m_id;

		/////================================================================================/////
		/////     Data                                                                       /////
		/////================================================================================/////

		////==================================================================================////
		////      Configurable                                                                ////
		////==================================================================================////
		bool m_shouldDeleteOnLoad; /*!< Indicator if this space should be deleted when a new scene
		                          * loads */

		////==================================================================================////
		////      Non-Configurable                                                            ////
		////==================================================================================////

		bool enabled;     //!< Indicator if this space is enabled
		std::string name; //!< The name of this space

		mutable std::mutex object_list_mutex;             //!< Mutex used for accessing objects.
		std::vector<std::shared_ptr<gameObject>> objects; /*!< The list of game objects within
		                                                   * this space. */

		scene * containing_scene; //!< Reference to this space's parent scene.

    bool m_should_serialize = true;
};
