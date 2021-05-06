/*!**********************************************************************************************************
\file       AnimatorComponent.hpp
\author     Henry Brobeck
\date       1/18/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#include "../Utility/AssetManager.hpp"

#include <Component.hpp>

class animation
{
public:
	animation();
	~animation() = default;
	animation(const std::string& p_tag, size_t p_frameBegin, size_t p_frameEnd, float p_fps, bool p_loop);
	animation(const animation& rhs);

	animation& operator=(const animation& rhs);

	//user defined data
	const std::string& getTag() const;
	void setTag(const std::string& p_tag);
	size_t getFrameBegin() const;
	void setFrameBegin(size_t p_frameBegin);
	size_t getFrameEnd() const;
	void setFrameEnd(size_t p_frameEnd);
	float getFPS() const;
	void setFPS(float p_fps);
	bool getLooping() const;
	void setLooping(bool p_loop);

	//control functions
	void play();
	void pause();
	void reset();
	void update(float p_dt);

	//state access
	bool isRunning();
	float getCurrentTime();
	float getMaxTime();
	size_t getCurrentFrame();

	typeRT toTypeRT() const;
	void updateFromTypeRT(typeRT& p_type);


private:
	//User defined
	std::string m_tag;
	size_t m_frameBegin;
	size_t m_frameEnd;
	float m_fps;
	bool m_loop;
	//State
	bool m_isRunning;
	float m_currentTime;
	float m_maxTime;
	
};


class animatorComponent : public componentCRTP<animatorComponent>
{
public:
	animatorComponent();
	animatorComponent(typeRT& p_type);
  componentType const type() const override { return getType(); }
  static componentType const getType() { static componentType const type = componentType::forConcrete<animatorComponent>(); return type; }
  static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "animatorComponent", true, true, true); }

	typeRT toTypeRT() const override;
	void updateFromTypeRT(typeRT& p_type) override;

	const std::map<std::string, animation>& getAnimations();
	void addAnimation(const animation& p_anim);
	void setCurrentAnimation(const std::string& p_tag);
	void setAnimationAsset(std::wstring p_assetName);
	animation* getCurrentAnimation();
	

protected:

private:
	assetHandle m_animationAsset;
	animation* m_currentAnimation;
	std::map<std::string, animation> m_animations;
	logger log;

	//====  Data  =========================================================================================//
};


// namespace <namespace name>

