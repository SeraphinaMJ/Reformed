/*!**********************************************************************************************************
\file       AnimationSystem.hpp
\author     Henry Brobeck
\date       11/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once


#include "../SystemBase.hpp"


class animationSystem : public systemBase
{
public:
	animationSystem() = default;
	~animationSystem() override = default;

	

	static const std::string& getName() { static const std::string n("animationSystem"); return n; }
	const std::string& name() const override { return getName(); }

protected:
	void onStartFrame() override;

private:
	static std::string m_name;
};


// namespace <namespace name>

