/*!***************************************************************************************
\file       TelemetrySystem.cpp
\author     Cole Astaire
\date       8/25/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the TelemetrySystem class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "TelemetrySystem.hpp"

//========1st Party Includes============================================================//
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include <Logger.hpp>
#include "../SceneManagement/Transform.hpp"
#include "../SystemManager.hpp"


//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
void SQL_Test();
/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////
namespace fsys = std::filesystem;

////////==========================================================================////////
////////  Enum(s) / Struct / Class                                                ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////


/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

void telemetrySystem::onInitialize()
{
	//create logger
	logger l_logger("TelemetrySystemInit");

	//print the date and time
	std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
	m_current_session_id = today;

	//convert timestamp to string
	time_t tt = std::chrono::system_clock::to_time_t(today);
	l_logger.debug() << "current session id: " << tt << std::endl;

	//unique file name, TODO: make item type modular, based on the desired object.
	m_local_filename = std::to_string(tt) + "_transform.csv";

	//create temp folder
	fsys::create_directory(m_local_temp_data_path);

	//construct m_local_path
	//m_local_path << "./data/logs/" << m_local_filename;
	//m_local_path << m_local_filename;
	m_local_temp_data_path += m_local_filename;
	//m_digipen_team_shared_path_alias += m_local_filename;
	//m_digipen_support_shared_path += m_local_filename;

	//create the file, TODO: determine how many files will need to be created for each session... 
	m_fs.open(m_local_temp_data_path.c_str(), std::fstream::out);
	//m_fs.open(m_digipen_team_shared_path_alias.c_str(), std::fstream::out);
	//m_fs.open(m_digipen_support_shared_path.c_str(), std::fstream::out);
	if(!m_fs.is_open())
	{
		l_logger.debug() << "unable to open file at path: " << m_local_path.str();
	}

	//SQL_Test();

	//open connection to database
	//SQLConnection l_conn("simson0.digipen.edu", 3306, "DBAST", "c.astaire", "280012916");
	//m_connection = l_conn;
	//m_connection.OpenConnection();
	//m_connection.SetQuery("drop table positions");
	//m_connection.SetQuery("create table positions(player_id int, x float, y float, z float)");
}

void telemetrySystem::onUpdate()
{
	//update timer, run once per buffer timer
	m_current_buffer_duration -= getDt();
	if(m_current_buffer_duration < 0)
	{
		m_current_buffer_duration = m_buffer_timer;
	}
	else
	{
		return;
	}

	//create logger
	//logger l_logger("TelemetrySystemUpdate");

	//set of component objects
	GameObjectFiltering::componentTypeSet l_components;
	l_components.setType(transform::getType());

	//ask object gatherer for all objects
	auto l_listObjects = getSystemManager()->getGameObjectGatherer()->getList(l_components);

	//parse the list for queryable objects
	for (auto & l_object : l_listObjects)
	{
		//currently, only transforms are queryable, hardcoding the extraction of their data
		//in the future, we will simply parse through the list of objects and check if they are queryable, then ask for data

		//get transform
		auto l_transform = l_object->getComponent<transform>();

		//append position to remote database--------------------------------------
		//get id
		//long int l_id = std::atol((l_transform->getQueryableID()).c_str());
		////offload
		//m_connection.OffloadEntityPosition("positions", l_id, l_transform->getPosition());


		//append position data to local files-------------------------------------

		//l_logger.debug() << l_transform->getQueryableType() << " ID: " << l_transform->getQueryableID() << std::endl;
		m_fs << l_transform->getQueryableID() << ", ";
		
		std::vector<std::string> l_properties = l_transform->getQueryablePropertyNames();
		
		//l_logger.debug() << l_transform->getQueryableType() << " Data: " << l_transform->getQueryableProperty("position");
		m_fs << l_transform->getQueryableProperty("position");
		m_fs << std::endl;
	}
}

void telemetrySystem::onShutdown()
{
	//close remote connections
	m_connection.CloseConnection();

	//close files
	m_fs.close();

	//copy the temp folder onto the digipen network
	std::string l_stamp = std::to_string(std::chrono::system_clock::to_time_t(m_current_session_id));
	fsys::copy("./data/logs/temp/", m_digipen_team_shared_path_alias + l_stamp);

	//remove the temp files, currently broken
	fsys::remove_all("./data/logs/temp");
}

void SQL_Test()
{
	vector3 l_testvec(1, 2, 3);

	SQLConnection testcon("simson0.digipen.edu", 3306, "DBAST", "c.astaire", "280012916");
	testcon.OpenConnection();
	testcon.SetQuery("drop table positions");
	testcon.SetQuery("create table positions(x float, y float, z float)");
	//testcon.OffloadEntityPosition("positions",,,,);
	//testcon.OffloadEntityPosition("positions",,,,);

	testcon.CloseConnection();
}

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Pure-Virtual/Overridden/Virtual/Non-Virtual                                 ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper /Destructor //
//======================================================================================//

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*=======================================================================================/
/         Other                                                                          /
/=======================================================================================*/

// namespace <namespace name>
