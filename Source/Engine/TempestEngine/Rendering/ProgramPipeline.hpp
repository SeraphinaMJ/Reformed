/*!***************************************************************************************
\file       ShaderProgram.hpp
\author     Cannell, Cody
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A CPU representation of the GPU program object that hold
            all shaders for pipline.
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
#include "Shader.hpp"
//========3rd Party Includes============================================================//
#include "TypeData.hpp"
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
class color;
class matrix4x4;
struct vector3;
struct vector4;


////////==========================================================================////////
////////   Class                                                                  ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: shaderProgram
\brief   A CPU representation of the GPU program object that hold all shaders for pipline.
*****************************************************************************************/
class programPipeline
{

    ///////============================================================================///////
    ///////   Public                                                                   ///////
    ///////============================================================================///////
public:

    /*!***************************************************************************************
    \brief  Constructor for shader program
    \param  p_assetPath    - The path to the assets folder
    \param  p_vertexName   - The name of the vertex shader file
    \param  p_fragmentName - The name of the fragment shader file.
    *****************************************************************************************/
    programPipeline(assetHandle p_vertexName, assetHandle p_fragmentName);

    /////================================================================================/////
    /////     Functions                                                                  /////
    /////================================================================================/////

    /*!***************************************************************************************
    \brief  process the files(no loger in use)
    *****************************************************************************************/
    void process();

    /*!***************************************************************************************
    \brief  creates the shaderprogram on the GPU
    *****************************************************************************************/
    void build();

    /*!***************************************************************************************
    \brief  binds the current shader program to the GPU shaderprogram
    *****************************************************************************************/
    void bindProgramPipeline() const;



    /*!***************************************************************************************
    \brief  unbinds the current shader program from the GPU shaderprogram
    *****************************************************************************************/
    void unbind() const;

    /*!***************************************************************************************
 \brief  Sets Vec4 Uniforms on the shader
 \param  p_name - The name of the uniform
 \param  p_colr - The color to be loaded in
 *****************************************************************************************/
    void setUniforms(GLuint p_handle, const std::string  & p_name, color p_colr);

    /*!***************************************************************************************
 \brief  Sets Vec4 Uniforms on the shader
 \param  p_name - The name of the uniform
 \param  p_vec3 - The vec3 to be loaded in
 *****************************************************************************************/
    void setUniforms(GLuint p_handle, const std::string & p_name, vector3 p_vec3);

    /*!***************************************************************************************
    \brief  Sets Vec4 Uniforms on the shader
    \param  p_name - The name of the uniform
    \param  p_vec4 - The vec4 to be loaded in
    *****************************************************************************************/
    void setUniforms(GLuint p_handle, const std::string & p_name, vector4 p_vec4);

    /*!***************************************************************************************
    \brief  Sets Vec4 Uniforms on the shader
    \param  p_name - The name of the uniform
    \param  p_mat4 - The mat4 to be loaded in
    *****************************************************************************************/
    void setUniforms(GLuint p_handle, const std::string & p_name, matrix4x4 p_mat4);

    /*!***************************************************************************************
    \brief  Sets int Uniforms on the shader
    \param  p_name - The name of the uniform
    \param  p_value - The int to be loaded in
    *****************************************************************************************/
    void setUniforms(GLuint p_handle, std::string const &p_name, int p_value);

    /*!***************************************************************************************
    \brief  Sets float Uniforms on the shader
    \param  p_name - The name of the uniform
    \param  p_value - The float to be loaded in
    *****************************************************************************************/
    void setUniforms(GLuint p_handle, std::string const & p_name, float p_value);

    void setActiveProgram(int p_index);


    void bindProgram(int p_index);

    /*!***************************************************************************************
    \brief  Get a shader for the program
    \param  p_type - Type of shader
    \return The shader
    *****************************************************************************************/
    shader const * getShader(shaderType p_type);


    ///////============================================================================///////
    ///////     Private                                                                ///////
    ///////============================================================================///////
private:

    /////================================================================================/////
    /////      Data                                                                      /////
    /////================================================================================/////
    //GLint m_handle;           //!<  The handle to the GPU program
    shader m_VertexShader;    //!<  The vertex shader
    shader m_fragmentShader;  //!<  The fragment shader
    GLuint m_pipelineHandle{};
    GLuint m_vertexProgramHandle{};
    GLuint m_fragmentProgramHandle{};


   // std::unordered_map<std::string, unsigned> uniforms, attributes;

};

