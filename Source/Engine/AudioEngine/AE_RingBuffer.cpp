/*!***************************************************************************************
\file       AE_RingBuffer.cpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition and Implementation of Circle Buffer used by audio functionality
*****************************************************************************************/

#include "AE_RingBuffer.hpp"

#include <algorithm>
#include <cstring>

RingBuffer::RingBuffer(int numSamples)
	: dataArray(new float[numSamples]), index(0), dataSize(numSamples)
{
}

RingBuffer::RingBuffer(int numSamples, float* data)
	: dataArray(new float[numSamples]), index(0), dataSize(numSamples)
{
	std::memcpy(dataArray, data, dataSize * sizeof(float));
}

RingBuffer::~RingBuffer()
{
	delete[] dataArray;
}

bool RingBuffer::write(int numSamples, const float* data)
{
	int writeSize = (dataSize < numSamples) ? dataSize : numSamples;
	int writeLoc = index;
	// int writeTemp = 0; // TODO: < ------- UNUSED
	int dataLoc  = 0;
	bool wrap = false;

	// determine how many samples to write, either whole array or samples from current index
    // up to the end of the internal buffer.
	int writeBlock = (writeSize < (dataSize - index)) ? writeSize : (dataSize - index);

	// copy from input buffer to end of input or end of internal buffer
	std::memcpy(dataArray + writeLoc, data + dataLoc, writeBlock * sizeof(float));

	// Deduct the number of samples written from
	writeSize -= writeBlock;

	if(writeSize) // check if we still have samples to write at the beginning of internal array
	{
		wrap = true;
		dataLoc += writeBlock;
		writeLoc = 0; // bring writeLoc back into correct range

		// copy the remaining data from the input buffer
		std::memcpy(dataArray + writeLoc, data + dataLoc, writeSize * sizeof(float));
	}

	return wrap;
}

// Copy a number of samples into the given array. It's assumed the pointer given
// will hold the number of requested samples. Returns the number of samples copied unless
// a wrap occurred in which case it returns the first element of the wrapped data
int RingBuffer::read(int numSamples, float* data)
{
	int readSize  = numSamples;
	int readLoc   = 0;
	int readBlock = (readSize < (dataSize - index)) ? readSize : (dataSize - index);
	int returnVal = numSamples;

	std::memcpy(data + readLoc, dataArray + index, readBlock * sizeof(float));
	readSize -= readBlock;
	readLoc += readBlock;

	if(readSize != 0)
		returnVal = readBlock;

	index += readBlock;
	if (index >= dataSize)
		index -= dataSize;

	while(readSize)
	{
		readBlock = (readSize < (dataSize - index)) ? readSize : (dataSize - index);
		std::memcpy(data + readLoc, dataArray + index, readBlock * sizeof(float));
		readSize -= readBlock;
		index += readBlock;
		if (index >= dataSize)
			index -= dataSize;
	}

	return returnVal;
}
