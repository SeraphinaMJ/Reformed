#include "ModularFrameBuffer.hpp"

#include "DepthTexture.hpp"
#include "SimpleTexture.hpp"

#include <GL/glew.h>

modularFramebuffer::modularFramebuffer(unsigned p_width, unsigned p_height, color const& p_clearColor)
	: m_width{p_width}, m_height{p_height}
	, m_clearColor{ p_clearColor }
	, m_drawBuffers{}
	, m_colorAttachments{}
	, m_depthAttachment{ std::make_shared<depthTexture>(p_width, p_height), std::string{"depthTexture"} }
{
}

modularFramebuffer::~modularFramebuffer()
{
	shutdown();
}

void modularFramebuffer::bindDepthAttachment(std::shared_ptr<shaderProgram> p_program, int slot)
{
	p_program->setUniforms(m_depthAttachment.m_samplerUniformName, slot);
	m_depthAttachment.m_texture->bind(slot);

}

void modularFramebuffer::setDepthAttachment(depthAttachment const & p_depthAttachment)
{
	m_depthAttachment = p_depthAttachment;
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D,
		m_depthAttachment.m_texture->getGLHandle(),
		0
	);
}

void modularFramebuffer::addColorAttachment(attachment const& p_colorAttachment, int p_slot)
{
	m_dirty = true;
	if (auto l_found = findBySlot(p_slot); l_found != m_colorAttachments.end())
	{
		auto&&[l_attach, l_slot] = *l_found;
		l_attach = p_colorAttachment;
		l_slot = p_slot;
		bindColorAttachment(l_attach.m_texture->getTextureHandle(), l_slot);
	}
	else
	{
		m_colorAttachments.emplace_back(p_colorAttachment, p_slot);
		auto&&[l_attach, l_slot] = m_colorAttachments.back();
		bindColorAttachment(l_attach.m_texture->getTextureHandle(), l_slot);
	}
}

void modularFramebuffer::removeColorAttachment(int slot)
{
	if (auto findIter = findBySlot(slot); findIter != m_colorAttachments.end())
	{
		bindColorAttachment(0, slot);
		m_colorAttachments.erase(findIter);
		m_dirty = true;
	}
}

void modularFramebuffer::clearColorAttachments()
{
	for (auto const& [l_attach, l_slot] : m_colorAttachments)
		bindColorAttachment(0, l_slot);

	m_colorAttachments.clear();
	m_dirty = true;
}

size_t modularFramebuffer::getBufferSize() const
{
	return 0;
}

void modularFramebuffer::build()
{
	glGenFramebuffers(1, &m_fboHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
}

void modularFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		logger("GLDebug").debug() << "incomplete frame buffer " << m_fboHandle << std::endl;
	}

	if (m_dirty) reconfigureDrawBuffers();

	glDrawBuffers(m_drawBuffers.size(), m_drawBuffers.data());
}

void modularFramebuffer::bindRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboHandle);
}

void modularFramebuffer::unbindRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void modularFramebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK_LEFT);
}

void modularFramebuffer::clear(clearBit bits)
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);

	glClear(
		(GL_COLOR_BUFFER_BIT   * bool(bits & bufferClearbits::color  )) |
		(GL_DEPTH_BUFFER_BIT   * bool(bits & bufferClearbits::depth  )) |
		(GL_STENCIL_BUFFER_BIT * bool(bits & bufferClearbits::stencil)) 
	);
}

void modularFramebuffer::shutdown()
{
	glDeleteFramebuffers(1, &m_fboHandle);
	m_fboHandle = 0;
}

void modularFramebuffer::bindColorAttachments(std::shared_ptr<shaderProgram> p_program, int p_offset)
{
	for (auto&& [l_attach, l_slot] : m_colorAttachments)
	{
		p_program->setUniforms(l_attach.m_samplerUniformName, l_slot + p_offset);
		l_attach.m_texture->bind(l_slot + p_offset);
	}
}

void modularFramebuffer::setClearColor(color const & p_clearColor)
{
	m_clearColor = p_clearColor;
}

std::pair<unsigned, unsigned> modularFramebuffer::getDim() const noexcept
{
	return std::make_pair(getWidth(), getHeight());
}

unsigned modularFramebuffer::getWidth() const noexcept
{
	return m_width;
}

unsigned modularFramebuffer::getHeight() const noexcept
{
	return m_height;
}

void modularFramebuffer::reconfigureDrawBuffers()
{
	m_drawBuffers.clear();
	m_drawBuffers.reserve(m_colorAttachments.size());

	for (auto&& [l_attach, l_slot] : m_colorAttachments)
		m_drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + l_slot);

	m_dirty = false;
}

void modularFramebuffer::bindColorAttachment(const GLint handle, int slot)
{
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + slot,
		GL_TEXTURE_2D,
		handle,
		0
	);
}

auto modularFramebuffer::findBySlot(int p_slot)
	-> decltype(m_colorAttachments.begin())
{
	return std::find_if(m_colorAttachments.begin(), m_colorAttachments.end(),
		[p_slot](auto const& p_pair) -> bool
		{
			auto const& [l_attach, l_slot] = p_pair;
			return l_slot == p_slot;
		}
	);
}
