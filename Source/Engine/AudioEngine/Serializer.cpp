/*!***************************************************************************************
\file       Serializer.cpp
\author     Morgen Hyde
\date       1/29/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains all of the filter definitions used throughout the audio engine as well
			as the base class all filters should inherit from
*****************************************************************************************/

#include "Serializer.h"
#include "AudioStorage.hpp"
#include "AE_AudioMath.hpp"

#include <cassert>
#include <stdexcept> // std::runtime_error

namespace Audio
{

	// The size of each chunk tag in bytes
#define TAG_SIZE 4

// The size of the header for each chunk (tag + size as int)
#define CHUNK_HEADER_SIZE 8

#define WRITE_TAG(file, tag) file.write(tag, TAG_SIZE)

	const char RIFF_tag[] = { 'R', 'I', 'F', 'F' };
	const char WAVE_tag[] = { 'W', 'A', 'V', 'E' };
	const char FMT_tag[] = { 'f', 'm', 't', ' ' };
	const char DATA_tag[] = { 'd', 'a', 't', 'a' };

	const char NODE_tag[] = { 'N', 'O', 'D', 'E' };
	const char ADTA_tag[] = { 'A', 'D', 'T', 'A' };

	const char MIX_tag[] = { 'M', 'I', 'X', ' ' };
	const char FILT_tag[] = { 'F', 'I', 'L', 'T' };
	const char DSP_tag[] = { 'D', 'S', 'P', ' ' };

	const int sampleRate = 48000;
	const int bitDepth = 16;
	const int channels = 2;

	const int mixSize = sizeof(Audio::MixSettings);
	const int InfoSize = 64;

	Serializer::Serializer(const std::string& outputPath_p)
		: outputPath(outputPath_p), objectInformation(new char[InfoSize])
	{
	}

	Serializer::~Serializer()
	{
		delete[] objectInformation;
	}

	void Serializer::setRelativeFilepath(const std::string& outputPath_p)
	{
		outputPath = outputPath_p;
	}

	void Serializer::writeNode(const Audio::AudioNode* node_p)
	{
		// Start by trying to create the file
		std::string filename = node_p->getName();
		if (filename.find(".node") == std::string::npos)
			filename.append(".node");

		std::ofstream outputFile(outputPath + filename, std::ios_base::binary);
		if (!outputFile.is_open()) // return that we failed to create the specified file
			throw std::runtime_error(std::string("Failed to create file at: " + outputPath + filename));

		// Just write the first 4 as an ID to the file type
		outputFile.write(RIFF_tag, TAG_SIZE);
		unsigned int fileSizeBytes = 0;
		// Write a placeholder int for the file size since we don't yet know the size of the total file
		outputFile.write(reinterpret_cast<char*>(&fileSizeBytes), sizeof(unsigned int));

		outputFile.write(NODE_tag, TAG_SIZE);

		// Format information about the data
		outputFile.write(FMT_tag, TAG_SIZE);

		int chunkSize = sizeof(int) * 3;
		outputFile.write(reinterpret_cast<char*>(&chunkSize), sizeof(int));

		outputFile.write(reinterpret_cast<const char*>(&sampleRate), sizeof(int));
		outputFile.write(reinterpret_cast<const char*>(&channels), sizeof(int));
		outputFile.write(reinterpret_cast<const char*>(&bitDepth), sizeof(int));

		// End format information

		// Add the size of the data we just wrote
		fileSizeBytes += TAG_SIZE + CHUNK_HEADER_SIZE + chunkSize;

		// Write overall mixSettings to the file
		const Audio::MixSettings& settings = node_p->getMixSettings();

		WRITE_TAG(outputFile, MIX_tag);
		outputFile.write(reinterpret_cast<const char*>(&mixSize), sizeof(int));

		outputFile.write(reinterpret_cast<const char*>(&settings), mixSize);

		// End mixSettings

		// Add the size of the data we just wrote
		fileSizeBytes += CHUNK_HEADER_SIZE + mixSize;

		// Write the filter to the file

		Audio::FilterBase* filter_l = node_p->getFilter();
		if (filter_l)
		{
			std::string filterName = filter_l->getFilterName();
			int nameSize = static_cast<int>(filterName.size());

			// Get the information from the filter as a c-string,
			// returns the amount of the buffer used
			int writeSize = filter_l->getSerializeInfo(objectInformation, InfoSize);
			int chunkSize = writeSize + nameSize + sizeof(int);

			// Write the filter tag and chunk size
			outputFile.write(FILT_tag, TAG_SIZE);
			outputFile.write(reinterpret_cast<char*>(&chunkSize), sizeof(int));

			// Write name size and name
			outputFile.write(reinterpret_cast<char*>(&nameSize), sizeof(int));
			outputFile.write(filterName.c_str(), nameSize);

			// Write the c-string block of data from the filter
			outputFile.write(objectInformation, writeSize);
		}

		/*
		// TODO: Write each DSP to the file
		const std::vector<DSPBase*>& dspList = node_p->getDSPList();
		std::vector<DSPBase*>::const_iterator dspStart, dspEnd;
		while (dspStart != dspEnd)
		{
			(*dspStart)->dispatchSerialize(this);
			++dspStart;
		}
		*/

		// Write each audio data object to the file
		const std::vector<AudioData*>& nodeData = node_p->getAllData();
		std::vector<AudioData*>::const_iterator dataStart, dataEnd;
		for (dataStart = nodeData.begin(), dataEnd = nodeData.end(); dataStart != dataEnd; ++dataStart)
		{
			//addAudioData(*dataStart);
			AudioData* writeData = (*dataStart);

			WRITE_TAG(outputFile, ADTA_tag);

			const std::string& name = writeData->getName();
			int nameLength = static_cast<int>(name.size());

			unsigned int dataSamples = writeData->getNumSamples();

			int chunkSize = sizeof(int) + nameLength + (dataSamples * (bitDepth / 8));
			outputFile.write(reinterpret_cast<char*>(&chunkSize), sizeof(int));

			// write the length of this objects name then the name itself
			outputFile.write(reinterpret_cast<char*>(&nameLength), sizeof(int));
			outputFile.write(name.c_str(), nameLength);

			// convert the data buffer to integer values and write it to the file
			const float* dataBuffer = writeData->getAudioDataBuffer();
			const short* writableData = reinterpretAudioData<short>(dataBuffer, dataSamples);
			outputFile.write(reinterpret_cast<const char*>(writableData), dataSamples * (bitDepth / 8));

			fileSizeBytes += chunkSize + CHUNK_HEADER_SIZE;

			// write the mixSettings override information //
			WRITE_TAG(outputFile, MIX_tag);
			const MixSettings* dataMix = node_p->getMixSettingsOverride(name);
			outputFile.write(reinterpret_cast<const char*>(&mixSize), sizeof(int));
			outputFile.write(reinterpret_cast<const char*>(dataMix), mixSize);

			fileSizeBytes += chunkSize + CHUNK_HEADER_SIZE;
			// end mixSettings information

			// TODO: Add filter override
			const Audio::FilterBase* filterOverride_l = node_p->getFilterBaseOverride(name);
			if (filterOverride_l)
			{
				std::string filterName = filterOverride_l->getFilterName();
				int nameSize = static_cast<int>(filterName.size());

				// Get the information from the filter as a c-string,
				// returns the amount of the buffer used
				int writeSize = filterOverride_l->getSerializeInfo(objectInformation, InfoSize);
				int chunkSize = writeSize + nameSize + sizeof(int);

				// Write the filter tag and chunk size
				outputFile.write(FILT_tag, TAG_SIZE);
				outputFile.write(reinterpret_cast<char*>(&chunkSize), sizeof(int));

				// Write name size and name
				outputFile.write(reinterpret_cast<char*>(&nameSize), sizeof(int));
				outputFile.write(filterName.c_str(), nameSize);

				// Write the c-string block of data from the filter
				outputFile.write(objectInformation, writeSize);
			}

			// TODO: Add DSP List override
		}

		outputFile.seekp(TAG_SIZE); // reset to front of output file + tag offset
		outputFile.write(reinterpret_cast<char*>(&fileSizeBytes), sizeof(fileSizeBytes));

		outputFile.close();
	}

	void Serializer::writeWav(const Audio::AudioData* /*data_p*/)
	{

	}

	void Serializer::writeWav(const std::string& /*fileName_p*/, int /*bitDepth_p*/, int /*channels_p*/, int /*sampleRate_p*/, float* /*buffer_p*/, int /*frames_p*/)
	{

	}

	// Writing processing functions
	void Serializer::writeLowPass(const Audio::LowPass* /*lpFilter_p*/)
	{

	}

	void Serializer::writeCompressor(const Audio::Compressor* /*compressor_p*/)
	{

	}

	// Private Functions

	void Serializer::writeFormat()
	{
	}

	void Serializer::writeMixInfo(const Audio::MixSettings& /*mixInfo*/)
	{

	}

	void Serializer::addAudioData(const Audio::AudioData* /*data_p*/)
	{

	}

} // namespace Audio
