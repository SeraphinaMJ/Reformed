/*!***************************************************************************************
\file       CubeTexture.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds all information about a texture
*****************************************************************************************/
//========Self Include==================================================================//
#include <Logger.hpp>
#include "CubeTexture.hpp"
#include "GLDebug.hpp"

#include <algorithm>
#include <GL/glew.h>

static unsigned char const UnbuiltTexture = 0;
static char const UnboundTexture = -1;


cubeTexture::cubeTexture(std::vector<assetHandle> p_assetHandles, std::string const& p_textureLabel) : m_cubeTextureLabel(p_textureLabel)
{

    if (p_assetHandles.size() == 0 || p_assetHandles.size() > 6)
		logger("CubeTexture").debug() << "ERROR:Cube texture filenameSize is " << p_assetHandles.size();


	m_textures.resize(p_assetHandles.size());

	std::transform(p_assetHandles.cbegin(), p_assetHandles.cend(),
		m_textures.begin(),
		[](auto const& m_asset)
		{
			auto texturePtr = std::make_shared<simpleTexture>(m_asset);
			texturePtr->setData(m_asset);
			return texturePtr;
		});
}

void cubeTexture::shutdown()
{
	m_textures.clear();
}

void cubeTexture::build()
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_cubeTextureHandle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTextureHandle);


    for (unsigned int i = 0; i < m_textures.size(); i++)
    {
            //https://learnopengl.com/Advanced-OpenGL/Cubemaps
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, m_textures[i]->getWidth(), m_textures[i]->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_textures[i]->getPixalData());
    }

    //https://www.opengl.org/discussion_boards/showthread.php/173152-Texture-looks-strange
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void cubeTexture::bind(unsigned slot)
{


    //TODO(cody):Check if already bound
    if (m_boundSlot != UnboundTexture)
        logger("CubeTexture").debug() << "ERROR: texture already bound";

    //http://stackoverflow.com/questions/8866904

    // sets current active object
    glActiveTexture(GL_TEXTURE0 + slot);

    // binds texture to that object
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTextureHandle);

    //GLDebug::getLastError();

    // save slot its in
    m_boundSlot = slot;
}

void cubeTexture::unbind()
{

    if (m_boundSlot == UnboundTexture)
    {
       // logger("CubeTexture").debug() << "ERROR: texture already unbound";
        return;
    }

    // unbind texture from slot
    glActiveTexture(GL_TEXTURE0 + m_boundSlot);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_boundSlot = UnboundTexture; // unbound = -1
}

void cubeTexture::setData(assetHandle p_assetHandle)
{

}
