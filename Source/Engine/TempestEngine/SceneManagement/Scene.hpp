/*!***************************************************************************************
\file       Scene.hpp
\author     Aaron Damyen
\date       5/29/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the scene class.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../GameObjectCollector.hpp"
#include "Space.hpp"
#include "../PersistenceRequest.hpp"

//========3rd Party Includes============================================================//
#include <vector>
#include <DataTypes.hpp>
#include "../DLLControl.hpp"

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: Scene
\brief   The scene class manages a list of spaces.
*****************************************************************************************/
class scene final
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
	public:

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////
		static std::shared_ptr<scene> read(persistenceRequest<scene>* p_persist_request);
        //static std::shared_ptr<scene> remove
		void write(persistenceRequest<scene>* p_persist_request);

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Operators                                                                     //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Converts to a type. Please note this has a special implementation of only
		        containing the names of the spaces not the spaces. This may change latter!
		\return The typeRT equivalent of this scene.
		*****************************************************************************************/
		typeRT toTypeRT() const;

		/*!***************************************************************************************
		\brief  Creates a new scene and specifies the game object collector the scene should
				report changes to.
		\param p_new_collector - pointer to the collector to which all changes should be reported
		*****************************************************************************************/
		scene(gameObjectCollector * p_new_collector, std::string p_name = "Unnamed_scene");
		
		/*!***************************************************************************************
		\brief  Releases all resources allocated by this scene.
		*****************************************************************************************/
		virtual ~scene();

		/*!***************************************************************************************
		\brief  Sets a new game object collector for this scene
		\param p_new_collector - the new collector that all changes should be reported to.
		*****************************************************************************************/
		void setGameObjectCollector(gameObjectCollector * p_new_collector);

		/*!***************************************************************************************
		\brief  Retrieves the current game object collector
		\return gameObjectCollector * - the current game object collector
		*****************************************************************************************/
		gameObjectCollector * getGameObjectCollector() const;

		objID getID() const { return m_id; }

        void setName(std::string p_name) { m_name = p_name; }
        std::string getName() const { return m_name; }

		/*!***************************************************************************************
		\brief  Creates a new space within this scene
		\return std::shared_ptr<space> - the newly created space
		*****************************************************************************************/
		std::shared_ptr<space> createSpace();
		std::shared_ptr<space> createSpace(const std::string& p_name);

		/*!***************************************************************************************
		\brief  Moves a space to this scene
		\param p_space - Space to move
		*****************************************************************************************/
		void moveSpace(const std::shared_ptr<space>& p_space);

		/*!***************************************************************************************
		\brief  Destroys a space, if it exists within this scene
		\param p_old_space - the space that should be destroyed
		*****************************************************************************************/
		void destroySpace(std::shared_ptr<space> const & p_old_space);

		/*!***************************************************************************************
		\brief  Retrieves all the spaces
		\return std::shared_ptr<space> - a reference to the space requested
		*****************************************************************************************/
        const std::vector<std::shared_ptr<space>> & getSpaces() const;

		/*!***************************************************************************************
		\brief  Retrieves a space by name //TODO(MP): Talk about optomizing
		\param p_name - the name of the space to retrieve
		\return std::shared_ptr<space> - a reference to the space requested
		*****************************************************************************************/
		std::shared_ptr<space> getSpace(std::string p_name) const;

		/*!***************************************************************************************
		\brief  Retrieves a space by its index
		\param p_index - the number of the space to retrieve
		\return std::shared_ptr<space> - a reference to the space requested
		*****************************************************************************************/
		std::shared_ptr<space> getSpaceByIndex(size_t p_index) const;

		/*!***************************************************************************************
		\brief  Finds a game object by name within this scene
		\param p_name - the name of the game object to find
		\return std::shared_ptr<gameObject> - a reference to the game object requested
		*****************************************************************************************/
		std::shared_ptr<gameObject> findGameObject(std::string p_name) const;

		/*!***************************************************************************************
		\brief  Finds a game object by name within a specific space within this scene
		\param p_name - the name of the game object to find
		\param p_space_to_look_in - The space to check for the game object
		\return std::shared_ptr<gameObject> - a reference to the game object requested
		*****************************************************************************************/
		//std::shared_ptr<gameObject> findGameObject(std::string p_name, std::shared_ptr<space> p_space_to_look_in) const;

		std::shared_ptr<gameObject> findGameObjectByID(objID p_id) const;
		void destroyGameObject(std::string p_name);
		void destroyGameObjectByID(objID p_ID);



	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
		std::shared_ptr<gameObject> getCameraObject() {
			return editor_camera_object;
		}
private:

	objID m_id; //!< The run-time unique ID number of this scene
    std::string m_name; //!< The name of this scene
		std::vector<std::shared_ptr<space>> spaces; //!< The list of spaces within the scene
    gameObjectCollector * go_collector; //!< Reference to the active collector for this scene's game objects
		//struct physcics_shit;
	std::shared_ptr<gameObject> editor_camera_object;
	space editor_camera_space;
	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};



