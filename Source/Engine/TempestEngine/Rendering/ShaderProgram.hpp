/*!***************************************************************************************
\file       ShaderProgram.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A CPU representation of the GPU program object that hold
            all shaders for pipline.
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
#include "Shader.hpp"
#include "TypeData.hpp"
#include "GLTypes.hpp"
//========3rd Party Includes============================================================//
#include <map>
#include <string>
#include "../Utility/AssetManager.hpp"
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

class color;
struct vector2;
struct vector3;
struct vector4;
class matrix4x4;


////////==========================================================================////////
////////   Class                                                                  ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: shaderProgram
\brief   A CPU representation of the GPU program object that hold all shaders for pipline.
*****************************************************************************************/
class shaderProgram
{
	// fwd decl of private subclass
   class OOBind;

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
    shaderProgram(assetHandle p_vertAsset, assetHandle p_fragAsset);
	~shaderProgram();

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

   void shutdown();
   /*!***************************************************************************************
   \brief  binds the current shader program to the GPU shaderprogram
   *****************************************************************************************/
   void bind() const;

   /*!***************************************************************************************
	\brief  binds the current shader program to the GPU shaderprogram
	\return an object that will automatically unbind the program at the end of its life
	*****************************************************************************************/
   OOBind objectBind() const;
   /*!***************************************************************************************
   \brief  unbinds the current shader program from the GPU shaderprogram
   *****************************************************************************************/
   void unbind() const;

      /*!***************************************************************************************
   \brief  Sets Vec4 Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_colr - The color to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, color const& p_colr);

   /*!***************************************************************************************
	\brief  Sets vec2 Uniforms on the shader
	\param  p_name - The name of the uniform
	\param  p_vec2 - The vec2 to be loaded in
	*****************************************************************************************/
   void setUniforms(std::string_view p_name, vector2 const& p_vec2);

      /*!***************************************************************************************
   \brief  Sets Vec4 Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_vec3 - The vec3 to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, vector3 const& p_vec3);

   /*!***************************************************************************************
   \brief  Sets Vec4 Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_vec4 - The vec4 to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, vector4 const& p_vec4);

   /*!***************************************************************************************
\brief  Sets uVec4 Uniforms on the shader
\param  p_name - The name of the uniform
\param  p_uvec4 - The vec4 to be loaded in
*****************************************************************************************/
   void setUniforms(std::string_view p_name, std::array<unsigned, 4> const& p_uvec4);

   /*!***************************************************************************************
   \brief  Sets Vec4 Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_mat4 - The mat4 to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, matrix4x4 const& p_mat4);

   /*!***************************************************************************************
   \brief  Sets int Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_value - The int to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, int p_value);

   /*!***************************************************************************************
   \brief  Sets float Uniforms on the shader
   \param  p_name - The name of the uniform
   \param  p_value - The float to be loaded in
   *****************************************************************************************/
   void setUniforms(std::string_view p_name, float p_value);

   /*!***************************************************************************************
   \brief  Get a shader for the program
   \param  p_type - Type of shader
   \return The shader
   *****************************************************************************************/
   shader const * getShader(shaderType p_type);

   int getUniformLocation(std::string_view p_name) const;


    ///////============================================================================///////
    ///////     Private                                                                ///////
    ///////============================================================================///////
private:
	class OOBind
	{
	public:
		OOBind(shaderProgram const& p_prog) noexcept;
		~OOBind();

	private:
		shaderProgram const& m_prog;
	};
    /////================================================================================/////
    /////      Data                                                                      /////
    /////================================================================================/////
    GLint m_handle;           //!<  The handle to the GPU program
    shader m_VertexShader;    //!<  The vertex shader
    shader m_fragmentShader;  //!<  The fragment shader

	class Hasher
	{
	public:
		// just wraps around std::hash of these types
		size_t operator()(std::string const& toHash) const;
		size_t operator()(std::string_view toHash) const;
	};

	mutable std::map<std::string, int, std::less<>> uniforms; // , attributes;

};
