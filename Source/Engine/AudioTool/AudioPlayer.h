/*!***************************************************************************************
\file       AudioPlayer.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "UI_System.h"
#include "UIEventTypes.h"

class AudioPlayer : public UI_ComponentsBase
{
public:
	AudioPlayer();
	~AudioPlayer() override;

	void onUpdate() override;

	void changeActiveCallback(UI::ActiveSwapEvent* eventPtr);

private:

	Audio::AssetBase* activeObject;
	Audio::AssetBase* activeParent;
};