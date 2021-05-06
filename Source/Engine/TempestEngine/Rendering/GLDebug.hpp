/*!***************************************************************************************
\file       GLDebug.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#pragma once

#include "GLTypes.hpp"

#include <string_view>

//#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }

struct GLDebug
{
	static constexpr const char* default_str = "unknown source";
	static void InitializeDebugCallback();
	/*!***************************************************************************************
	\brief  Gets the last error of program
	\param  p_handle - Handle to the program object
	*****************************************************************************************/
	static void GetLastProgramError(GLint p_handle, std::string_view p_name = default_str);

	/*!***************************************************************************************
	\brief  Gets the last error
	\param  p_handle - handle to the GPU shader object
	*****************************************************************************************/
	static void GetLastShaderError(GLint p_handle, 
		std::string_view p_name = default_str,
		std::string p_info = default_str);

	static void GetLastProgramLinkError(GLint, std::string_view p_name = default_str);

	static void GetLastProgramPipelineError(GLint p_handle, std::string_view p_name = default_str);

	static void getLastError();

	static void getLastFrameBufferError();
};
