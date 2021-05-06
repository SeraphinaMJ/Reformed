/*!***************************************************************************************
\file       Voice.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "Voice.h"
#include "AE_AudioMath.hpp"
#include <cmath>
#include <cstring>

#define GAIN_EPSILON 0.001f

namespace Audio
{

const double PI = 4.0 * atan(1.0);

// Voice Class Implementation

	// IMPORTANT: RANGE OF (16 bit) SHORT IS -32768      TO 32767
	//			  RANGE OF (32 bit) INT   IS -2147483648 TO 2147483647

Voice::Voice(const std::string& sourceName, GameObjectID ID, const float* p_buffer,
			 size_t samples_p, int channels, const MixSettings& settings_p, FilterBase* filter_p)
		: ringData(int(samples_p)), numChannels(channels), numSamples(samples_p),
		  sourceName(sourceName), pitchChange(settings_p.pitchChangeCents),
		  panningAmount(settings_p.panValue),
		  envelope(settings_p.fadeInTime, settings_p.maxGain, settings_p.fadeOutTime, 48000, settings_p.signalTime, settings_p.looping),
		  filter(filter_p), ownerID(ID),
		  voiceState(enm_playing)


{
	ringData.write(static_cast<int>(numSamples), p_buffer);
}

Voice::~Voice()
{
}

// returns if the voice is still playing so the audio engine doesn't waste time adding 0's
bool Voice::updateVoice()
{
	bool playing = true;

	int currentState = voiceState.load(std::memory_order_acquire);

	switch (currentState)
	{

	case enm_playing:

		break;

	case enm_paused:

		break;

	case enm_stopped:

		playing = false;
		break;
	}

	return playing;
}

void Voice::getProcessedBuffer(float* p_buffer, const int& reqFrames, bool* voiceFinished_p)
{
	int currentState = voiceState.load(std::memory_order_acquire);

	if (currentState == enm_paused)
	{
		std::memset(p_buffer, 0, sizeof(float) * reqFrames * numChannels);
		return;
	}

	int pitchBufferFrames = pitchChange.getBufferFrames(static_cast<int>(reqFrames));
	float* pitchBuffer = reinterpret_cast<float*>(alloca(sizeof(float) * pitchBufferFrames * numChannels));

	// Temporary solution to a long term problem.
	// For future reference the problem is that the last sample needed by the pitch
	// parameter should be the start of the next read. current behavior on the ringData
	// object is that it assumes you used all the sample it gave you and dont need
	// to re-request them later for shifting.
	int wrapStart = ringData.read(static_cast<int>(pitchBufferFrames * numChannels), pitchBuffer);
	ringData.rewind(static_cast<int>(numChannels));

	// check for loop occurance and handle end of loop by setting samples to 0
	if (!envelope.getLooping() && wrapStart != pitchBufferFrames * numChannels)
		std::memset(p_buffer, 0, wrapStart * sizeof(float));

	if (currentState == enm_stopping)
		envelope.triggerRelease();

	pitchChange.process(pitchBuffer, p_buffer, reqFrames, (int)numChannels);

	if (filter)
		filter->process(p_buffer, reqFrames);

	if (envelope.process(p_buffer, (int)(reqFrames * numChannels)))
	{
		voiceState.store(enm_stopped);
		*voiceFinished_p = true;
	}

	double panAngle = ((PI / 2.0) / 100) * panningAmount;
	float lPan = static_cast<float>(cos(panAngle));
	if (lPan < GAIN_EPSILON) lPan = 0.0f;

	float rPan = static_cast<float>(sin(panAngle));
	if (rPan < GAIN_EPSILON) rPan = 0.0f;

	for (int i = 0; i < reqFrames; ++i)
	{
		*p_buffer++ *= lPan;
		*p_buffer *= rPan;
		p_buffer++;
	}
	p_buffer -= reqFrames * numChannels;
}

VoiceGroup::VoiceGroup(const std::string groupName_p, int numVoices_p, int channels_p,
	GameObjectID ID_p, const MixSettings& settings_p, FilterBase* filter_p)
	: groupName(groupName_p), parentID(ID_p),
	  pitchChange(settings_p.pitchChangeCents), panning(settings_p.panValue),
	  envelope(settings_p.fadeInTime, settings_p.maxGain, settings_p.fadeOutTime, 48000, settings_p.signalTime, settings_p.looping),
	  filter(filter_p), channelCount(channels_p), maxVoices(numVoices_p),
	  activeVoices(0), vArray(new Voice*[numVoices_p]), voiceGroupState(enm_playing)
{
	std::memset(vArray, 0, sizeof(Voice*) * maxVoices);
}

VoiceGroup::~VoiceGroup()
{
	for (int i = 0; i < maxVoices; ++i)
	{
		Voice* tempVoice = *vArray;
		if (tempVoice)
			delete tempVoice;

		*vArray++ = nullptr;
	}
	vArray -= maxVoices;

	delete[] vArray;
}

bool VoiceGroup::addVoice(Voice* newVoice)
{
	if (activeVoices == maxVoices)
		return false;

	vArray[activeVoices++] = newVoice;
	return true;
}

bool VoiceGroup::addVoice(const std::string& sourceName_p, const float* buffer_p,
		size_t samples_p, int channels_p, const MixSettings& settings_p)
{
	if (activeVoices == maxVoices)
		return false;

	Voice* constructedVoice = new Voice(sourceName_p, parentID, buffer_p, samples_p, channels_p, settings_p);
	vArray[activeVoices++] = constructedVoice;
	return true;
}

Voice* VoiceGroup::removeVoice(const std::string& voiceName_p)
{
	for (int i = 0; i < activeVoices; ++i)
	{
		// look for a voice with the matching name
		if (vArray[i]->getObjectName() == voiceName_p)
		{
			// swap the found voice with the last one
			Voice* targetVoice = vArray[i];
			vArray[i] = vArray[--activeVoices];

			// set the new last voice to null since we detached it
			vArray[activeVoices] = nullptr;

			// give the detached voice to the function caller
			return targetVoice;
		}
	}

	return nullptr; // if not found return null
}

bool VoiceGroup::updateVoice()
{
	for (size_t i = activeVoices; i < maxVoices; ++i)
	{
		if (vArray[i] != nullptr)
		{
			delete vArray[i];
			vArray[i] = nullptr;
		}
		else
			break;
	}

	return true;
}

void VoiceGroup::getProcessedBuffer(float* outputBuffer_p, const int& reqFrames_p, bool* voiceFinished_p)
{
	int groupState = voiceGroupState.load(std::memory_order_acquire);
    (void)groupState;
	/*
	if (groupState == enm_paused || groupState == enm_stopped)
	{
		std::memset(outputBuffer_p, 0, reqFrames_p * channelCount * sizeof(float));
		*voiceFinished_p = true;
		return;
	}
	*/

	// Determine the buffer size needed for the current pitch shift
	// TODO: This doesn't change every frame, so we don't need to call this function every frame. Find an
	//       efficient method similar to a dirty flag to save processing time.
	int pitchShiftFrames = pitchChange.getBufferFrames(reqFrames_p);
	int pitchShiftSamples = pitchShiftFrames * channelCount;

	// Generate two buffers to use
	// TODO: Shift this allocation so that it doesn't require 2 buffers to be allocated every frame.
	float* voiceBuffer      = reinterpret_cast<float*>(alloca(sizeof(float) * pitchShiftSamples));

	// pitchShiftBuffer is the sum of all the voices and sent to the voiceGroups pitch processing function
	float* pitchShiftBuffer = reinterpret_cast<float*>(alloca(sizeof(float) * pitchShiftSamples));
	std::memset(pitchShiftBuffer, 0, sizeof(float) * pitchShiftSamples);

	// Temp pointer to traverse the voice list without causing problems when another thread accesses the same array
	Voice** arrayMovement   = vArray;

	for (int i = 0; i < activeVoices; ++i)
	{
		Voice* currentUpdate = *arrayMovement++; // efficient array traversal
		bool voiceFinished = false;
		currentUpdate->getProcessedBuffer(voiceBuffer, reqFrames_p, &voiceFinished);

		for (int sample = 0; sample < pitchShiftSamples; ++sample)
			*pitchShiftBuffer++ += *voiceBuffer++;

		pitchShiftBuffer -= pitchShiftSamples;
		voiceBuffer -= pitchShiftSamples;

		// Perform a swap to move the voice outside of the active voice indices
		if (voiceFinished)
		{
			--arrayMovement;
			int lastElem = (activeVoices -= 1);
			*arrayMovement = *(vArray + lastElem); // Make sure to use the front of the array with this math
			*(vArray + lastElem) = currentUpdate;
			--i; // Backup the loop by one since we swapped with an element that hasn't been updated
		}
	}

	// Perform the processing on the newly created submix
	pitchChange.process(pitchShiftBuffer, outputBuffer_p, reqFrames_p, channelCount);

	if (voiceGroupState.load() == enm_stopping) // If the group is stopping start the fade out.
		envelope.triggerRelease();

	if (filter)
		filter->process(outputBuffer_p, reqFrames_p);

	// Panning calculations using a constant power pan.
	double panAngle = ((PI / 2.0) / 100) * panning;
	float lPan = static_cast<float>(cos(panAngle));
	if (lPan < GAIN_EPSILON) lPan = 0.0f;

	float rPan = static_cast<float>(sin(panAngle));
	if (rPan < GAIN_EPSILON) rPan = 0.0f;

	for (int i = 0; i < reqFrames_p; ++i)
	{
		*outputBuffer_p++ *= lPan;
		*outputBuffer_p++ *= rPan;
	}
	outputBuffer_p -= reqFrames_p * channelCount;

	// Apply the envelope to this buffer, checking if the envelope is finished and setting state appropriately
	if (envelope.process(outputBuffer_p, (int)(reqFrames_p * channelCount)))
	{
		voiceGroupState.store(enm_stopped);
		*voiceFinished_p = true;
	}
}

void VoiceGroup::start()
{
	voiceGroupState.store(enm_playing);
	for (int i = 0; i < activeVoices; ++i)
		vArray[i]->start();
}

void VoiceGroup::pause()
{
	voiceGroupState.store(enm_paused);
	for (int i = 0; i < activeVoices; ++i)
		vArray[i]->pause();
}

void VoiceGroup::stop()
{
	voiceGroupState.store(enm_stopping);
	for (int i = 0; i < activeVoices; ++i)
		vArray[i]->stop();
}

bool VoiceGroup::startVoice(const std::string& voiceName)
{
	for (int i = 0; i < activeVoices; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName)
		{
			vArray[i]->start();
			return true;
		}
	}

	return false;
}

bool VoiceGroup::pauseVoice(const std::string& voiceName)
{
	for (int i = 0; i < activeVoices; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName)
		{
			vArray[i]->pause();
			return true;
		}
	}

	return false;
}

bool VoiceGroup::stopVoice(const std::string& voiceName)
{
	for (int i = 0; i < activeVoices; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName)
		{
			vArray[i]->stop();
			return true;
		}
	}

	return false;
}

MixBus::MixBus(int maxVoices_p, int channels_p, int bufferSize_p, const std::string& busName_p)
	: vArray(new VoiceBase*[maxVoices_p]), numActiveVoices(0), maxVoices(maxVoices_p),
	  channelCount(channels_p), busBuffer(new float[bufferSize_p]), busName(busName_p),
	  pitchChange(1.0f), panning(50), outputGain(1.0f), filter(nullptr)
{
	std::memset(vArray, 0, sizeof(Voice*) * maxVoices_p);
	std::memset(busBuffer, 0, sizeof(float) * bufferSize_p);
}

MixBus::~MixBus()
{
	for (int i = 0; i < numActiveVoices; ++i)
		delete vArray[i];

	delete[] vArray;
	delete[] busBuffer;
}

bool MixBus::addVoice(VoiceBase* newVoice_p)
{
	if (numActiveVoices == maxVoices || newVoice_p == nullptr) return false;

	vArray[numActiveVoices++] = newVoice_p;
	return true;
}

bool MixBus::addVoice(const std::string& sourceName_p, const float* buffer_p, size_t numSamples_p,
					  int channels_p, GameObjectID parentID_p, const MixSettings& settings_p, FilterBase* /*filter_p*/)
{
	int l_activeVoices = numActiveVoices.load(std::memory_order_acquire);
	if (l_activeVoices == maxVoices || buffer_p == nullptr) return false;

	Voice* createdVoice = new Voice(sourceName_p, parentID_p, buffer_p, numSamples_p, channels_p, settings_p);

	vArray[l_activeVoices++] = createdVoice;
	numActiveVoices.store(l_activeVoices);

	return true;
}

VoiceBase* MixBus::detatchVoice(const std::string& voiceName_p)
{
	VoiceBase* foundVoice = nullptr;
	int l_activeVoices = numActiveVoices.load(std::memory_order_acquire);

	for (int i = 0; i < l_activeVoices; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName_p)
		{
			foundVoice = vArray[i];
			vArray[i] = vArray[--l_activeVoices];
			vArray[l_activeVoices] = nullptr;

			numActiveVoices.store(l_activeVoices);

			return foundVoice;
		}
	}

	return foundVoice;
}

// Called by the game thread
void MixBus::update()
{
	int activeVoices_l = numActiveVoices.load(std::memory_order_acquire);
	for (int i = 0; i < maxVoices; ++i)
	{
		if (vArray[i])
			vArray[i]->updateVoice();

		if (i >= activeVoices_l && vArray[i])
		{
			// The standard with these buffers is that any voices that aren't NULL
			// beyond the number we'd expect are valid and queued to be deleted.
			delete vArray[i];
			vArray[i] = nullptr;
		}
		else
		{
			break;
		}
	}
}

// Called by the audio thread
// TODO: Fix the pitch shifting portion of this code, currently it introduces some really annoying buzzing
void MixBus::getProcessedBuffer(float* outputBuffer_p, int requestedFrames_p, bool* emptyBusFlag_p)
{
	int samples = requestedFrames_p * channelCount;

	int l_activeVoices = numActiveVoices.load(std::memory_order_acquire);

	if (l_activeVoices == 0)
	{
		*emptyBusFlag_p = true;
		return;
	}

	//int pitchShiftFrames = pitchChange.getBufferFrames(requestedFrames_p);
	//int pitchShiftSamples = pitchShiftFrames * channelCount;

	//float* voiceBuffer = reinterpret_cast<float*>(alloca(sizeof(float) * pitchShiftFrames * channelCount));

	//float* pitchBuffer = reinterpret_cast<float*>(alloca(sizeof(float) * pitchShiftFrames * channelCount));
	//std::memset(pitchBuffer, 0, sizeof(float) * pitchShiftFrames * channelCount);

	VoiceBase** voiceTraversal = vArray;

	for (int i = 0; i < l_activeVoices; ++i)
	{
		VoiceBase* currentVoice = *voiceTraversal++;
		bool voiceFinished = false;
		//currentVoice->getProcessedBuffer(voiceBuffer, pitchShiftFrames, &voiceFinished);
		currentVoice->getProcessedBuffer(busBuffer, requestedFrames_p, &voiceFinished);

		for (int j = 0; j < samples; ++j)
			*outputBuffer_p++ += *busBuffer++;

		outputBuffer_p -= samples;
		busBuffer -= samples;

		if (voiceFinished)
		{
			int lastVoice = --l_activeVoices;
			*(--voiceTraversal) = *(vArray + lastVoice);
			*(vArray + lastVoice) = *voiceTraversal;
			numActiveVoices.store(l_activeVoices);
			--i;
		}
	}

	//pitchChange.process(pitchBuffer, outputBuffer_p, requestedFrames_p, channelCount);

	if (filter)
		filter->process(outputBuffer_p, requestedFrames_p);

	// Panning calculations using a constant power pan.
	double panAngle = ((PI / 2.0) / 100) * panning;
	float lPan = static_cast<float>(cos(panAngle));
	if (lPan < GAIN_EPSILON) lPan = 0.0f;

	float rPan = static_cast<float>(sin(panAngle));
	if (rPan < GAIN_EPSILON) rPan = 0.0f;

	lPan *= outputGain;
	rPan *= outputGain;

	for (int i = 0; i < requestedFrames_p; ++i)
	{
		*outputBuffer_p++ *= lPan;
		*outputBuffer_p++ *= rPan;
	}
	outputBuffer_p -= requestedFrames_p * channelCount;
}

void startAll()
{

}

void pauseAll()
{

}

void stopAll()
{

}

//This function starts a specific voice inside the bus
bool MixBus::startVoice(const std::string& voiceName_p, GameObjectID objectID_p)
{
	int l_voiceCount = numActiveVoices.load(std::memory_order_acquire);
	for (int i = 0; i < l_voiceCount; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName_p && vArray[i]->getGameObjectID() == objectID_p)
		{
			vArray[i]->start();
			return true;
		}
	}

	return false;
}

//This function pauses a specific voice inside the bus
bool MixBus::pauseVoice(const std::string& voiceName_p, GameObjectID objectID_p)
{
	int l_voiceCount = numActiveVoices.load(std::memory_order_acquire);
	for (int i = 0; i < l_voiceCount; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName_p && vArray[i]->getGameObjectID() == objectID_p)
		{
			vArray[i]->pause();
			return true;
		}
	}

	return false;
}

//This function stops a specific voice inside the bus
bool MixBus::stopVoice(const std::string& voiceName_p, GameObjectID objectID_p)
{
	int l_voiceCount = numActiveVoices.load(std::memory_order_acquire);
	for (int i = 0; i < l_voiceCount; ++i)
	{
		if (vArray[i]->getObjectName() == voiceName_p && vArray[i]->getGameObjectID() == objectID_p)
		{
			vArray[i]->stop();
			return true;
		}
	}

	return false;
}

void MixBus::setPitchMixing(const pitchParameter& newPitch_p) { pitchChange = newPitch_p; }

pitchParameter& MixBus::getPitchMixing() { return pitchChange; }

const pitchParameter& MixBus::getPitchMixing() const { return pitchChange; }

void MixBus::setSpeedFactor(float speedupFactor_p) { pitchChange.setCurrent(speedupFactor_p); }

float MixBus::getSpeedFactor() const { return pitchChange.getCurrent(); }

void MixBus::setGain(float maxVolumedB_p) { outputGain = dBToGain(maxVolumedB_p); }

float MixBus::getGain() const { return gainTodB(outputGain); }

void MixBus::setPanning(int panValue_p) { panning = panValue_p; }

int MixBus::getPanning() const { return panning; }


const std::string& MixBus::getBusName() { return busName; }

int MixBus::getObjectCount() { return numActiveVoices; }

} // namespace Audio
