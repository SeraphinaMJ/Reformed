/*!***************************************************************************************
\file       DataTypes.cpp
\author     Michael-Paul Moore
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Functionality for your utility belt.
*****************************************************************************************/

//========Self Include==================================================================//
#include "DataTypes.hpp"
#define d_objIDLength 12

//========3rd Party Includes============================================================//
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <regex>

#ifdef _WIN32
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
//#include <windows.h>
#else
#include <uuid/uuid.h>
#endif

namespace Utilities 
{

}
std::string ReadInFile(const std::string &path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::stringstream buffer;
		buffer << file.rdbuf();
		return  buffer.str();
}
/*!***************************************************************************************
		\brief  Creates new UUID
		\return unsigned long objID - new ID
*****************************************************************************************/
objID getNextID()
{
#ifdef _WIN32
	UUID l_uuid;
	UuidCreate(&l_uuid);
	return l_uuid.Data1;
#else
    uuid_t l_uuid;

    // uuid_t _is_ a pointer type already, so just pass it in as-is
    uuid_generate(l_uuid);

    // This makes sure we grab the first bunch of bytes that match the size of
    //  objID
    // See /usr/include/uuid/uuid.h for the definition of uuid_t.
    return *((objID*)&(l_uuid[0]));
#endif
}

/*!***************************************************************************************
		\brief  Takes an objID and coverts it to a std::string
		\param p_id - The objId to convert
		\return std::string
*****************************************************************************************/
std::string objIDtoString(objID p_id)
{
	std::string l_objString = std::to_string(p_id);
	if (l_objString.length() < d_objIDLength)
	{
		std::string l_newString;
		int l_appendAmount = d_objIDLength - static_cast<int>(l_objString.length());
		for (int i = 0; i < l_appendAmount; ++i)
		{
			l_newString.append("0");
		}
		l_objString = l_newString.append(l_objString);
	}
	return l_objString;
}

std::chrono::duration<int, std::nano> FPStoFrequency(fps p_fps)
{
	if (p_fps)
	{
		const int nano_seconds_in_a_second = 1000000000;
		const std::chrono::duration<int, std::nano> dur_nano_sec(nano_seconds_in_a_second);
		return dur_nano_sec / p_fps;
	}

	return {};
}

std::string getFileNameFromAssetPath(const std::string &p_path)
{
	std::regex l_r("(.*/)?([A-z0-9]*)/([A-z0-9]*.[A-z0-9]*)");


	//std::regex l_r("(.*\\\\)?([A-z0-9]*)\\\\([A-z0-9]*)");

	std::smatch l_match;

	std::regex_search(p_path, l_match, l_r);

	return l_match[3];
}
