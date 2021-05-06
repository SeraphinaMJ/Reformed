/*!***************************************************************************************
\file       GLDebug.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/
#include "../Precompiled.h"
#include "GLDebug.hpp"
#include <Logger.hpp>
#include <GL/glew.h>

static void GLAPIENTRY message_callback(
	GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	GLchar const* message, void const* user_param);

/*!***************************************************************************************
\brief  Prints Error from defines
\param error - error number
\return string of the error
*****************************************************************************************/

const char* GLErrorToString(GLenum error)
{
    switch (error)
    {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return "unknown error";
    }
}

const char* GLFramebufferErrorToString(GLenum error)
{
    switch (error)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        return "GL_FRAMEBUFFER_COMPLETE";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case GL_FRAMEBUFFER_UNSUPPORTED:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
    default:
        return "unknown error";
    }
}


template<typename getFunc, typename infoLogFunc>
void logGLError(getFunc getter, infoLogFunc logging, const GLuint p_handle, std::string_view name)
{

	auto log = logger("GLDebug").debug();
	log << "in " << name << '\n'
		<< GLErrorToString(glGetError()) << '\n';

	const auto getLength = [getter, p_handle]
	{
		GLint l_length;
		getter(p_handle, GL_INFO_LOG_LENGTH, &l_length);
		return l_length;
	};

	const auto getLog = [logging, p_handle](const GLint p_length)-> std::string
	{
		std::string l_infoLog(p_length, '\0');
		GLint amountWrote;
		logging(p_handle, l_infoLog.length(), &amountWrote, l_infoLog.data());
		l_infoLog.resize(amountWrote);
		return l_infoLog;
	};

	log << "ERROR::PROGRAM::COMPILATION_FAILED\n" << getLog(getLength());
}


void logPipelineError(const GLint p_handle, std::string_view p_name)
{
	logGLError(glGetProgramPipelineiv, glGetProgramPipelineInfoLog, p_handle, p_name);
}


void logShaderError(const GLint p_handle, std::string_view p_name)
{
	logGLError(glGetShaderiv, glGetShaderInfoLog, p_handle, p_name);
}

void logProgramError(const GLint p_handle, std::string_view p_name)
{
	logGLError(glGetProgramiv, glGetProgramInfoLog, p_handle, p_name);
}

void GLDebug::GetLastShaderError(GLint p_handle, std::string_view p_name, std::string p_info)
{
    GLint l_success = 0;
    glGetShaderiv(p_handle, GL_COMPILE_STATUS, &l_success);
    if (l_success == GL_FALSE)
    {
		logShaderError(p_handle, p_name);

		auto log = logger("GLDebug").error();
		std::stringstream ss;
		ss << std::move(p_info);

		std::string line;
		size_t num = 0;
		log << "Shader source with line number information:\n";
		while (std::getline(ss, line))
			log << ++num << ' ' << std::move(line) << '\n';
    }
    else
    {
        logger("GLDebug").debug() << "SHADER_" << p_handle << ":no error\n";
    }
}


void GLDebug::InitializeDebugCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	GLuint unusedIds{ 0 };
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&message_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
		&unusedIds, true);
}

void GLDebug::GetLastProgramError(const GLint p_handle, std::string_view p_name)
{
    GLint l_success = 0;

    glGetProgramiv(p_handle, GL_COMPILE_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
		logProgramError(p_handle, p_name);
	}
}

void GLDebug::GetLastProgramLinkError(const GLint p_handle, std::string_view p_name)
{
    GLint l_success = 0;
    glGetProgramiv(p_handle, GL_LINK_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
		logProgramError(p_handle, p_name);
    }
}

void GLDebug::GetLastProgramPipelineError(GLint p_handle, std::string_view p_name)
{
    GLint l_success = 0;

    glGetProgramPipelineiv(p_handle, GL_COMPILE_STATUS, &l_success);

    if (l_success == GL_FALSE)
    {
		logPipelineError(p_handle, p_name);
    }
}



void GLDebug::getLastError()
{
	if (const auto error{ glGetError() }; error != GL_NO_ERROR)
	{
		logger("GLDebug").debug() << GLErrorToString(error);
	}
}


void GLDebug::getLastFrameBufferError()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_NO_ERROR) return;

    logger("Framebuffer").debug() << GLFramebufferErrorToString(status);

}

 static void GLAPIENTRY message_callback(
	 const GLenum source, const GLenum type, const GLuint id, 
	 const GLenum severity, const GLsizei length, GLchar const*const message,
	 void const*const user_param)
{
	 if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // verbose debug information
	 if (id == 131218) return; // NVIDIA vendor specific: recompilation based on GLstate
	UNREFERENCED_PARAMETER(length);
	UNREFERENCED_PARAMETER(user_param);
	auto const src_str = [source]() {
		switch (source)
		{
		case GL_DEBUG_SOURCE_API: return "API";
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
		case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
		case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
		case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
		case GL_DEBUG_SOURCE_OTHER: return "OTHER";
		default: return "No_Error";
		}
	}();

	auto const type_str = [type]() {
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR: return "ERROR";
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
		case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
		case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
		case GL_DEBUG_TYPE_MARKER: return "MARKER";
		case GL_DEBUG_TYPE_OTHER: return "OTHER";
		default: return "No_Error";
		}
	}();

	auto const severity_str = [severity]() {
		switch (severity) {
		case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
		case GL_DEBUG_SEVERITY_LOW: return "LOW";
		case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
		case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
		default: return "No_Error";
		}
	}();

	logger("GLDebug").debug() << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message;
}
