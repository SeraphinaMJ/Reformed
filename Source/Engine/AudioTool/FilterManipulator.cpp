/*!***************************************************************************************
\file       FilterManipulator.cpp
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/

#include "FilterManipulator.h"
#include "Filter.h"
#include "ImGui/imgui.h"

FilterManipulator::FilterManipulator() 
	: dirtyFlag(true) 
{

}

FilterManipulator::~FilterManipulator() = default;

void FilterManipulator::editLowPass(Audio::LowPass* lpFilter_p)
{
	if (!lpFilter_p)
		return;

	// We assume this function has been called inside an ImGui child already so we
	// don't want to try and create a new one

	ImGui::TextWrapped("Low Pass Filter Editor:");
	ImGui::Separator();
	
	static float currentCutoff = 0.0f;
	if (dirtyFlag)
	{
		currentCutoff = lpFilter_p->get();
		dirtyFlag = false;
	}

	ImGui::TextWrapped("Cutoff Frequency (Hz):");
	ImGui::DragFloat("##lpCutoff", &currentCutoff, 5.0f, 20.0f, 20000.0f, "%.1f", 2.0f);
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		lpFilter_p->set(currentCutoff);
		dirtyFlag = true;
	}

	// Other weird filter stuff we might want to do?
}

void FilterManipulator::editHighPass(Audio::HighPass* hpFilter_p)
{
	(void)hpFilter_p;
}

void FilterManipulator::editBandPass(Audio::BandPass* bpFilter_p)
{
	(void)bpFilter_p;
}