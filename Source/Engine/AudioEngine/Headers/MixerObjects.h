/*!***************************************************************************************
\file       MixerObjects.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#define SAMPLE_RATE 48000.0f

namespace Audio 
{
	// Attaches to Audio containers (AudioData, Nodes, Graphs)
	// Used to instantiate voice sub classes (envelope, pitch, panning)
	struct MixSettings
	{
		MixSettings() : maxGain(1.0f), panValue(50) { }

		// Envelope Information
		float fadeInTime, fadeOutTime;
		float maxGain;
		float signalTime;

		// Pitch Change Information
		float pitchChangeCents;

		// Panning Information
		short panValue;
	
		// Others
		char bus;
		char looping;
	};

} // namespace Audio