/*!***************************************************************************************
\file       UIEventTypes.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include "EventHandler.hpp"
#include "AE_Events.hpp"

namespace Audio {
	class AssetBase;
}

namespace UI
{

class ActiveSwapEvent : public Audio::EventTemplate<ActiveSwapEvent>
{
public:
	ActiveSwapEvent(Audio::AssetBase* newObject, Audio::AssetBase* parent_p = nullptr)
		: data(newObject), parent(parent_p) {}

	Audio::AssetBase* data;
	Audio::AssetBase* parent;
};

} // namespace UI
