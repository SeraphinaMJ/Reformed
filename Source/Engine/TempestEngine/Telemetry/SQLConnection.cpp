/*!**********************************************************************************************************
\file       SQLConnection.cpp
\author     Cole Astaire
\date       10/23/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "SQLConnection.hpp"

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//

#ifndef _countof
# define _countof(array) (sizeof(array) / sizeof(array[0]))
#endif

//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//

/*!***************************************************************************************
\brief sets server data
\param server
\param port
\param database
\param user
\param password
\param option
\param provider
\param driver
*****************************************************************************************/
SQLConnection::SQLConnection(std::string server,
	int port,
	std::string database,
	std::string user,
	std::string password,
	int option,
	std::string provider,
	std::string driver) :
	m_provider(provider),
	m_driver(driver),
	m_server(server),
	m_port(port),
	m_database(database),
	m_user(user),
	m_password(password),
	m_option(option)
{
	//set data
	m_isOpen = false;
	SetConnectionString();
}

/*!***************************************************************************************
\brief closes ope connections
*****************************************************************************************/
SQLConnection::~SQLConnection()
{
	//close connection if not closed
	if (!m_isOpen)
	{
		Cleanup();
	}
}

void SQLConnection::operator=(const SQLConnection& rhs)
{
	//ODBC data
	m_henv = rhs.m_henv;
	m_hdbc = rhs.m_hdbc;
	m_hstmt = rhs.m_hstmt;
	m_retcode = rhs.m_retcode;

	//interface data
	m_henv = rhs.m_henv;
	m_driver = rhs.m_driver;
	m_server = rhs.m_server;
	m_port = rhs.m_port;
	m_database = rhs.m_database;
	m_user = rhs.m_user;
	m_password = rhs.m_password;
	m_option = rhs.m_option;

	SetConnectionString();
}

std::string SQLConnection::OpenConnection()
{
	using std::cout;
	using std::endl;

	//open connection if not open

	if (!m_isOpen)
	{
		// Allocate environment handle
		m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);

		if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
		{
			// Set the ODBC version environment attribute
			m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

			if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
			{
				// Allocate connection handle
				m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);

				if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
				{
					// Set login timeout to 5 seconds
					SQLSetConnectAttr(m_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

					// Connect to data source
					const char * l_driver_string = m_driver_string.c_str();
					SQLCHAR * buffer = new SQLCHAR[1024];
					SQLSMALLINT len;
					m_retcode = SQLDriverConnect(m_hdbc, nullptr, (SQLCHAR*)l_driver_string, (SQLSMALLINT)m_driver_string.size(), buffer, 1024, &len, SQL_DRIVER_COMPLETE);

					// allocate statement handle
					if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO)
					{
						//connected
						m_isOpen = true;
						cout << "connection opened successfully" << endl;
					}
					else
					{
						cout << "Error connecting to: " << m_server << "/" << m_database << endl;
						cout << "Retcode: " << m_retcode << endl;
					}
				}
				else
				{
					cout << "Error  Allocating connection handle." << endl;
					cout << "Retcode: " << m_retcode << endl;
				}
			}
			else
			{
				cout << "Error Setting the ODBC version environment attribute." << endl;
				cout << "Retcode: " << m_retcode << endl;
			}
		}
		else
		{
			cout << "Error Allocating environment handle" << endl;
			cout << "Retcode: " << m_retcode << endl;
		}
	}

	return "nothing";
}

void SQLConnection::CloseConnection()
{
	//close connection if not closed
	if (!m_isOpen)
	{
		Cleanup();
	}
}

void SQLConnection::Cleanup()
{
	if (m_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		std::cout << "Statement Handle freed." << std::endl;
	}

	if (m_hdbc != SQL_NULL_HDBC) {
		SQLDisconnect(m_hdbc);
		std::cout << "connection closed." << std::endl;
		SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
		std::cout << "ODBC Handle freed." << std::endl;
	}

	if (m_henv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
		std::cout << "Environment Handle freed." << std::endl;
	}
}

void SQLConnection::OffloadData()
{
	//given a matrix, offload the data
}

void SQLConnection::OffloadEntityPosition(std::string p_table, int p_player_id, vector3 p_position)
{
	if(!m_isOpen)
	{
		std::cout << "connection is not open." << std::endl;
		return;
	}

	//// SQLBindParameter variables.
	//SQLLEN cbTextSize, lbytes;

	//// SQLParamData variable.
	//PTR pParmID;

	//// SQLPutData variables.
	//UCHAR  Data[] =
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	//	"abcdefghijklmnopqrstuvwxyz";

	//SDWORD cbBatch = (SDWORD)sizeof(Data) - 1;

	//prepare the query string
	std::stringstream l_query;

	l_query << "INSERT INTO "
		<< p_table
		<< " VALUES("
		<< p_player_id << ", "
		<< p_position.x << ", "
		<< p_position.y << ", "
		<< p_position.z << ")";

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	// execute query
	m_retcode = SQLExecDirect(m_hstmt, (UCHAR*)l_query.str().c_str(), SQL_NTS);
	if ((m_retcode != SQL_SUCCESS) && (m_retcode != SQL_SUCCESS_WITH_INFO)) {
		std::cout << "SQLParamData Failed" << std::endl;
	}

	//free the statement handle
	if (m_hstmt != SQL_NULL_HSTMT)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
		std::cout << "Statement Handle freed." << std::endl;
	}
}

void SQLConnection::SetQuery(std::string p_query)
{
	//TODO: integrate into offload data functions, currently it is just a test
	//set the current query

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	// Process data
	m_retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)(p_query.c_str()), SQL_NTS);

	if (m_retcode == SQL_SUCCESS) {
		SQLLEN cbTestStr;
		SQLINTEGER iCount = 1;
		//SQLFLOAT dTestFloat;
		SQLCHAR szTestStr[200];
		while (true) {
			m_retcode = SQLFetch(m_hstmt);
			if (m_retcode == SQL_ERROR || m_retcode == SQL_SUCCESS_WITH_INFO) {
				std::cout << "no data to fetch";
			}
			if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO) {

				SQLGetData(m_hstmt, 1, SQL_C_CHAR, szTestStr, 200, &cbTestStr);

				/* Print the row of data */
				std::cout << "Row " << iCount << ":" << std::endl;
				std::cout << szTestStr << std::endl;
				iCount++;
			}
			else {
				break;
			}
		}
	}
	else
	{
		std::cout << "SQLExecDirect failed" << std::endl;
		std::cout << "Retcode: " << m_retcode << std::endl;
	}
}

std::string SQLConnection::ShowTables()
{
	//if open

	//query db for list of tables


	return "nothing";
}

void SQLConnection::SetConnectionString()
{
	//set the entire connection string in proper driver format
	m_driver_string.clear();
	m_driver_string += "Provider=";
	m_driver_string += m_provider;
	m_driver_string += ";Driver=";
	m_driver_string += m_driver;
	m_driver_string += ";Server=";
	m_driver_string += m_server;
	m_driver_string += ";Port=";
	m_driver_string += std::to_string(m_port);
	m_driver_string += ";Database=";
	m_driver_string += m_database;
	m_driver_string += ";User=";
	m_driver_string += m_user;
	m_driver_string += ";Password=";
	m_driver_string += m_password;
	m_driver_string += ";Option=";
	m_driver_string += std::to_string(m_option);
}

void DONTUSETHIS()
{
	using namespace std;

	SQLHENV henv;
	SQLHDBC hdbc;
	SQLHSTMT hstmt;
	SQLRETURN retcode;

	//SQLCHAR * OutConnStr = (SQLCHAR *)malloc(255);
	//SQLSMALLINT * OutConnStrLen = (SQLSMALLINT *)malloc(255);

	// Allocate environment handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

		// Allocate connection handle
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

				// Connect to data source
				const char * driver_string = "Provider=MSDASQL;Driver={MySQL ODBC 8.0 ANSI Driver};Server=simson0.digipen.edu;Database=DBAST;User=c.astaire;Password=280012916;Option=3;";
				SQLCHAR * buffer = new SQLCHAR[1024];
				SQLSMALLINT len;
				retcode = SQLDriverConnect(hdbc, nullptr, (SQLCHAR*)driver_string, _countof(L"Provider=MSDASQL;Driver={MySQL ODBC 8.0 ANSI Driver};Server=simson0.digipen.edu;Database=DBAST;User=c.astaire;Password=280012916;Option=3;"), buffer, 1024, &len, SQL_DRIVER_COMPLETE);

				// Allocate statement handle
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
				{
					retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

					// Process data
					retcode = SQLExecDirect(hstmt, (SQLCHAR*)"SELECT id FROM test ORDER BY id ASC", SQL_NTS);

					if (retcode == SQL_SUCCESS) {
						SQLLEN cbTestStr;
						SQLINTEGER iCount = 1;
						//SQLFLOAT dTestFloat;
						SQLCHAR szTestStr[200];
						while (true) {
							retcode = SQLFetch(hstmt);
							if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
								cout << "An error occurred";
							}
							if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

								SQLGetData(hstmt, 1, SQL_C_CHAR, szTestStr, 200, &cbTestStr);

								/* Print the row of data */
								cout << "Row " << iCount << ":" << endl;
								cout << szTestStr << endl;
								iCount++;
							}
							else {
								break;
							}
						}
					}
					else
					{
						cout << 1 << endl;
						cout << retcode << endl;
					}

					SQLDisconnect(hdbc);
				}
				else
				{
					cout << 2 << endl;
					cout << retcode << endl;
				}

				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
			}
			else
			{
				cout << 3 << endl;
				cout << retcode << endl;
			}
		}
		else
		{
			cout << 4 << endl;
			cout << retcode << endl;
		}

		SQLFreeHandle(SQL_HANDLE_ENV, henv);

	}
	else
	{
		cout << 5 << endl;
		cout << retcode << endl;
	}

	////This happens after connection ------------------------------------------------------------
	////TODO: remove this query in open
	//m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);

	//// Process data
	//m_retcode = SQLExecDirect(m_hstmt, (SQLCHAR*)"SELECT id FROM test ORDER BY id ASC", SQL_NTS);

	//if (m_retcode == SQL_SUCCESS) {
	//	SQLLEN cbTestStr;
	//	SQLINTEGER iCount = 1;
	//	//SQLFLOAT dTestFloat;
	//	SQLCHAR szTestStr[200];
	//	while (true) {
	//		m_retcode = SQLFetch(m_hstmt);
	//		if (m_retcode == SQL_ERROR || m_retcode == SQL_SUCCESS_WITH_INFO) {
	//			cout << "An error occurred";
	//		}
	//		if (m_retcode == SQL_SUCCESS || m_retcode == SQL_SUCCESS_WITH_INFO) {

	//			SQLGetData(m_hstmt, 1, SQL_C_CHAR, szTestStr, 200, &cbTestStr);

	//			/* Print the row of data */
	//			cout << "Row " << iCount << ":" << endl;
	//			cout << szTestStr << endl;
	//			iCount++;
	//		}
	//		else {
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	cout << 1 << endl;
	//	cout << m_retcode << endl;
	//}

	//SQLDisconnect(m_hdbc);
	//m_isOpen = false;
}
