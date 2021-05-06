/*!**********************************************************************************************************
\file       SQLConnection.hpp
\author     Cole Astaire
\date       10/23/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Defines sql connection object, which retains a connection to a MYSQL server
************************************************************************************************************/
#pragma once

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
#include <cstdlib>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <Vector3.hpp>

//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

class SQLConnection
{

public:
	//====  Static  =======================================================================================//
	//====  Pure-Virtual  =================================================================================//
	//====  Constructor/Destructor  =======================================================================//
	SQLConnection() = default;
	
	SQLConnection(std::string server,
		int port,
		std::string database,
		std::string user,
		std::string password,
		int option = 3,
		std::string provider = "MSDASQL",
		std::string driver = "{MySQL ODBC 8.0 ANSI Driver}");

	~SQLConnection();

	//====  Accessor Methods  =============================================================================//
	//====  Virtual/Overrides  ============================================================================//
	//====  Operators  ====================================================================================//
	void operator=(const SQLConnection &D);

	//====  Interface Declarations (For each interface Implemented)  ======================================//
	//====  Everything Else  ==============================================================================//

	std::string OpenConnection();

	void CloseConnection();
	
	void OffloadData();

	void OffloadEntityPosition(std::string p_table, int p_player_id, vector3 p_position);

	void SetQuery(std::string p_query);

	std::string ShowTables();

protected:

private:

	//====  helpers  ====================================================================================//
	void SetConnectionString();
	void Cleanup();

	//====  ODBC Data  ====================================================================================//
	SQLHENV m_henv = nullptr;
	SQLHDBC m_hdbc = nullptr;
	SQLHSTMT m_hstmt = nullptr;
	SQLRETURN m_retcode = 0;

	//====  Interface Data  ===============================================================================//
	std::string m_provider;
	std::string m_driver;
	std::string m_server;
	int m_port;
	std::string m_database;
	std::string m_user;
	std::string m_password;
	int m_option;

	std::string m_driver_string;

	bool m_isOpen = false;
};


// namespace <namespace name>
