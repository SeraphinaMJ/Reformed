/*!***************************************************************************************
\file       AE_RingBuffer.hpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition and Implementation of Circle Buffer used by audio functionality
*****************************************************************************************/
#pragma once

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
//////// Classes                                                                  ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: RingBuffer
\brief      Basic circle buffer to hold a finite amount of data. Makes use of a read and
	        write pointer to track insertions and reads.
*****************************************************************************************/
class RingBuffer
{

///////============================================================================///////
///////   Public                                                                   ///////
///////============================================================================///////
public:

	/*!***************************************************************************************
	\brief  Default Constructor for RingBuffer, requires an initial size, 
	        optionally an array of data to own
	*****************************************************************************************/
	RingBuffer(int numSamples);
	RingBuffer(int numSamples, float* data);

	/*!***************************************************************************************
	\brief  Default deconstructor
	*****************************************************************************************/
	~RingBuffer();

	// Read a sample without moving the pointer
	__inline const float& operator[](const int& sample) const;

	// Write blocks of sample data, writes a number of samples in front of the current index
	// if the data wraps the function will return true
	// TODO: Re evaluate if there should be a write front and write back function
	//       depending if the need arises to store data and move the write pointer forward.
	bool write(int numSamples, const float* data); // Will not move pointer

	// writes a sample to the current index and moves the index forward
	__inline bool write(const float& data); // Will move pointer forward

	// Copy data to the given pointer of specified size, will move the index accordingly.
	// if the data wraps the function returns the index of the first wrapped element instead
	// of the number of samples copied.
	int read(int numSamples, float* data);

	// This function makes me sad, it backs up the index by the given number of samples
	__inline void rewind(const int& numSamples);


///////============================================================================///////
///////   Private                                                                  ///////
///////============================================================================///////
private:
	float* dataArray;
	int index;
	const int dataSize;
};

// Treat the ring buffer like a delay line
__inline const float& RingBuffer::operator[](const int& sampleNum) const
{
	int access = index + sampleNum;
	if (access < 0) access += dataSize;
	while (access > dataSize)
		access -= dataSize;

	return dataArray[access];
}

// Treat the ring buffer like a delay line
__inline bool RingBuffer::write(const float& data)
{
	// Insert the element into the last index and move forward by one.
	dataArray[index++] = data;
	if (index == dataSize)
	{
		index = 0;
		return true;
	}

	return false;
}

__inline void RingBuffer::rewind(const int& numSamples)
{
	index -= numSamples; // back up the number of samples

	// perform wrapping to get an acceptable index
	if (index < 0) 
		index += dataSize; 
}