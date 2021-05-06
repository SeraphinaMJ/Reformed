/*!***************************************************************************************
\file       SimpleTexture.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds all information about a texture
*****************************************************************************************/
//========Self Include==================================================================//
#include "Texture.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#define STB_IMAGE_IMPLEMENTATION
#include <Logger.hpp>
#include <GL/glew.h>

#include "GLDebug.hpp"

#include "SimpleTexture.hpp"

#include <stb_image.h>
#include "../Utility/TextureLoader.hpp"

#include <cstring>
#include <algorithm>
#include <utility> // std::exchange

//#include <stb_image_write.h>

class assetHandle;
static unsigned char const UnbuiltTexture = 0;
static char const UnboundTexture = -1;

//========Types=========================================================================//
//========Defines=======================================================================//
static constexpr unsigned formatToChannels[] =
{
	1,
	3,
	4,
	4
};

static constexpr format channelsToFormat[] =
{
	format::enm_INVALID,
	format::enm_r,
	format::enm_INVALID,
	format::enm_rgb,
	format::enm_rgba
};

static constexpr GLenum formatToGLFormat[] =
{
	GL_RED,
	GL_RGB,
	GL_RGBA,
	GL_RGBA16F
};

int toOffset(format const& f)
{
	return static_cast<int>(f);
}
//========Static Deceleration===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

textureData const& getTexture(assetHandle const& handle)
{
	return handle->getData<textureData>();
}

unsigned getWidth(assetHandle const& handle)
{
	if (!handle || !handle->hasData()) return 0;

	return getTexture(handle).m_width;
}

unsigned getHeight(assetHandle const& handle)
{
	if (!handle || !handle->hasData()) return 0;

	return getTexture(handle).m_height;
}

unsigned getChannels(assetHandle const& handle)
{
	if (!handle || !handle->hasData()) return 0;

	return getTexture(handle).m_bpp;
}

unsigned char* getRawData(assetHandle const& handle)
{
	if (!handle || !handle->hasData()) return nullptr;

	return getTexture(handle).getRaw();
}

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//
simpleTexture::simpleTexture(assetHandle p_assetHandle)
	: m_assetHandle(std::exchange(p_assetHandle, assetHandle{ nullptr }))
	, m_pixelData{ ::getRawData(m_assetHandle) }
	, m_width{ ::getWidth(m_assetHandle) }, m_height{ ::getHeight(m_assetHandle) }
	, m_format{ channelsToFormat[::getChannels(m_assetHandle)] }
{
}

simpleTexture::simpleTexture(unsigned p_width, unsigned p_height, format p_format)
	: m_assetHandle(nullptr)
	, m_pixelData{ nullptr }
	, m_width(p_width), m_height(p_height)
	, m_format{ p_format }
{
}

simpleTexture::~simpleTexture()
{
	shutdown();
}

unsigned simpleTexture::getColorChannels() const
{
	return m_format != format::enm_INVALID ? formatToChannels[toOffset(m_format)] : 0;
}

void simpleTexture::setData(assetHandle p_assetHandle)
{
	auto asset = static_cast<textureData>(p_assetHandle->getData<textureData>());

	m_pixelData = asset.getRaw();
	m_width = asset.m_width;
	m_height = asset.m_height;
	m_format = channelsToFormat[asset.m_bpp];
}

void simpleTexture::initTex()
{
	// generate new texture on GPU
	glGenTextures(1, &m_textureHandle);

	// bind to texture on GPU
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
}


//void simpleTexture::setPixelData(unsigned char* p_data)
//{
//
//
//        // create internal buffer
//        auto pixelData = new unsigned char[m_width* m_height* m_format];
//
//        // copy data into our buffer
//        std::memcpy(pixelData, p_data, m_width * m_height * m_format);
//
//        m_pixelData = pixelData;
//
//
//}

//==============================================================================//
//        Operators                                                             //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

void simpleTexture::shutdown()
{
	unbind();
	glDeleteTextures(1, &m_textureHandle);
	m_textureHandle = 0;
}

void simpleTexture::build()
{
	if (!m_assetHandle || !m_assetHandle->hasData())
	{
		buildNoData();
		return;
	}

	assert(m_format != format::enm_INVALID);

	initTex();

	auto text = m_assetHandle->getData<textureData>().getRaw();
	m_width = m_assetHandle->getData<textureData>().m_width;
	m_height = m_assetHandle->getData<textureData>().m_height;

	const GLenum l_internalformat = formatToGLFormat[toOffset(m_format)];
	const GLenum l_format = m_format != format::enm_rgba_float ? l_internalformat : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(l_internalformat), m_width, m_height, 0, l_format, GL_UNSIGNED_BYTE, text);
	// send texture data

	//GLDebug::getLastError();

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void simpleTexture::buildNoData()
{
	if (m_assetHandle && !m_assetHandle->hasData())
	{
		logger("SimpleTexture").warn() << "No such texture " << m_assetHandle->getFullPath().string();

		m_format = format::enm_r;
		m_width  = 1;
		m_height = 1;
	}

	assert(m_format != format::enm_INVALID);
	initTex();

	const GLenum l_internalformat = formatToGLFormat[toOffset(m_format)];
	const GLenum l_format = m_format != format::enm_rgba_float ? l_internalformat : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(l_internalformat), m_width, m_height, 0, l_format, GL_UNSIGNED_BYTE, nullptr);

	//unbind
	glBindTexture(GL_TEXTURE_2D, 0);
}

void simpleTexture::bind(unsigned int slot)
{
	//http://stackoverflow.com/questions/8866904
	// sets current active object
	glActiveTexture(GL_TEXTURE0 + slot);

	// binds texture to that object
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

	//GLDebug::getLastError();

	// save slot its in
	m_boundSlot.emplace_back(slot);


	//GLDebug::getLastError();

}

void simpleTexture::unbind(unsigned int slot)
{
	auto slotInd = std::find(m_boundSlot.begin(), m_boundSlot.end(), slot);
	if (slotInd != m_boundSlot.end())
	{
		m_boundSlot.erase(slotInd);
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	}
}

void simpleTexture::unbind()
{
	while (m_boundSlot.size())
	{
		// unbind texture from slot
		glActiveTexture(GL_TEXTURE0 + m_boundSlot.back());
		glBindTexture(GL_TEXTURE_2D, 0);
		m_boundSlot.pop_back();
	}
}


//void simpleTexture::load()
//{
//
//    int l_width, l_height, l_bpp;
//
//
//
//
//
//    stbi_set_flip_vertically_on_load(true);
//    unsigned char * textureData = stbi_load(m_texturePath.c_str(), &l_width, &l_height, &l_bpp, 0);
//
//    if (!textureData)
//    {
//        logger("SimpleTexture").debug() << "ERROR: stbi_load failed";
//        return;
//    }
//
//
//    m_width = l_width;
//    m_height = l_height;
//    m_format = l_bpp;
//
//    if (!textureData)
//        logger("SimpleTexture").debug() << "stbi_load failed";
//
//    setPixelData(textureData);
//
//    stbi_image_free(textureData);
//
//}

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


