/*!***************************************************************************************
\file       FrameBuffer.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "FrameBuffer.hpp"


#include "SimpleTexture.hpp"
#include "GLDebug.hpp"

#include <numeric>

#include <GL/glew.h>




framebuffer::framebuffer(const framebuffer& p_rhs) :
	m_width(p_rhs.m_width), m_height(p_rhs.m_height),
	m_colorTextureAttachments(p_rhs.m_colorTextureAttachments),
	m_depthTexture(p_rhs.m_depthTexture)
{

    //build();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, p_rhs.m_fboHandle);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboHandle);

    unsigned numAttachments = static_cast<unsigned>(m_colorTextureAttachments.size());

    for (unsigned  i = 0; i < numAttachments; ++i)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
        glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            printf("Error while blitting\n");
        }

    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}

void framebuffer::addColorAttachment(std::string const& samplerUniformName)
{

    m_colorTextureAttachments.emplace_back(std::make_shared<simpleTexture>(m_width, m_height, format::enm_rgba), samplerUniformName );
}

void framebuffer::bindColorAttachments(std::shared_ptr<shaderProgram> p_program)
{

    int totalAttachment = static_cast<unsigned>(m_colorTextureAttachments.size());

    for (int i = 0; i < totalAttachment; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_colorTextureAttachments[i].m_texture->getTextureHandle());
        p_program->setUniforms(m_colorTextureAttachments[i].m_samplerUniformName, i);
    }
}

void framebuffer::bindDepthAttachment(std::shared_ptr<shaderProgram> p_prog)
{
	const int depthSlot = static_cast<int>(m_colorTextureAttachments.size());
	m_depthTexture.bind(depthSlot);
	p_prog->setUniforms("t_depth", depthSlot);
}

size_t framebuffer::getBufferSize() const
{
    return 0;
}

void framebuffer::build()
{
    // build framebuffer
    glGenFramebuffers(1, &m_fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

    // for color buffer use a texture so you can read and write

	drawBuffers.resize(m_colorTextureAttachments.size());
	std::iota(drawBuffers.begin(), drawBuffers.end(), 0);


    //TODO(cody): Look into RGBA32F for more percision if needed

	for (auto && tex : m_colorTextureAttachments)
	{
		tex.m_texture->buildNoData();
	}

	m_depthTexture.rebuild();

    // attach render buffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.getGLHandle());

	for (GLenum i = 0; i < drawBuffers.size(); ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorTextureAttachments[i].m_texture->getTextureHandle(), 0);
	}

    glDrawBuffers(drawBuffers.size(), drawBuffers.data());

   GLDebug::getLastFrameBufferError();

    glClearColor(0.f, 1.f, 1.f, 1.f); // default clear color
    glClearDepth(1.f);
    unbind();
}

void framebuffer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

    //glViewport(0, 0, m_width, m_height);
    //glClearColor(0.f, 0.f, 0.f, 1.1f); // default clear color
   // if (m_fboHandle == NULL) { logger::debug() << "Warning: Binding unbuilt framebuffer." };
}

void framebuffer::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind screen framebuffer
	glDrawBuffer(GL_BACK_LEFT);
}

void framebuffer::shutdown()
{
    unbind();

    glDeleteFramebuffers(1, &m_fboHandle);
    m_fboHandle = 0;
    m_depthTextureHandle = 0;
}

