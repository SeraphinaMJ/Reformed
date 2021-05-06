/*!***************************************************************************************
\file       ShaderProgram.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A CPU representation of the GPU program object that hold 
            all shaders for pipline.
*****************************************************************************************/
#include "../Precompiled.h"


//========Self Include==================================================================//
#include "ShaderProgram.hpp"
//========1st Party Includes============================================================//
#include "TypeData.hpp"
#include "Color.hpp"
#include "GLDebug.hpp"

//========3rd Party Includes============================================================//
#include <Logger.hpp>
#include <Vector2.hpp>
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
    shaderProgram::shaderProgram(assetHandle p_vertAsset, assetHandle p_fragAsset) 
		: m_handle(0)
		, m_VertexShader(p_vertAsset)
		, m_fragmentShader(p_fragAsset)
    {
	}
	shaderProgram::~shaderProgram()
	{
		shutdown();
	}
	//==============================================================================//
	//        Destructor                                                            //
	//==============================================================================//

	//==============================================================================//
	//        Getters & Setters                                                     //
	//==============================================================================//

	// recommendation for using this template: use the glUniform call on its own
	// before calling this function instead
	// glUniform1i(location, value) becomes checkedUniformSetter(glUniform1i, location, value)
	template<typename Callable, typename ... Args>
	void checkedUniformSetter(Callable setter, const int loc, Args&&...args)
	{
		if (loc >= 0) setter(loc, std::forward<Args>(args)...);
	}

    void shaderProgram::setUniforms(std::string_view p_name, vector4 const& p_vec4)
    {
        checkedUniformSetter(glUniform4fv, getUniformLocation(p_name), 1, p_vec4.toFloatPtr());
    }

	void shaderProgram::setUniforms(std::string_view p_name, std::array<unsigned, 4> const& p_uvec4)
	{
		checkedUniformSetter(glUniform4uiv, getUniformLocation(p_name), 1, p_uvec4.data());
	}

    void shaderProgram::setUniforms(std::string_view p_name, color const& p_colr)
    {
		checkedUniformSetter(glUniform4fv, getUniformLocation(p_name), 1, p_colr.toFloatPtr());
    }

	void shaderProgram::setUniforms(std::string_view p_name, vector2 const& p_vec2)
	{
		checkedUniformSetter(glUniform2fv, getUniformLocation(p_name), 1, p_vec2.toFloatPtr());
	}

    void shaderProgram::setUniforms(std::string_view p_name, vector3 const& p_vec3)
    {
		checkedUniformSetter(glUniform3fv, getUniformLocation(p_name), 1, p_vec3.toFloatPtr());
    }

    void shaderProgram::setUniforms(std::string_view p_name, matrix4x4 const& p_mat4)
    {
		// TODO: this will fail if the matrix is transposed internally (maybe never do that? :p)
		checkedUniformSetter(glUniformMatrix4fv, getUniformLocation(p_name), 1, GL_TRUE, p_mat4.toFloatPtr());
    }

    void shaderProgram::setUniforms(std::string_view p_name, int p_value)
    {
        // uploads the raw integer value to the GPU
		checkedUniformSetter(glUniform1i, getUniformLocation(p_name), p_value);
    }

    void shaderProgram::setUniforms(std::string_view p_name, float p_value)
    {
        // uploads the raw integer value to the GPU
		checkedUniformSetter(glUniform1f, getUniformLocation(p_name), p_value);
    }
    shader const * shaderProgram::getShader(shaderType p_type)
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

	int shaderProgram::getUniformLocation(const std::string_view p_name) const
	{
		if (auto i_iter = uniforms.find(p_name); i_iter != uniforms.end())
		{
			return i_iter->second;
		}

		// failed to find it, now need to create it
		auto l_name = std::string{ p_name };

		int location = glGetUniformLocation(m_handle, l_name.c_str());
		if (location < 0)
		{
			logger("ShaderProgram").debug() << p_name << ":  loc: " << location;
		}

		return uniforms.emplace(std::move(l_name), location).first->second;
	}

    //////======================================================================//////
    //////    Non-Static                                                        //////
    /////     Functions                                                          /////
    /////========================================================================/////
    void shaderProgram::process()
    {
        //std::stringstream ssv;
        //ssv << ASSET_PATH << "shaders/" << m_VertexShader.getFileName();
        //std::stringstream ssf;
        //ssf << ASSET_PATH << "shaders/" << m_fragmentShader.getFileName();

        //m_VertexShader.setSourceData();
        //m_fragmentShader.setSourceData(ReadInFile(ssf.str()));
    }

    void shaderProgram::build()
    {
        // create the shader on the gpu
       //std::cout << GLErrorToString(glGetError()) << std::endl;

       m_VertexShader.setHandle(glCreateShader(GL_VERTEX_SHADER));
       //GLDebug::GetLastShaderError(m_VertexShader.getHandle());
       m_fragmentShader.setHandle(glCreateShader(GL_FRAGMENT_SHADER));
       //GLDebug::GetLastShaderError(m_fragmentShader.getHandle());


        // change source data for shaders
       const GLchar * l_vertStr = m_VertexShader.getSourceData().c_str();
       auto l_vertSize = static_cast<GLint>(m_VertexShader.getSourceData().length());

       glShaderSource(m_VertexShader.getHandle(), 1, &l_vertStr, &l_vertSize);

       const char * l_fragStr = m_fragmentShader.getSourceData().c_str();
       auto l_fragSize = static_cast<GLint>(m_fragmentShader.getSourceData().length());

       glShaderSource(m_fragmentShader.getHandle(), 1, &l_fragStr, &l_fragSize);
      
        //Compile shaders on gpu
       glCompileShader(m_VertexShader.getHandle());
       GLDebug::GetLastShaderError(m_VertexShader.getHandle(), m_VertexShader.getFileName(), m_VertexShader.getSourceData());

       //Error Check


       glCompileShader(m_fragmentShader.getHandle());
       GLDebug::GetLastShaderError(m_fragmentShader.getHandle(), m_fragmentShader.getFileName(), m_fragmentShader.getSourceData());

       // create shader program on gpu
       m_handle = glCreateProgram();

       // Attach shaders to program
	   int shaderCount;
       glAttachShader(m_handle, m_VertexShader.getHandle());
	   glGetProgramiv(m_handle, GL_ATTACHED_SHADERS, &shaderCount);
       glAttachShader(m_handle, m_fragmentShader.getHandle());
	   glGetProgramiv(m_handle, GL_ATTACHED_SHADERS, &shaderCount);

        // Link
       glLinkProgram(m_handle);
       GLDebug::GetLastProgramLinkError(m_handle, m_VertexShader.getFileName() + ' ' +  m_VertexShader.getFileName());
        

        // Delet shaders on gpu
       glDeleteShader(m_VertexShader.getHandle());
       glDeleteShader(m_fragmentShader.getHandle());

			uniforms.clear();
    }

	void shaderProgram::shutdown()
	{
		if (m_handle)
		{
			glDeleteProgram(m_handle);
			m_handle = 0;
		}
	}

	shaderProgram::OOBind shaderProgram::objectBind() const
	{
		return OOBind{ *this };
	}

    void shaderProgram::bind() const
    {
       // logger("ShaderProgram").debug() << "bind ProgrameHnadle: " << m_handle;
        glUseProgram(m_handle);
       // GLDebug::getLastError();
    }

    void shaderProgram::unbind() const
    {
        glUseProgram(0);
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

	shaderProgram::OOBind::OOBind(shaderProgram const& p_prog) noexcept
		: m_prog {p_prog}
	{
		m_prog.bind();
	}

	shaderProgram::OOBind::~OOBind()
	{
		m_prog.unbind();
	}

	size_t shaderProgram::Hasher::operator()(std::string const & toHash) const
	{
		return std::hash<std::string>{}(toHash);
	}

	size_t shaderProgram::Hasher::operator()(std::string_view toHash) const
	{
		return std::hash<std::string_view>{}(toHash);
	}
