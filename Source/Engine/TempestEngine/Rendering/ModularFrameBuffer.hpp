/*!***************************************************************************************
\file       ModularFrameBuffer.hpp
\author     Patrick Cook
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      FBO with hotswappable attachments
*****************************************************************************************/
#include "Buffer.hpp"
#include "Color.hpp"
#include "ShaderProgram.hpp"
#include "BufferAttachment.hpp"

#include <array>
#include "GLTypes.hpp"

using clearBit = GLuint;
namespace bufferClearbits
{
	constexpr clearBit color   = 0b001;
	constexpr clearBit depth   = 0b010;
	constexpr clearBit stencil = 0b100;

	constexpr clearBit all     = 0b111;
}



class modularFramebuffer : public iBuffer
{
public:
	modularFramebuffer(unsigned p_width, unsigned p_height, color const& p_clearColor = color::black);

	~modularFramebuffer() override;

	void bindColorAttachments(std::shared_ptr<shaderProgram> p_program, int offset=0);
	void bindDepthAttachment(std::shared_ptr<shaderProgram> p_program, int slot=0);

	void setDepthAttachment(depthAttachment const& p_depthAttachment);
	void addColorAttachment(attachment const& p_colorAttachment, int slot);
	void removeColorAttachment(int slot);
	void clearColorAttachments();

	size_t getBufferSize() const override;
	void build() override;
	void bind() override;
	void unbind() const override;

	virtual void bindRead();
	virtual void unbindRead();
	void clear(clearBit bits = bufferClearbits::all);

	void shutdown() override;
	void reconfigureDrawBuffers();

	void setClearColor(color const& p_clearColor);

	std::pair<unsigned, unsigned> getDim() const noexcept;
	unsigned getWidth() const noexcept;
	unsigned getHeight() const noexcept;

private:
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	color m_clearColor = color::cyan;
	GLuint m_fboHandle = 0;
	GLuint m_depthTextureHandle = 0;

	mutable std::vector<GLenum> m_drawBuffers;
	std::vector<std::pair<attachment, int>> m_colorAttachments;
	depthAttachment m_depthAttachment;

	mutable bool m_dirty{ false };

	void bindColorAttachment(GLint handle, int slot);

	auto findBySlot(int slot) -> decltype(m_colorAttachments.begin());
};
