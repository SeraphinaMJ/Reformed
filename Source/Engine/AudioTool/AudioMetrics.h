/*!***************************************************************************************
\file       AudioMetrics.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "UI_System.h"

class IOTargetBase;
struct ImVec2;

class AudioMetrics : public UI_ComponentsBase
{
public:
	AudioMetrics();
	~AudioMetrics();

	void onUpdate() override;

private:

	struct MeterValues
	{
		float LChannel_dB;
		float RChannel_dB;
	};

	void calculateMeterLevels(MeterValues& RMS_dB, MeterValues& peak_dB);
	void renderMeterLevelBar(const float& RMS_dB, const ImVec2& topCorner, const ImVec2& meterSize, bool trackPeak = false);
	void renderMeter(const float& dBLevel, const ImVec2& topCorner, const ImVec2& meterSize, bool trackPeak = false);

	IOTargetBase* metricsInput;
	float* metricsBuffer;
};