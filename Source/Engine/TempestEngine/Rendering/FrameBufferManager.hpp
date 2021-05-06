/*!***************************************************************************************
\file       FrameBufferManager.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "FrameBuffer.hpp"
constexpr auto MAXFRAMES = 50;

#include "GLTypes.hpp"

#include <deque>
#include <unordered_map>

class camera;

enum class framebufferType
{
    Screen = 0,
    DeferredGBuffer,
    LightAccBuf,
	DeferredOutput,
	ShadowBuffer,
	ShadowMap,
    CaptureBuffer,
};

struct renderPool
{
    bool m_used;
    std::deque<std::shared_ptr<framebuffer>> pool;
};

class modularFramebuffer;

class framebufferManager
{
public:
	framebufferManager() = default;
    
    std::shared_ptr<framebuffer> createFramebuffer(framebufferType type, unsigned int width, unsigned int height);
    std::shared_ptr<framebuffer> getFramebuffer(framebufferType type);

	std::shared_ptr<modularFramebuffer> createModFramebuffer(framebufferType type, unsigned width, unsigned height);
	std::shared_ptr<modularFramebuffer> getModFramebuffer(framebufferType type);

    unsigned createBufferPool();
    bool addFrameToBufferPool(unsigned p_index);

    int createBufferPool(const unsigned p_bufferSize);

    int attachRenderPool();

    void clearBuffer(unsigned p_index);

    std::deque<std::shared_ptr<framebuffer>> & getBufferPool(const unsigned p_id);

    std::deque<std::shared_ptr<framebuffer>> & getCapturePool(const unsigned p_id);

	void renderToScreen(); 

    void saveRenderToCapturePool(unsigned p_captureIndex, unsigned p_tapeIndex);

    unsigned createCapturePool(unsigned p_index);

    void resetBufferPool(unsigned p_index);
    

private:

    std::unordered_map<framebufferType, std::shared_ptr<framebuffer> > m_framebuffers; //!< holds meshes by name
	std::unordered_map<framebufferType, std::shared_ptr<modularFramebuffer> > m_modFramebuffers; //!< holds customizable framebuffers

    std::vector<renderPool> m_renderPools;
    std::vector <std::deque<std::shared_ptr<framebuffer>>> m_capturePools;

};


