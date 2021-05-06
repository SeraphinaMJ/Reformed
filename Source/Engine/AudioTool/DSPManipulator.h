/*!***************************************************************************************
\file       DSPManipulator.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#pragma once

#include "DisplayPrimer.h"

class DSPManipulator final : public Audio::DisplayPrimer
{
public:

	// Add override functions for the DSP you want to display here
	virtual void editCompressor(Audio::Compressor* compressor_p) override;


private:


};