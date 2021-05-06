/*!***************************************************************************************
\file       Logger.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

//========1st Party Includes============================================================//
#include "Logger.hpp"

//========3rd Party Includes============================================================//
#include <locale>
#include <codecvt>

#ifdef _WIN32
#include <Windows.h>
#else
#include <syslog.h>
#endif

//========Forward Deceleration==========================================================//

std::filesystem::path logger::m_logfile_path;
std::ofstream logger::m_logfile;

bool logger::m_quiet = false;
bool logger::m_verbose = false;

logger logger::m_internal_logger("logger");

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////
logger::loggerProxy::~loggerProxy()
{
    m_logger->message(level, this->str().c_str());
}

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////


void logger::setLogFileLocation(const std::filesystem::path& logfile_location) {
	using namespace std::string_literals;

	m_logfile_path = logfile_location;
	if(!m_logfile_path.empty()) {
		if(m_logfile)
			m_logfile.close();
		// If the parent paths up to here don't exist yet, try to create them
		if(auto parent_path = m_logfile_path.parent_path(); !std::filesystem::exists(parent_path))
			std::filesystem::create_directories(parent_path);
		m_logfile.open(m_logfile_path);
		if(!m_logfile.is_open())
			m_internal_logger.error("Could not open logfile at "s + m_logfile_path.generic_string());
	} else {
		if(m_logfile)
			m_logfile.close();
	}
}

void logger::setQuiet(bool quiet) {
	m_quiet = quiet;
}

void logger::setVerbose(bool verbose) {
	m_verbose = verbose;
}


bool logger::isLevelEnabled(Level p_level) {
	switch(p_level) {
		// NONE and ERROR are always enabled
		case Level::LLEVEL_NONE:
		case Level::LLEVEL_ERROR:
			return true;

		// WARN and INFO are enabled only if quiet is not
		case Level::LLEVEL_WARN:
		case Level::LLEVEL_INFO:
			return !m_quiet;

		// DEBUG is only enabled if quiet is not and verbose is
		case Level::LLEVEL_DEBUG:
			return !m_quiet && m_verbose;
	}
}

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////


logger::logger(std::string p_system_designator)
	: m_tag_ws{p_system_designator.begin(),p_system_designator.end()},
	  m_tag_s{p_system_designator}
{
#ifndef _WIN32
	openlog(p_system_designator.c_str(), LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER);
#endif
}

logger::loggerProxy logger::info()
{
    return loggerProxy(this, Level::LLEVEL_INFO);
}

logger::loggerProxy logger::debug()
{
    return loggerProxy(this, Level::LLEVEL_DEBUG);
}

logger::loggerProxy logger::warn()
{
    return loggerProxy(this, Level::LLEVEL_WARN);
}

logger::loggerProxy logger::error()
{
    return loggerProxy(this, Level::LLEVEL_ERROR);
}

void logger::info(std::string p_message)
{
    logger::message(Level::LLEVEL_ERROR, p_message);
}

void logger::debug(std::string p_message)
{
    logger::message(Level::LLEVEL_DEBUG, p_message);
}

void logger::warn(std::string p_message)
{
    logger::message(Level::LLEVEL_WARN, p_message);
}
void logger::error(std::string p_message)
{
    logger::message(Level::LLEVEL_ERROR, p_message);
}

void logger::message(Level p_level, std::string p_message)
{
	if(!isLevelEnabled(p_level)) return;

#ifdef _WIN32
    // convert the std::string to a wide string (std::wstring)
    // std::wstring l_wide_level = to_wstring(p_level);
    std::wstring l_wide_message = std::wstring(p_message.begin(), p_message.end());
    message(p_level, l_wide_message);
#else
	syslog(to_int(p_level), "%s", p_message.c_str());

	if(m_logfile.is_open()) {
		m_logfile << ": " << to_string(p_level) << " - " << m_tag_s << p_message << std::endl;
	}
#endif
}

void logger::message(Level p_level, std::wstring p_message)
{
	if(!isLevelEnabled(p_level)) return;

	std::wstringstream l_stream;
	l_stream << m_tag_ws << ": " << to_wstring(p_level) << " - " << p_message;

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> l_converter;

	auto l_ws = l_stream.str();
	std::string l_s = l_converter.to_bytes(l_ws);

#ifdef _WIN32
	l_ws.push_back('\n');
    OutputDebugStringW(l_ws.c_str());

	if(m_logfile.is_open()) {
		m_logfile << l_s << std::endl;
	}

#else
    message(p_level, l_s.c_str());
#endif
}

int logger::to_int(Level p_level)
{
#ifndef _WIN32
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return LOG_INFO;
		case Level::LLEVEL_WARN:
			return LOG_WARNING;
		case Level::LLEVEL_ERROR:
			return LOG_ERR;
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return LOG_DEBUG;
	}
#else
	return 0;
#endif
}

std::string logger::to_string(Level p_level)
{
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return "Info";
		case Level::LLEVEL_WARN:
			return "Warn";
		case Level::LLEVEL_ERROR:
			return "Error";
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return "Debug";
	}
}

std::wstring logger::to_wstring(Level p_level)
{
	switch(p_level)
	{
		case Level::LLEVEL_INFO:
			return L"Info";
		case Level::LLEVEL_WARN:
			return L"Warn";
		case Level::LLEVEL_ERROR:
			return L"Error";
		case Level::LLEVEL_DEBUG:
		case Level::LLEVEL_NONE:
		default:
			return L"Debug";
	}
}

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Oveided/(Non-/Pure-)Virtual                                                 ////
////==================================================================================////

//======================================================================================//
//        Helper / Constructor / Destructor                                            ///
//======================================================================================//

//======================================================================================//
//        Other                                                                         //
//======================================================================================/
