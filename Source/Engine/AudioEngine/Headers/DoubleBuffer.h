/*!***************************************************************************************
\file       DoubleBuffer.h
\author     Morgen Hyde
\date       1/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/
#pragma once

#include <fstream>

#include "IOTarget.h"

class DoubleBufferSource
{
public:

	DoubleBufferSource(IOTargetBase* readSource_p, int bufferSize_p);
	~DoubleBufferSource();

	void read(float* outputBuffer_p);

private:

	IOTargetBase* readSource;

	float** bufferSet;
	int bufferSize;
	
	char nextBufferRead;
};

class DoubleBufferOutput
{
public:

	DoubleBufferOutput(IOTargetBase* writeTarget_p, int bufferSize_p);
	~DoubleBufferOutput();

	void write(float* inputBuffer_p);

private:

	IOTargetBase* writeTarget;

	float** bufferSet;
	int bufferSize;

	char nextBufferWrite;

};