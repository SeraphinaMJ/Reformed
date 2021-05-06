/*!***************************************************************************************
\file       Logger.hpp
\author     Aaron Damyen
\date       6/3/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This file contains the definition for a basic logging system.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>
#include <fstream>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: logger
\brief   This is a simple logger class that writes to the Visual Studio Output tab.
*****************************************************************************************/
class logger final
{
	// This has to be first so that it is:
	//    a) still private, and
	//    b) accessible from loggerProxy
	enum class Level {
		LLEVEL_NONE,
		LLEVEL_INFO,
		LLEVEL_DEBUG,
		LLEVEL_WARN,
		LLEVEL_ERROR
	};

	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	static void setLogFileLocation(const std::filesystem::path&);
	static void setQuiet(bool);
	static void setVerbose(bool);

	static bool isLevelEnabled(Level level);

    ///////==========================================================================///////
    ///////    Helper class                                                          ///////
    ///////==========================================================================///////

	/*!***************************************************************************************
	\par class: loggerProxy
	\brief   This class wraps a stringstream to facilitate the operator<< methods desired, but
	         still allowing control of the destruction process (which actually writes the data).
	*****************************************************************************************/
    class loggerProxy : public std::stringstream
    {
    public:
		/*!***************************************************************************************
		\brief  Creates a new proxy with a specific logging level.
		\param p_logger - The logger object that should perform the output.
		\param p_level - The level this proxy should filter.
		*****************************************************************************************/
		loggerProxy(logger* p_logger, Level p_level)
    		: level(p_level), m_logger{ p_logger }
    	{}

		/*!***************************************************************************************
		\brief  Creates a new proxy that should mimic another.
		\param p_other - The other proxy to mimic
		*****************************************************************************************/
		loggerProxy(const loggerProxy & p_other)
    		: level(p_other.level), m_logger{ p_other.m_logger }
		{
			*this << p_other.str();
		}

		/*!***************************************************************************************
		\brief  Destroys the proxy, flushing any messages to the output stream.
		*****************************************************************************************/
		~loggerProxy() override;

    private:
        Level level; //!< The logging threshold for this proxy object.

		logger* m_logger; //!< Pointer back to the original logger so we can actually write the message
    };

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Constructs a logger object which can write to the streams.
	\param p_system_designator - the text that will appear at the beginning of every line written
	                with this logger object.
	*****************************************************************************************/
    /*TEMPESTENGINE_API*/ explicit logger(std::string p_system_designator);

	/*!***************************************************************************************
	\brief  Retrieves a proxy to the info level logger stream.
	\return loggerProxy - Proxy object representing the stream.
	*****************************************************************************************/
	loggerProxy info();

	/*!***************************************************************************************
	\brief  Retrieves a proxy to the debug level logger stream.
	\return loggerProxy - Proxy object representing the stream.
	*****************************************************************************************/
	loggerProxy debug();

	/*!***************************************************************************************
	\brief  Retrieves a proxy to the warn level logger stream.
	\return loggerProxy - Proxy object representing the stream.
	*****************************************************************************************/
	loggerProxy warn();

	/*!***************************************************************************************
	\brief  Retrieves a proxy to the error level logger stream.
	\return loggerProxy - Proxy object representing the stream.
	*****************************************************************************************/
    /*TEMPESTENGINE_API*/ loggerProxy error();


	/*!***************************************************************************************
	\brief  Writes a single message to the info level logger stream.
	\param p_message - The message to be written.
	*****************************************************************************************/
	void info(std::string p_message);

	/*!***************************************************************************************
	\brief  Writes a single message to the debug level logger stream.
	\param p_message - The message to be written.
	*****************************************************************************************/
	void debug(std::string p_message);

	/*!***************************************************************************************
	\brief  Writes a single message to the warn level logger stream.
	\param p_message - The message to be written.
	*****************************************************************************************/
	void warn(std::string p_message);

	/*!***************************************************************************************
	\brief  Writes a single message to the error level logger stream.
	\param p_message - The message to be written.
	*****************************************************************************************/
	/*TEMPESTENGINE_API*/ void error(std::string p_message);

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:
	int to_int(Level);
	std::string to_string(Level);
	std::wstring to_wstring(Level);

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Generic method to actually output a message (ANSI) prepended with the logging level
	\param p_level - The logging level to be prepended to the message.
	\param p_message - The message to be output.
	*****************************************************************************************/
	void message(Level p_level, std::string p_message);

    /*!***************************************************************************************
    \brief  Generic method to actually output a message (Unicode) prepended with the logging level
    \param p_level - The logging level to be prepended to the message.
    \param p_message - The message to be output.
    *****************************************************************************************/
    void message(Level p_level, std::wstring p_message);

	std::wstring m_tag_ws;
	std::string m_tag_s;

	static std::filesystem::path m_logfile_path;
	static std::ofstream m_logfile;

	static logger m_internal_logger;

	static bool m_quiet;
	static bool m_verbose;
};

