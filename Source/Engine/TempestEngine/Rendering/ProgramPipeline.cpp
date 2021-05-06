/*!***************************************************************************************
\file       ShaderProgram.cpp
\author     Cannell, Cody
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A CPU representation of the GPU program object that hold
            all shaders for pipline.
*****************************************************************************************/
#include "../Precompiled.h"


//========Self Include==================================================================//

//========1st Party Includes============================================================//
#include "TypeData.hpp"
#include "Color.hpp"
#include "GLDebug.hpp"
#include "ProgramPipeline.hpp"
#include "../Utility/AssetManager.hpp"


//========3rd Party Includes============================================================//
#include <Logger.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>
#include <Matrix4x4.hpp>

#include <GL/glew.h>
//========Types=========================================================================//


//========Defines=======================================================================//
//========Static Deceleration===========================================================//

  ///////========================================================================///////
  ///////   Public                                                               ///////
  ///////========================================================================///////

    //==============================================================================//
    //        Constructor                                                           //
    //==============================================================================//
programPipeline::programPipeline(assetHandle p_vertexName, assetHandle p_fragmentName) :  m_VertexShader(p_vertexName), m_fragmentShader(p_fragmentName)
{
}
//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//
void programPipeline::setUniforms(GLuint p_handle, const std::string  & p_name, vector4 p_vec4)
{
    glBindProgramPipeline(m_pipelineHandle);
    // TODO(cody): store location for later use.
    int location = glGetUniformLocation(p_handle, p_name.c_str());
     if(location < 0)
         logger("ShaderProgram").debug() << p_name << ": " << p_vec4.toStringPtr() << " loc: " << location;


    glProgramUniform4fv(p_handle, location, 1, p_vec4.toFloatPtr());

    GLDebug::getLastError();
}

void programPipeline::setUniforms(GLuint p_handle, const std::string  & p_name, color p_colr)
{
    // TODO(cody): store location for later use.
    glBindProgramPipeline(m_pipelineHandle);
    int location = glGetUniformLocation(p_handle, p_name.c_str());
    if (location < 0)
        logger("ShaderProgram").debug() << p_name << ": " << p_colr.toStringPtr() << " loc: " << location;
    glProgramUniform4fv(p_handle, location, 1, p_colr.toFloatPtr());

    GLDebug::getLastError();
}

void programPipeline::setUniforms(GLuint p_handle, const std::string & p_name, vector3 p_vec3)
{
    glBindProgramPipeline(m_pipelineHandle);
    //TODO(cody) store location for later use.
    int location = glGetUniformLocation(p_handle, p_name.c_str());
     if (location < 0)
         logger("ShaderProgram").debug() << p_name << ": " << p_vec3.toStringPtr() << " loc: " << location;
     //GLDebug::getLastError();
    glProgramUniform3fv(p_handle, location, 1, p_vec3.toFloatPtr());

    GLDebug::getLastError();

}

void programPipeline::setUniforms(GLuint p_handle, const std::string & p_name, matrix4x4 p_mat4)
{
    glBindProgramPipeline(m_pipelineHandle);
    // TODO(cody): store location for later use.    
    int location = glGetUniformLocation(p_handle, p_name.c_str());
    if (location < 0)
       logger("ShaderProgram").debug() << p_name << ": " << p_mat4.toStringPtr() << " loc: " << location;
    //GLDebug::getLastError();
    glProgramUniformMatrix4fv(p_handle, location, 1, GL_TRUE, p_mat4.toFloatPtr());

    GLDebug::getLastError();
}

void programPipeline::setUniforms(GLuint p_handle, std::string const & p_name, int p_value)
{
    glBindProgramPipeline(m_pipelineHandle);
    // TODO(cody): store location for later use.    
    // uploads the raw integer value to the GPU
    int location = glGetUniformLocation(p_handle, p_name.c_str());
    if (location < 0)
        logger("ShaderProgram").debug() << p_name << ": " << p_value << " loc: " << location;
    //GLDebug::getLastError();
    glProgramUniform1i(p_handle, location, p_value);

    GLDebug::getLastError();
}



void programPipeline::setUniforms(GLuint p_handle, std::string const & p_name, float p_value)
{
    glBindProgramPipeline(m_pipelineHandle);
    // TODO(cody): store location for later use.    
    // uploads the raw integer value to the GPU
    int location = glGetUniformLocation(p_handle, p_name.c_str());
    if (location < 0)
    {
        logger("ShaderProgram").debug() << p_name << ": " << p_value << " loc: " << location;
    }
    glProgramUniform1f(p_handle, location, p_value);

    GLDebug::getLastError();
}



shader const * programPipeline::getShader(shaderType p_type)
{

    switch (p_type)
    {
    case shaderType::enm_vert:
        return &m_VertexShader;
        break;

    case shaderType::enm_frag:
        return &m_fragmentShader;
        break;
    default:
        return nullptr;

    }
}

//////======================================================================//////
//////    Non-Static                                                        //////
/////     Functions                                                          /////
/////========================================================================/////
void programPipeline::process()
{
    //std::stringstream ssv;
    //ssv << ASSET_PATH << "shaders/" << m_VertexShader.getFileName();
    //std::stringstream ssf;
    //ssf << ASSET_PATH << "shaders/" << m_fragmentShader.getFileName();

    //m_VertexShader.setSourceData();
    //m_fragmentShader.setSourceData(ReadInFile(ssf.str()));
}

void programPipeline::build()
{


     // change source data for shaders
    const GLchar * l_vertStr = m_VertexShader.getSourceData().c_str();
    const GLchar * l_fragStr = m_fragmentShader.getSourceData().c_str();


    //m_vertexProgramHandle = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &l_vertStr);

    //logger("GLDebug").debug() << "ERROR::VERTEXPROGRAM::COMPILATION_FAILED\n";
    //GLDebug::GetLastProgramError(m_vertexProgramHandle);
    //logger("GLDebug").debug() << "ERROR::VERTEXPROGRAM::LINKER_FAILED\n";
    //GLDebug::GetLastProgramLinkError(m_pipelineHandle);

    //m_fragmentProgramHandle = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &l_fragStr);

    //logger("GLDebug").debug() << "ERROR::FRAGMENTPROGRAM::COMPILATION_FAILED\n";
    //GLDebug::GetLastProgramError(m_fragmentProgramHandle);
    //logger("GLDebug").debug() << "ERROR::FRAGMENTPROGRAM::LINKER_FAILED\n";
    //GLDebug::GetLastProgramLinkError(m_fragmentProgramHandle);

    //glGenProgramPipelines(1, &m_pipelineHandle);
    //glBindProgramPipeline(m_pipelineHandle);
    //glUseProgramStages(m_pipelineHandle, GL_VERTEX_SHADER_BIT, m_vertexProgramHandle);
    //glUseProgramStages(m_pipelineHandle, GL_FRAGMENT_SHADER_BIT, m_fragmentProgramHandle);

    //logger("GLDebug").debug() << "ERROR::PROGRAMPIPELINE::COMPILATION_FAILED\n";
    //GLDebug::GetLastProgramPipelineError(m_pipelineHandle);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Create two programs. One with just the vertex shader, and 
// one with both geometry and fragment stages.
    GLuint vertexProgram = glCreateProgram();
    GLDebug::GetLastProgramError(vertexProgram);
    GLuint FragmentProgram = glCreateProgram();
    GLDebug::GetLastProgramError(FragmentProgram);


    // Declare that programs are separable - this is crucial!
    glProgramParameteri(vertexProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glProgramParameteri(FragmentProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

    // Generate and compile shader objects, as normal.
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    //GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &l_vertStr, NULL);
  
    glShaderSource(fragShader, 1, &l_fragStr, NULL);

    glCompileShader(vertShader);

    GLDebug::GetLastShaderError(vertShader);
    //glCompileShader(geomShader);
    glCompileShader(fragShader);

    GLDebug::GetLastShaderError(fragShader);


    int shaderCount;
    // Attach the shaders to their respective programs
    glAttachShader(vertexProgram, vertShader);
    glGetProgramiv(vertexProgram, GL_ATTACHED_SHADERS, &shaderCount);
    //glAttachShader(geomFragProgram, geomShader);
    glAttachShader(FragmentProgram, fragShader);
    glGetProgramiv(FragmentProgram, GL_ATTACHED_SHADERS, &shaderCount);

    // Perform any pre-linking steps.
    //glBindAttribLocation(vertexProgram, 0, "Position");
    //glBindFragDataLocation(FragmentProgram, 0, "FragColor");

    // Link the programs
    glLinkProgram(vertexProgram);
    GLDebug::GetLastProgramLinkError(vertexProgram);
    glLinkProgram(FragmentProgram);
    GLDebug::GetLastProgramLinkError(FragmentProgram);

    // Detach and delete the shader objects
    glDetachShader(vertexProgram, vertShader);
    glDeleteShader(vertShader);

    
    glDetachShader(FragmentProgram, fragShader);
    //glDeleteShader(geomShader);
    glDeleteShader(fragShader);

    // Generate a program pipeline
    glGenProgramPipelines(1, &m_pipelineHandle);
    GLDebug::GetLastProgramPipelineError(m_pipelineHandle);

    // Attach the first program to the vertex stage, and the second program
    // to the geometry and fragment stages
    glUseProgramStages(m_pipelineHandle, GL_VERTEX_SHADER_BIT, vertexProgram);
    glUseProgramStages(m_pipelineHandle, GL_FRAGMENT_SHADER_BIT, FragmentProgram);
    

    glValidateProgramPipeline(m_pipelineHandle);

    GLint l_success = 0;
    glGetProgramPipelineiv(m_pipelineHandle, GL_VALIDATE_STATUS, &l_success);

}

void programPipeline::bindProgramPipeline() const
{
    // logger("ShaderProgram").debug() << "bind ProgrameHnadle: " << m_handle;
    glBindProgramPipeline(m_pipelineHandle);
    //GLDebug::getLastError();
}

void programPipeline::setActiveProgram(int p_index)
{
    glActiveShaderProgram(m_pipelineHandle, p_index);
}


void programPipeline::bindProgram(int p_index)
{
    glUseProgram(p_index);
}


void programPipeline::unbind() const
{
    glBindProgramPipeline(NULL);
}

///////========================================================================///////
  ///////   Private                                                              ///////
  ///////========================================================================///////

    //////======================================================================//////
    //////    Non-Static                                                        //////
    //////======================================================================//////

    /////========================================================================/////
    /////     Functions                                                          /////
    /////========================================================================/////

    ////==========================================================================////
    ////      Non-Virtual                                                         ////
    ////==========================================================================////

    //==============================================================================//
    //        Helper                                                                //
    //==============================================================================//









