/*!***************************************************************************************
\file       FrameBufferManager.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "FrameBufferManager.hpp"

#include "ModularFrameBuffer.hpp"

#include <GL/glew.h>

std::shared_ptr<framebuffer> framebufferManager::createFramebuffer(framebufferType p_type, unsigned int p_width, unsigned int p_height)
{

    m_framebuffers.insert(std::make_pair(p_type, std::shared_ptr<framebuffer>(new framebuffer(p_width, p_height))));
    return getFramebuffer(p_type);
}

std::shared_ptr<framebuffer> framebufferManager::getFramebuffer(framebufferType p_type)
{
    return  m_framebuffers.find(p_type)->second;
}

std::shared_ptr<modularFramebuffer> framebufferManager::createModFramebuffer(framebufferType type, unsigned width, unsigned height)
{
	auto l_fb = std::make_shared<modularFramebuffer>(width, height);
	m_modFramebuffers.emplace(type, l_fb);
	return l_fb;
}

std::shared_ptr<modularFramebuffer> framebufferManager::getModFramebuffer(framebufferType type)
{
	return m_modFramebuffers.find(type)->second;
}

int framebufferManager::createBufferPool(const unsigned p_bufferSize)
{

    renderPool l_renderPool;

    for (unsigned i = 0; i < p_bufferSize; ++i)
    {
        l_renderPool.pool.emplace_back(new framebuffer(1024, 768));

        l_renderPool.pool.back()->addColorAttachment("screenSampler");
        l_renderPool.pool.back()->build();
    }

    l_renderPool.m_used = false;

    m_renderPools.push_back(l_renderPool);

    return static_cast<int>(m_renderPools.size() - 1);

}



bool framebufferManager::addFrameToBufferPool(unsigned p_index)
{
    if(m_renderPools[p_index].pool.size() < MAXFRAMES)
    {
        m_renderPools[p_index].pool.emplace_front(new framebuffer(1024, 768));
        m_renderPools[p_index].pool.front()->addColorAttachment("screenSampler");
        m_renderPools[p_index].pool.front()->build();

        return true;
    }

    return false;
}

int framebufferManager::attachRenderPool()
{
    for (size_t i = 0; i < m_renderPools.size(); ++i)
    {
        if(!m_renderPools[i].m_used)
        {
            m_renderPools[i].m_used = true;
            return i;
        }
    }

    int index = createBufferPool(MAXFRAMES);
    m_renderPools[index].m_used = true;

    return index;

}

unsigned framebufferManager::createCapturePool(unsigned p_index)
{
    m_capturePools.push_back(m_renderPools[p_index].pool);
    resetBufferPool(p_index);
    return static_cast<unsigned>(m_capturePools.size() - 1);

}

void framebufferManager::saveRenderToCapturePool(unsigned p_captureIndex, unsigned p_tapeIndex)
{
    m_capturePools[p_captureIndex].clear();
    m_capturePools[p_captureIndex] = m_renderPools[p_tapeIndex].pool;
    resetBufferPool(p_tapeIndex);
}

void framebufferManager::clearBuffer(unsigned p_index)
{
    m_renderPools[p_index].pool.clear();
}

void framebufferManager::resetBufferPool(unsigned p_index)
{

   clearBuffer(p_index);

   for (unsigned i = 0; i < MAXFRAMES; ++i)
   {
       m_renderPools[p_index].pool.emplace_back(new framebuffer(1024, 768));

       m_renderPools[p_index].pool.back()->addColorAttachment("screenSampler");
       m_renderPools[p_index].pool.back()->build();
   }

   m_renderPools[p_index].m_used = false;

}


std::deque<std::shared_ptr<framebuffer>> & framebufferManager::getBufferPool(const unsigned p_id)
{
    return m_renderPools[p_id].pool;
}

std::deque<std::shared_ptr<framebuffer>>& framebufferManager::getCapturePool(const unsigned p_id)
{
    return m_capturePools[p_id];
}

void framebufferManager::renderToScreen()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  // bind screen framebuffer
}
