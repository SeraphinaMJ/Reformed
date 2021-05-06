/*!***************************************************************************************
\file       ScriptMaps.hpp
\author     Ryan Booth
\date       9/13/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			This file contains the declerations for functions that populate and access the list of script functions.
*****************************************************************************************/

#pragma once
//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <map>
#include <string>
#include <functional>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: scriptMaps
\brief			Holds the implementation and maps for all game-play scripts that are loaded
*****************************************************************************************/
class scriptMaps
{
		///////============================================================================///////
		///////   Public                                                                  ///////
		///////============================================================================///////
public:
		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		/*!***************************************************************************************
		\brief  Adds a script to the map of onStart functions
		\param script_name - The name of the game-play script
		\param func				 - The onStart function
		*****************************************************************************************/
		void add_on_start_script(std::string script_name, std::function<void()> func);

		/*!***************************************************************************************
		\brief  Adds a script to the map of onUpdate functions
		\param script_name - The name of the game-play script
		\param func				 - The onUpdate function
		*****************************************************************************************/
		void add_on_update_script(std::string script_name, std::function<void()> func);

		//======================================================================================//
		//        Getters & Setters                                                            ///
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Gets a script from the map of onStart functions
		\param script_name - The name of the game-play script
		*****************************************************************************************/
		std::function<void()> get_script_on_start(std::string script_name);

		/*!***************************************************************************************
		\brief  Gets a script from the map of onUpdate functions
		\param script_name - The name of the game-play script
		*****************************************************************************************/
		std::function<void()> get_script_on_update(std::string script_name);

		///////============================================================================///////
		///////   Private                                                                  ///////
		///////============================================================================///////
private:
		static std::map<std::string, std::function<void()>> onStartScripts;  //!< Map of all the onStart() script functions

		static std::map<std::string, std::function<void()>> onUpdateScripts; //!< Map of all the onUpdate() script functions
};

