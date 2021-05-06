/*!***************************************************************************************
\file       ScriptMaps.cpp
\author     Ryan Booth
\date       9/13/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			This file contains the functions that populate and access the list of script functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "ScriptMaps.hpp"

//========1st Party Includes============================================================//
#include <Logger.hpp>

std::map<std::string, std::function<void()>> scriptMaps::onStartScripts;  //!< Map of all the onStart() script functions
std::map<std::string, std::function<void()>> scriptMaps::onUpdateScripts; //!< Map of all the onUpdate() script functions

		///////============================================================================///////
		///////   Public                                                                  ///////
		///////============================================================================///////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

void scriptMaps::add_on_start_script(std::string script_name, std::function<void()> func)
{
		//Try to find a previous function with the same id/name.
		if(onStartScripts.find(script_name) == onStartScripts.end())
		{
				//Add it to the map
				onStartScripts[script_name] = func;
		}
		else
		{
				logger("ScriptMaps").info() << "Script " << script_name << "has already been added to the map of start functions.";
		}
}

void scriptMaps::add_on_update_script(std::string script_name, std::function<void()> func)
{
		//Try to find a previous function with the same id/name.
		if (onUpdateScripts.find(script_name) == onUpdateScripts.end())
		{
				//Add it to the map
				onUpdateScripts[script_name] = func;
		}
		else
		{
				logger("ScriptMaps").info() << "Script " << script_name << "has already been added to the map of update functions.";
		}
}

		//======================================================================================//
		//        Getters & Setters                                                            ///
		//======================================================================================//

std::function<void()> scriptMaps::get_script_on_start(std::string script_name)
{
		//Try to find a previous function with the same id/name.
		std::map<std::string, std::function<void()>>::iterator it;
		it = onStartScripts.find(script_name);

		if (it != onStartScripts.end())
		{
				//Return the function pointer
				return it->second;
		}
		else
		{
				logger("ScriptMaps").info() << "Could not find start function for " << script_name << ".";
		}

		return NULL;
}

std::function<void()> scriptMaps::get_script_on_update(std::string script_name)
{
		//Try to find a previous function with the same id/name.
		std::map<std::string, std::function<void()>>::iterator it;
		it = onUpdateScripts.find(script_name);

		if (it != onUpdateScripts.end())
		{
				//Return the function pointer
				return it->second;
		}
		else
		{
				logger("ScriptMaps").info() << "Could not find update function for " << script_name << ".";
		}

		return NULL;
}
