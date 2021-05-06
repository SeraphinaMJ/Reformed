/*!***************************************************************************************
\file       DoubleBuffer.cpp
\author     Morgen Hyde
\date       1/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/
#include "DoubleBuffer.h"

#include <future>
#include <cstring> // std::memcpy

#define SWAP_ACTIVE(buffFlag) (buffFlag ^= 1)
#define BUFFER_A 0
#define BUFFER_B 1

DoubleBufferSource::DoubleBufferSource(IOTargetBase* readSource_p, int bufferSize_p)
	: readSource(readSource_p), bufferSet(new float*[2]), bufferSize(bufferSize_p), nextBufferRead(BUFFER_A)
{
	bufferSet[BUFFER_A] = new float[bufferSize_p];
	bufferSet[BUFFER_B] = new float[bufferSize_p];
}

DoubleBufferSource::~DoubleBufferSource()
{
	// Also watch out for threading here
	delete[] bufferSet[BUFFER_A];
	delete[] bufferSet[BUFFER_B];

	delete[] bufferSet;

	delete readSource;
}

void DoubleBufferSource::read(float* outputBuffer_p)
{
	if (!outputBuffer_p)
		return;

	std::memcpy(outputBuffer_p, bufferSet[nextBufferRead], sizeof(float) * bufferSize);

	// This requires that the IOTargetBase readData function is thread safe and won't cause 
	// any weird shenanigans with the the float buffer we want it to fill
	std::async(std::launch::async, &IOTargetBase::readData, readSource, bufferSet[nextBufferRead], bufferSize);

	// there needs to be some code protecting critical sections here
	// options are:
	// std::atomic<bool> set by the IOTargetBase
	// querying if the std::future object has finished with std::future::wait_for

	SWAP_ACTIVE(nextBufferRead);
}


DoubleBufferOutput::DoubleBufferOutput(IOTargetBase* writeTarget_p, int bufferSize_p)
	: writeTarget(writeTarget_p), bufferSet(new float*[2]), bufferSize(bufferSize_p), nextBufferWrite(BUFFER_A)
{
	bufferSet[BUFFER_A] = new float[bufferSize_p];
	bufferSet[BUFFER_B] = new float[bufferSize_p];
}

DoubleBufferOutput::~DoubleBufferOutput()
{
	// Again threading could be an issue, double check there's no dangling threads
	// that might be in the middle of using this data
	delete[] bufferSet[BUFFER_A];
	delete[] bufferSet[BUFFER_B];

	delete[] bufferSet;

	delete writeTarget;
}

void DoubleBufferOutput::write(float* inputBuffer_p)
{
	if (!inputBuffer_p)
		return;

	std::memcpy(bufferSet[nextBufferWrite], inputBuffer_p, sizeof(float) * bufferSize);

	// Again this will likely have some sort of threading issue with the buffers getting overwritten or having some
	// weird issue. Look into the options listed in the DoubleBufferInput class to 
	std::async(std::launch::async, &IOTargetBase::writeData, writeTarget, bufferSet[nextBufferWrite], bufferSize);

	SWAP_ACTIVE(nextBufferWrite);
}
