#include "DepthTexture.hpp"
#include <GL/glew.h>
#include "GLDebug.hpp"

depthTexture::depthTexture(const unsigned p_width, const unsigned p_height)
	: texture{}
	, m_width{ p_width }
	, m_height{ p_height }
{
}

depthTexture::~depthTexture()
{
	shutdown();
}

void depthTexture::shutdown()
{
	glDeleteTextures(1, &m_depthHandle);
	if (m_boundSlot != -1) unbind();
	m_depthHandle = 0;
}

void depthTexture::build()
{
	glGenTextures(1, &m_depthHandle);
	bind(0);


	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0,
		GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);



	unbind();
}

void depthTexture::rebuild()
{
	shutdown();
	build();
}

void depthTexture::bind(unsigned int p_slot)
{
	m_boundSlot = static_cast<int>(p_slot);
	glActiveTexture(GL_TEXTURE0 + p_slot);
	glBindTexture(GL_TEXTURE_2D, m_depthHandle);
}

void depthTexture::unbind()
{
	if (m_boundSlot == -1)
	{
		return;
	}
	glActiveTexture(GL_TEXTURE0 + m_boundSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_boundSlot = -1;
}

int depthTexture::getBoundSlot()
{
	return m_boundSlot;
}

// candidate for GPU-only memory?
void depthTexture::setData(assetHandle)
{
	assert(!"Cannot set a depth texture's data");
}

unsigned depthTexture::width() const
{
	return m_width;
}

unsigned depthTexture::height() const
{
	return m_height;
}

GLuint depthTexture::getGLHandle()
{
	return m_depthHandle;
}
