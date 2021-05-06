/*!***************************************************************************************
\file       AudioMetrics.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/

#include "AudioMetrics.h"
#include "IOTarget.h"
#include "AE_Events.hpp"

#include "AE_AudioMath.hpp"

#include "ImGui/imgui.h"

#include <string>
#include <cmath> // std::sqrt

#define IO_FRAMES 256
#define IO_CHANNELS 2
#define IOBUFFER_SIZE (IO_FRAMES * IO_CHANNELS)

const float meterMax_dB         =   0.0f;
const float meterMin_dB         = -96.0f;
const float meterMarkIncrements =   6.0f; // dB difference between adjacent meter markings

const float safeBarMax_posDB   = 66.0f;
const float dangerBarMax_posDB = 84.0f;
const float clipBarMax_posDB   = 96.0f;

const int numMarks = static_cast<int>((meterMax_dB - meterMin_dB) / meterMarkIncrements);

const ImColor meterBackground(55, 55, 55);
const ImColor meterMarkColor(255, 255, 255);
const ImColor meterSafeColor(0, 120, 0);
const ImColor meterDangerColor(120, 120, 0);
const ImColor meterClippingColor(120, 0, 0);

AudioMetrics::AudioMetrics()
{
	metricsInput = new IOTargetBuffer(IOBUFFER_SIZE);
	// When we send this event, we transfer "ownership" of the IOTarget to the audio engine
	sendGlobalEvent(new Audio::IOTargetSwapEvent(metricsInput));

	metricsBuffer = new float[IOBUFFER_SIZE];
}

AudioMetrics::~AudioMetrics()
{
	delete[] metricsBuffer;
}

static inline ImVec2 operator+(const ImVec2& left, const ImVec2& right) { return ImVec2(left.x + right.x, left.y + right.y); }
static inline ImVec2 operator-(const ImVec2& left, const ImVec2& right) { return ImVec2(left.x - right.x, left.y - right.y); }

void AudioMetrics::onUpdate()
{
	ImGui::Begin("Audio Metrics");

	static bool showAudioLevels = true;
	
	ImGui::Checkbox("Show Level Meter", &showAudioLevels);

	ImGui::Separator();

	MeterValues RMS_dB, peak_dB;

	if (showAudioLevels)
	{
		// Render the frame of the two meters

		// NOTE: The bottom of the meter is going to be set at -96 dB
		//       The top of the meter (clipping) is going to be set at 0 dB
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 windowSize = ImGui::GetWindowSize();
		ImVec2 windowPos  = ImGui::GetWindowPos();
		float drawYPos = ImGui::GetCursorScreenPos().y;

		ImVec2 meterPadding(10.0f, 10.0f);
		float center = (windowSize.x / 2.0f);

		drawList->AddText(ImVec2(windowPos.x + meterPadding.x, drawYPos), meterMarkColor, "RMS Meter (dB) ");
		drawList->AddText(ImVec2(windowPos.x + center + meterPadding.x, drawYPos), meterMarkColor, "Peak Meter (dB)");

		float textHeight = ImGui::GetTextLineHeight();
		drawYPos += textHeight;

		ImVec2 meterSize((center - (3.0f * meterPadding.x)) / 2.0f, windowSize.y - (2.0f * meterPadding.y) - drawYPos);
		ImVec2 RMeterOffset(meterSize.x + meterPadding.x, 0.0f);

		ImVec2 RMS_LMeterPos(windowPos.x + meterPadding.x, drawYPos + meterPadding.y);
		ImVec2 RMS_RMeterPos(RMS_LMeterPos + RMeterOffset);

		ImVec2 Peak_LMeterPos(windowPos.x + center + meterPadding.x, drawYPos + meterPadding.y);
		ImVec2 Peak_RMeterPos(Peak_LMeterPos + RMeterOffset);

		calculateMeterLevels(RMS_dB, peak_dB);

		renderMeter(RMS_dB.LChannel_dB, RMS_LMeterPos, meterSize);
		renderMeter(RMS_dB.RChannel_dB, RMS_RMeterPos, meterSize);

		renderMeter(peak_dB.LChannel_dB, Peak_LMeterPos, meterSize);
		renderMeter(peak_dB.RChannel_dB, Peak_RMeterPos, meterSize);
	}

	ImGui::End();
}

void AudioMetrics::calculateMeterLevels(MeterValues& RMS_dB, MeterValues& peak_dB)
{
	// First, calculate the RMS of the buffer and the peak level of this buffer read
	metricsInput->readData(metricsBuffer, IOBUFFER_SIZE);

	float maxGainLBuffer = 0.0f;
	float maxGainRBuffer = 0.0f;

	float RMSCalculation_L = 0.0f;
	float RMSCalculation_R = 0.0f;

	for (int i = 0; i < IO_FRAMES; ++i)
	{
		// Get the unsigned value for checking peak levels
		float unsignedCurrent_L = std::abs(*metricsBuffer++);
		float unsignedCurrent_R = std::abs(*metricsBuffer++);

		// Find if this value would set a new maximum
		if (maxGainLBuffer < unsignedCurrent_L)
			maxGainLBuffer = unsignedCurrent_L;

		if (maxGainRBuffer < unsignedCurrent_R)
			maxGainRBuffer = unsignedCurrent_R;

		// Add in the square of the value to the RMS calculation
		RMSCalculation_L += unsignedCurrent_L * unsignedCurrent_L;
		RMSCalculation_R += unsignedCurrent_R * unsignedCurrent_R;
	}

	metricsBuffer -= IOBUFFER_SIZE;

	RMSCalculation_L /= IOBUFFER_SIZE;
	RMSCalculation_R /= IOBUFFER_SIZE;

	RMSCalculation_L = static_cast<float>(std::sqrt(RMSCalculation_L));
	RMSCalculation_R = static_cast<float>(std::sqrt(RMSCalculation_R));

	RMS_dB.LChannel_dB = gainTodB(RMSCalculation_L);
	RMS_dB.RChannel_dB = gainTodB(RMSCalculation_R);

	peak_dB.LChannel_dB = gainTodB(maxGainLBuffer);
	peak_dB.RChannel_dB = gainTodB(maxGainRBuffer);
}

void AudioMetrics::renderMeterLevelBar(const float& RMS_dB, const ImVec2& topCorner, const ImVec2& meterSize, bool trackPeak)
{
	(void)trackPeak;

	// Convert our dB values to a more intuitive range (0 as min, 96 as max)
	float positiveDBRange = RMS_dB + 96.0f;

	// Calculate the ImGui coordinates for where the top of each bar should be clipped to
	float safeBarMaxHeight   = meterSize.y * (safeBarMax_posDB   / 96.0f);
	float dangerBarMaxHeight = meterSize.y * (dangerBarMax_posDB / 96.0f);
	//float clipBarMaxHeight   = meterSize.y;

	// Determine the height of the bottom bar in ImGui window units
	float safeBarHeight = meterSize.y * (positiveDBRange / 96.0f);
	
	// Determine the height of the middle bar, clipping the bottom bar if needed
	float dangerBarHeight = 0.0f;
	if (positiveDBRange > safeBarMax_posDB)
	{
		// Subtract the bottom bar max and calculate the top corner of the middle bar
		// as if it was the bottom one to get the actual height of the bar
		dangerBarHeight = meterSize.y * ((positiveDBRange - safeBarMax_posDB) / 96.0f);
		safeBarHeight = safeBarMaxHeight;
	}

	// Determine the height of the top bar, clipping the middle bar if needed
	float clipBarHeight = 0.0f;
	if (positiveDBRange > dangerBarMax_posDB)
	{
		// Repeat the same operation for the top bar
		clipBarHeight = meterSize.y * ((positiveDBRange - dangerBarMax_posDB) / 96.0f);
		dangerBarHeight = dangerBarMaxHeight - safeBarMaxHeight;
	}

	// If the input dB is too large for the scale on the meter, clip the top bar to the top
	// of the meter to prevent it rendering incorrectly
	if (positiveDBRange > clipBarMax_posDB)
		clipBarHeight = meterSize.y - dangerBarMaxHeight;

	// Calculate the top and bottom of the bottom bar
	ImVec2 safeBarBottom = topCorner + meterSize;
	ImVec2 safeBarTop(topCorner.x, safeBarBottom.y - safeBarHeight);

	// Get the window draw list to acces the primitive API
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw the bottom bar
	drawList->AddRectFilled(safeBarTop, safeBarBottom, meterSafeColor);

	// Check if the other bars need to be rendered
	if (dangerBarHeight)
	{
		// Calculate the middle bar dimensions and render it
		ImVec2 dangerBarBottom(safeBarBottom.x, safeBarTop.y);
		ImVec2 dangerBarTop(safeBarTop.x, safeBarTop.y - dangerBarHeight);

		drawList->AddRectFilled(dangerBarTop, dangerBarBottom, meterDangerColor);

		// Check if the top bar needs to be rendered
		if (clipBarHeight)
		{
			// Calculate the top bar dimensions and render it
			ImVec2 clipBarBottom(dangerBarBottom.x, dangerBarTop.y);
			ImVec2 clipBarTop(dangerBarTop.x, dangerBarTop.y - clipBarHeight);

			drawList->AddRectFilled(clipBarTop, clipBarBottom, meterClippingColor);
		}
	}
}

void AudioMetrics::renderMeter(const float& dBLevel, const ImVec2& topCorner, const ImVec2& meterSize, bool trackPeak)
{
	(void)trackPeak;

	// Grab the window's draw list to access primitive drawing functions
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Draw the background of the meter
	drawList->AddRectFilled(topCorner, topCorner + meterSize, meterBackground);

	// Draw the bar showing the current audio level
	renderMeterLevelBar(dBLevel, topCorner, meterSize);

	// Calculate variables needed for drawing regularly spaced dB marks on the meter
	float markDistance = meterSize.y / numMarks;
	float currentMarkHeight = topCorner.y;

	ImVec2 LineLeft(topCorner.x, currentMarkHeight);
	ImVec2 LineRight(topCorner.x + meterSize.x, currentMarkHeight);

	int dBLevel_l = 0;
	// Draw each line in even increments specified by the above variables
	for (int lineCount = 0; lineCount < numMarks; ++lineCount, dBLevel_l -= (int)meterMarkIncrements)
	{
		drawList->AddLine(LineLeft, LineRight, meterMarkColor);

		// Add the text below the line to indicate the level
		drawList->AddText(LineLeft, meterMarkColor, std::to_string(dBLevel_l).c_str());

		LineLeft.y += markDistance;
		LineRight.y += markDistance;
	}
}
