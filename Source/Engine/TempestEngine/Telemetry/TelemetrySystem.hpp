/*!***************************************************************************************
\file       TelemetrySystem.hpp
\author     Cole Astaire
\date       8/25/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      The Telemetry System provides  a pipeline for gathering  and storing data from
			a specified object, object type, or alternate system.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../SystemBase.hpp"

//========3rd Party Includes============================================================//
#include <fstream>
#include <chrono>
#include <Vector3.hpp>
#include "SQLConnection.hpp"

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class telemetrySystem final : public systemBase
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the system
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("telemetrySystem"); return n; }
	virtual const std::string& name() const override { return getName(); }

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////
protected:

	/*!***************************************************************************************
	\brief determine a unique ID for the session, create and open necessary files
	*****************************************************************************************/
	void onInitialize() override;

	/*!***************************************************************************************
	\brief get a list of all relevant components and systems, determine if objects are
		   queryable, begin the query and write data if necessary
	*****************************************************************************************/
	void onUpdate() override;

	/*!***************************************************************************************
	\brief clean up, close all necessary files
	*****************************************************************************************/
	void onShutdown() override;


	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////

private:

	//====  remote streams =====================================================================//
	SQLConnection m_connection;

	//====  local streams =====================================================================//
	std::fstream m_fs; //!< output file stream
	std::string m_local_filename; //!< generated file name with time stamp
	std::stringstream m_local_path; //!< file name

	//====  timers/counters ============================================================================//
	float m_buffer_timer = 0.5f;
	float m_current_buffer_duration = 0;
	float m_buffer_counter;

	//==== file paths =============================================================================//
	std::string m_local_temp_data_path = "./data/logs/temp/"; //!< access to the local temp folder
	std::string m_local_data_path = "./data/logs/"; //!< access to the local logs folder
	std::string m_digipen_team_shared_path_alias = "S:\\Teams\\US\\_ShareWith\\supremetoyinc_ShareWith_everybody\\data\\logs\\"; //!< path of the public shared team folder. 
	std::string m_digipen_team_shared_path_samba = "\\shares.digipen.edu\\Teams\\US\\_ShareWith\\supremetoyinc_ShareWith_everybody\\data\\logs\\"; //!< path of the public shared team folder. should work for any digipen machine
	std::string m_digipen_support_shared_path = "S:\\Teams\\US\\_ShareWith\\support_ShareWith_everybody\\sti_temp\\"; //!< path of the shared support folder

	//==== storage  ===========================================================================//
	std::vector<vector3> m_transform_table;
	static constexpr int MAX_KEYS = 1024;
	const bool* m_keys; //!< current status of known buttons

	//====  misc. ============================================================================//
	std::chrono::system_clock::time_point m_current_session_id; //!< numerical id of the current session, represents the number of seconds since computer time

};


