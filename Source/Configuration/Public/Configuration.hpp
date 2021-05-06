/*!***************************************************************************************
\file       Configuration.hpp
\author     Aaron Damyen
\date       7/6/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the configuration class for storing engine configuration.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <string>
#include <functional>
#include <filesystem>

////////==========================================================================////////
////////  Structures                                                              ////////
////////==========================================================================////////

/*!***************************************************************************************
\par struct: configuration
\brief   This struct collects all configuration settings for the engine.  This is not game-level
         configuration.
*****************************************************************************************/
struct configuration final
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	std::filesystem::path asset_path; //!< The current path to the assets folder.
	std::filesystem::path data_path; //!< The current path to the data folder.
	std::filesystem::path log_path;
	int initial_window_show; //!< The initial window state
	bool no_initial_scene; //!< Flag to indicate if a scene should be loaded after engine initialization
	std::string initial_scene_name; //!< The initial scene to load (should be moved into a file in the data folder).

	bool quiet;
	bool verbose;

	/*!***************************************************************************************
	\brief  Constructs an empty configuration structure.
	*****************************************************************************************/
	configuration() : asset_path{ "" }, data_path{ "data/" }, log_path{ "" }, initial_window_show{ 0 }, no_initial_scene{false},
	initial_scene_name{ "" }, quiet{false}, verbose{false}

	{ }

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

typedef std::function<configuration()> configurationFunctionPtr;

