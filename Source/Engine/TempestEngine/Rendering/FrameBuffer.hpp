/*!***************************************************************************************
\file       FrameBuffer.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#ifndef H_FRAMEBUFFER
#define H_FRAMEBUFFER
#include "Buffer.hpp"
#include "Color.hpp"
//#include "TypeData.hpp"
#include "SimpleTexture.hpp"
#include "DepthTexture.hpp"
#include "ShaderProgram.hpp"
#include "BufferAttachment.hpp"

#include "GLTypes.hpp"

class framebuffer : public iBuffer
{
public:

	framebuffer(unsigned p_width, unsigned p_height) 
		: m_width(p_width)
		, m_height(p_height)
		, m_depthTexture{ p_width, p_height }
	{}

    framebuffer(const framebuffer & p_rhs);

    void addColorAttachment(std::string const& samplerUniformName);
    void bindColorAttachments(std::shared_ptr<shaderProgram> p_program);
	void bindDepthAttachment(std::shared_ptr<shaderProgram> p_prog);

    ~framebuffer() { shutdown(); }

    size_t getBufferSize() const override;
    void build() override;
    void bind() override;
    void unbind() const override;
    void clear();
    void shutdown() override;

    bool isDirty() { return m_dirty; }
    void setDirty(bool p_dirty) { m_dirty = p_dirty; }

    bool isFirstFrame() { return m_firstFrame; }
    void setIsFirstFrame(bool p_firstFrame) { m_firstFrame = p_firstFrame; }
   

private:
	std::vector<GLenum> drawBuffers;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    color m_clearColor = color::cyan;
    GLuint m_fboHandle = 0;
    GLuint m_depthTextureHandle = 0;

    std::vector<attachment> m_colorTextureAttachments;
	depthTexture m_depthTexture;

    bool m_dirty = false;

    bool m_firstFrame = false;
    
};

#endif //H_FRAMEBUFFER
