/*!***************************************************************************************
\file       Serializer.h
\author     Morgen Hyde
\date       1/29/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains all of the filter definitions used throughout the audio engine as well
			as the base class all filters should inherit from
*****************************************************************************************/
#pragma once

#include <string>
#include <fstream>

#include "FileDefinitions.h"
#include "MixerObjects.h"

namespace Audio
{
	class AudioNode;
	class AudioData;

	class LowPass;

	class Compressor;

	class Serializer
	{
	public:
		Serializer(const std::string& outputPath_p);
		~Serializer();

		void setRelativeFilepath(const std::string& outputPath_p);

		void writeNode(const Audio::AudioNode* node_p);

		void writeWav(const Audio::AudioData* data_p);
		void writeWav(const std::string& fileName_p, int bitDepth_p, int channels_p, int sampleRate_p, float* buffer_p, int frames_p);


		// For writing the processing objects attached to nodes and audio data
		void writeLowPass(const Audio::LowPass* lpFilter_p);

		void writeCompressor(const Audio::Compressor* compressor_p);

	private:

		std::string outputPath;
		char* objectInformation;
		//std::ofstream outputFile;

		void writeFormat();
		void writeMixInfo(const Audio::MixSettings& mixInfo);

		// Used to handle writing the audio data objects owned by a node to the overall node file
		void addAudioData(const Audio::AudioData* data_p);

	};
}
