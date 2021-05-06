/*!***************************************************************************************
\file       FilterManipulator.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the displayObjects class which is responsible for
			displaying various AudioEngine objects in the Authoring tool through
			the double dispatch pattern
*****************************************************************************************/
#pragma once

#include "DisplayPrimer.h"

/*!***************************************************************************************
\par class: objectManipulator
\brief This class is responsible for displaying and editing various AudioEngine objects 
       that get sent to it. 
*****************************************************************************************/
class FilterManipulator final : public Audio::DisplayPrimer
{
public:
	FilterManipulator();
	~FilterManipulator();

	// Filter edit functions
	virtual void editLowPass(Audio::LowPass* lpFilter_p) override;
	virtual void editHighPass(Audio::HighPass* hpFilter_p) override;
	virtual void editBandPass(Audio::BandPass* bpFilter_p) override;

	void reset() { dirtyFlag = true; }

private:
	
	bool dirtyFlag;
};