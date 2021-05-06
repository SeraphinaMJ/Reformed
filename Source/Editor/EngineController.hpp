/*!***************************************************************************************
\file       EngineController.hpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for the engine controller class
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class engineRunner;

namespace Editor
{
  struct EditorLogger;
	/*!***************************************************************************************
	\par class: engineController
	\brief   This is the class which knows how to interact with the game engine directly.
	*****************************************************************************************/
	class engineController
	{
	public:
		/*!***************************************************************************************
		\brief  Constructs a controller for a particular engine
		\param p_engine - the engine to control
		*****************************************************************************************/
		engineController(engineRunner * p_engine, EditorLogger & p_logger);

		/*!***************************************************************************************
		\brief  Specifies a new engine to control, releasing old engine
		\param p_engine - pointer to the new engine to be controlled.
		*****************************************************************************************/
		void setEngine(engineRunner * p_engine);
		/*!***************************************************************************************
		\brief  Retrieves the pointer to the engine runner being controlled
		\return engineRunner * - Pointer to the engine runner
		*****************************************************************************************/
		engineRunner * getEngineRunner() const;
		/*!***************************************************************************************
		\brief  Releases control of the current engine, restoring its state appropriately
		*****************************************************************************************/
		void releaseEngine();

		/*!***************************************************************************************
		\brief  Allows the engine to process events/state.  Effectively runs the engine for a step.
		*****************************************************************************************/
		void processEvents();

		/*!***************************************************************************************
		\brief  Sets the engine into the running state, starting all appropriate systems
		*****************************************************************************************/
		void start(bool p_override_temporary_store = false);
		/*!***************************************************************************************
		\brief  Sets the engine into the paused state, stopping most systems (excepting for those 
		        needed to view the state of the engine in the editor)
		*****************************************************************************************/
		void stop();

		/*!***************************************************************************************
		\brief  Retrieves the playing status of the underlying engine.
		\return bool - True if the engine is currently playing, false otherwise
		*****************************************************************************************/
		bool isPlaying();

	private:
		/*!***************************************************************************************
		\brief  Pushes a camera into the engine specifically for the editor's use
		*****************************************************************************************/
		void injectEditorCamera();
		/*!***************************************************************************************
		\brief  Removes the editor specific camera (as pushed by injectEditorCamera()).
		*****************************************************************************************/
		void removeEditorCamera();

		/*!***************************************************************************************
		\brief  Writes the state of current scene to a temporary location.  (Overwrites any previous
		        stored temporary scene)
		*****************************************************************************************/
		void storeTemporaryScene();
		/*!***************************************************************************************
		\brief  Reads and restores the state of the current scene from the temporary location.
		*****************************************************************************************/
		void restoreTemporaryScene();
		
		engineRunner * m_engine;
		bool m_is_playing = false;
    EditorLogger & m_logger;
	};


} // namespace Editor




