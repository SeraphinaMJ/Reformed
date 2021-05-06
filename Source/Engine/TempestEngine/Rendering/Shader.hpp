/*!***************************************************************************************
\file       Shader.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This hold the CPU repersentation of the GPU shaders.
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include "GLTypes.hpp"

#include <string>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

class assetHandle;

/*!***************************************************************************************
\par class: shader
\brief   This hold the CPU repersentation of the GPU shaders.
*****************************************************************************************/
class shader
{
    ///////========================================================================///////
    ///////   Public                                                               ///////
    ///////========================================================================///////
public:

    //==================================================================================//
    //        Constructor                                                               //
    //==================================================================================//

    /*!***************************************************************************************
    \brief  Constructs a shader from a file
	\param  p_assetPath - the root path for all assets
    \param  p_fileName - the name of the file to be loaded in
    *****************************************************************************************/
    shader(assetHandle p_assetHandle);

    /*!***************************************************************************************
    \brief  Defualt Destructor
    *****************************************************************************************/
	~shader() = default;

	shader(shader const&) = delete;
	shader(shader&&) = delete;

	shader& operator=(shader const&) = delete;
	shader& operator=(shader) = delete;

    /*!***************************************************************************************
    \brief  Gets the shaders source file data 
    \return a string ref of the source file
    *****************************************************************************************/
	 [[nodiscard]] std::string & getSourceData() { return m_source; }
	 [[nodiscard]] std::string const& getSourceData() const { return m_source; }

    /*!***************************************************************************************
    \brief  Can Set source data from a string
    \param  p_source_ - the string used to set internal source data
    *****************************************************************************************/
    void setSourceData(std::string p_source) { m_source = std::move(p_source); }

    /*!***************************************************************************************
    \brief  Gets the name of the shader
    \return a copy of the string name
    *****************************************************************************************/
    [[nodiscard]] std::string const& getFileName() const { return m_shaderName; }

    /*!***************************************************************************************
    \brief  sets the shaders handle without destroying the underlying shader object
    \param  p_handle -- number set as the handle
	\return old handle
    *****************************************************************************************/
    [[nodiscard]] GLuint popHandle(GLuint p_handle) noexcept
	{ 
		std::swap(p_handle, m_handle);
		return p_handle;
	}

	/*!***************************************************************************************
	\brief  sets the shaders handle
	\param  p_handle -- number set as the handle
	*****************************************************************************************/
	void setHandle(const GLuint p_handle) noexcept
	{
		m_handle = p_handle;
	}

    /*!***************************************************************************************
    \brief  returns handle number
    \return copy of the handle number
    *****************************************************************************************/
    [[nodiscard]] GLuint getHandle() const { return m_handle; }

    ///////========================================================================///////
    ///////   Private                                                              ///////
    ///////========================================================================///////

	friend void swap(shader& lhs, shader& rhs) noexcept;
	void swap(shader& rhs) noexcept;
private:

    /////============================================================================/////
    /////      Data                                                                  /////
    /////============================================================================/////
    GLuint m_handle = 0;      //!< The handle used to find GPU side shader
    std::string m_source;     //!< hold the source file 
    std::string m_shaderName; //!< the name of the shader for lookup
};
