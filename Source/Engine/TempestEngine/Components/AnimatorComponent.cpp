/*!**********************************************************************************************************
\file       AnimatorComponent.cpp
\author     Henry Brobeck
\date       1/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief 
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "AnimatorComponent.hpp"
#include "../Utility/AssetManager.hpp"
//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Static Deceleration  ==========================================================================//
//========  Static Class Methods  =========================================================================//
animation::animation() : m_tag(""), m_frameBegin(0), m_frameEnd(0), m_fps(0), m_loop(false), m_isRunning(false), m_currentTime(0), m_maxTime(0)
{
}

animation::animation(const std::string& p_tag, size_t p_frameBegin, size_t p_frameEnd, float p_fps, bool p_loop) : m_tag(p_tag), m_frameBegin(p_frameBegin),
	m_frameEnd(p_frameEnd), m_fps(p_fps), m_loop(p_loop), m_isRunning(false), m_currentTime(0.0f), m_maxTime(0.0)
{
	m_maxTime = m_fps * float(p_frameEnd - p_frameBegin);
	
}

animation::animation(const animation& rhs) : 
m_tag(rhs.m_tag), m_frameBegin(rhs.m_frameBegin), m_frameEnd(rhs.m_frameEnd), m_fps(rhs.m_fps), m_loop(rhs.m_loop), m_isRunning(rhs.m_isRunning), m_currentTime(rhs.m_currentTime), m_maxTime(rhs.m_maxTime)
{
	
}

animation& animation::operator=(const animation& rhs)
{
	if(&rhs == this)
	{
		return *this;
	}
	m_tag = rhs.m_tag;
	m_frameBegin = rhs.m_frameBegin;
	m_frameEnd = rhs.m_frameEnd;
	m_fps = rhs.m_fps;
	m_loop = rhs.m_loop;
	m_isRunning = rhs.m_isRunning;
	m_currentTime = rhs.m_currentTime;
	m_maxTime = rhs.m_maxTime;
	return *this;
}

const std::string& animation::getTag() const
{
	return m_tag;
}

void animation::setTag(const std::string& p_tag)
{
	m_tag = p_tag;
}

size_t animation::getFrameBegin() const
{
	return m_frameBegin;
}

void animation::setFrameBegin(size_t p_frameBegin)
{
	m_frameBegin = p_frameBegin;
}

size_t animation::getFrameEnd() const
{
	return m_frameEnd;
}

void animation::setFrameEnd(size_t p_frameEnd)
{
	m_frameEnd = p_frameEnd;
}

float animation::getFPS() const
{
	return m_fps;
}

void animation::setFPS(float p_fps)
{
	//if the animation is running, we need to scale current time to match new fps
	m_fps = p_fps;
	float currentProgress = m_currentTime / m_maxTime;
	m_maxTime = m_fps * float(m_frameEnd - m_frameBegin);
	m_currentTime = m_maxTime * currentProgress;
}

bool animation::getLooping() const
{
	return m_loop;
}

void animation::setLooping(bool p_loop)
{
	m_loop = p_loop;
}

void animation::play()
{
	m_isRunning = true;
}

void animation::pause()
{
	m_isRunning = false;
}

void animation::reset()
{
	m_currentTime = 0.0f;
}

void animation::update(float p_dt)
{
	if(!m_isRunning)
	{
		return;
	}

	m_currentTime += p_dt;

	if(m_currentTime > m_maxTime)
	{
		if(m_loop)
		{
			m_currentTime -= m_maxTime;
		}
		else
		{
			m_currentTime = 0.0f;
			m_isRunning = false;
		}
	}

}

bool animation::isRunning()
{
	return m_isRunning;
}

float animation::getCurrentTime()
{
	return m_currentTime;
}

float animation::getMaxTime()
{
	return m_maxTime;
}

size_t animation::getCurrentFrame()
{
	float currentProgress = m_currentTime - m_maxTime;
	auto offset = size_t(currentProgress * (m_frameEnd - m_frameBegin));
	return m_frameBegin + offset;
}

typeRT animation::toTypeRT() const
{
	typeRT result;
	result.setTypeName("animation");
	result.setVariableName(m_tag);
	result.insertMember(typeRT("tag",m_tag));
	result.insertMember(typeRT("frameBegin", unsigned(m_frameBegin)));
	result.insertMember(typeRT("frameEnd", unsigned(m_frameEnd)));
	result.insertMember(typeRT("fps", m_fps));
	result.insertMember(typeRT("loop", m_loop));
	return result;
}

void animation::updateFromTypeRT(typeRT& p_type)
{
	m_tag = p_type.member("tag").getString();
	m_frameBegin = p_type.member("frameBegin").getUInt();
	m_frameEnd = p_type.member("frameEnd").getUInt();
	m_fps = p_type.member("fps").getFloat();
	m_loop = p_type.member("loop").getBool();
}

animatorComponent::animatorComponent() : m_animationAsset(nullptr), m_currentAnimation(nullptr), log("animatorComponent")
{

}

animatorComponent::animatorComponent(typeRT& p_type) : m_animationAsset(nullptr), m_currentAnimation(nullptr), log("animatorComponent")
{
	updateFromTypeRT(p_type);
}

typeRT animatorComponent::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	if(m_animationAsset == nullptr)
	{
		result.insertMember(typeRT("animationAsset", std::string("")));
	}
	else
	{
		result.insertMember(typeRT("animationAsset", assetManager::stringFromWide(m_animationAsset->getName())));
	}
	
	if(m_currentAnimation)
	{
		result.insertMember(typeRT("currentAnimation", m_currentAnimation->getTag()));
	}
	else
	{
		result.insertMember(typeRT("currentAnimation", ""));
	}
	std::vector<typeRT> animations;
	for(auto& iter : m_animations)
	{
		animations.push_back(iter.second.toTypeRT());
	}
	result.insertMember(typeRT("animations", animations));

	return result;

}

void animatorComponent::updateFromTypeRT(typeRT& p_type)
{
	auto assetStr = p_type.member("animationAsset").getString();
	if(assetStr != "")
	{
		m_animationAsset = assetManager::getAsset(assetManager::wideFromString(p_type.member("animationAsset").getString()));
	}
	
	auto anims = p_type.member("animations");

	for(auto& anim : anims.array())
	{
		animation deserialized;
		deserialized.updateFromTypeRT(anim);
		m_animations.insert(std::pair(deserialized.getTag(), deserialized));
	}

	//update current after anims have been loaded
	std::string current = p_type.member("currentAnimation").getString();
	if(current != "")
	{
		m_currentAnimation = &m_animations[current];
	}

}

const std::map<std::string, animation>& animatorComponent::getAnimations()
{
	return m_animations;
}

void animatorComponent::addAnimation(const animation& p_anim)
{
	m_animations.insert(std::pair(p_anim.getTag(), p_anim));
}

void animatorComponent::setCurrentAnimation(const std::string& p_tag)
{
	auto iter = m_animations.find(p_tag);
	if(iter == m_animations.end())
	{
		log.error("No animation with name " + p_tag);
		m_currentAnimation = nullptr;
		return;
	}
	m_currentAnimation = &(iter->second);
}

void animatorComponent::setAnimationAsset(std::wstring p_assetName)
{
	m_animationAsset = assetManager::getAsset(p_assetName);
	if(m_animationAsset == nullptr)
	{
		log.error("No asset with the name " + assetManager::stringFromWide(p_assetName));
	}
}

animation* animatorComponent::getCurrentAnimation()
{
	return m_currentAnimation;
}

