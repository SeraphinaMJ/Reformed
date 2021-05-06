/*!***************************************************************************************
\file       IOTarget.cpp
\author     Morgen Hyde
\date       1/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "IOTarget.h"
#include <algorithm>
#include <cstring>

IOTargetBase::IOTargetBase() = default;

IOTargetBase::~IOTargetBase() = default;


IOTargetFile::IOTargetFile(const std::string& /*fileName_p*/)
{

}

IOTargetFile::~IOTargetFile()
{

}

bool IOTargetFile::readData(float* /*dataBuffer_p*/, int /*dataBufferSize_p*/)
{

	return true;
}

bool IOTargetFile::writeData(float* /*dataBuffer_p*/, int /*dataBufferSize_p*/)
{

	return true;
}

void IOTargetFile::setIOMode()
{

}


// This needs some serious work to figure out what it's actually supposed to be doing
// I know what it should do kindof but not super distinctly

// This also needs some serious synrchonization help to make sure that an async thread
// won't completely ruin everything when another thread is trying to read data.
IOTargetBuffer::IOTargetBuffer(int bufferSize_p)
	: buffer(new float[bufferSize_p]), bufferSize(bufferSize_p)
{
}

IOTargetBuffer::~IOTargetBuffer()
{
	delete[] buffer;
}

bool IOTargetBuffer::readData(float* dataBuffer_p, int dataBufferSize_p)
{
	std::memcpy(dataBuffer_p, buffer, sizeof(float) * dataBufferSize_p);
	return true;
}

bool IOTargetBuffer::writeData(float* dataBuffer_p, int dataBufferSize_p)
{
	int writeSize = std::min(dataBufferSize_p, bufferSize);

	std::memcpy(buffer, dataBuffer_p, sizeof(float) * writeSize);

	return true;

}
