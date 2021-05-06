/*!***************************************************************************************
\file       IOTarget.h
\author     Morgen Hyde
\date       1/17/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of the IOTarget classes, which are responsible for sending/recieving
			data from a specified IO object
*****************************************************************************************/
#pragma once

#include <string>
#include <fstream>


/*!***************************************************************************************
\par class: IOTargetBase
\brief   
*****************************************************************************************/
class IOTargetBase
{
public:
	/*!***************************************************************************************
	\brief  
	\param  
	\return 
	*****************************************************************************************/
	IOTargetBase();

	/*!***************************************************************************************
	\brief  
	\param  
	\return 
	*****************************************************************************************/
	virtual ~IOTargetBase();

	/*!***************************************************************************************
	\brief  
	\param  
	\return 
	*****************************************************************************************/
	virtual bool readData(float* dataBuffer_p, int dataBufferSize_p)  = 0;

	/*!***************************************************************************************
	\brief 
	\param  
	\return 
	*****************************************************************************************/
	virtual bool writeData(float* dataBuffer_p, int dataBufferSize_p) = 0;

private:


};

/*!***************************************************************************************
\par class: IOTargetFile
\brief
*****************************************************************************************/

// NOTE: Maybe for these IO output targets (or just in general) we build up a small buffer of buffers
//       and send one larger (delayed) batch to be written to save time since larger writes are generally faster.
//       I also don't see why that same concept couldn't be applied to reading from a file needed by the audio thread
//       assuming it's handled well (it won't be)
class IOTargetFile : public IOTargetBase
{
public:
	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	IOTargetFile(const std::string& fileName_p);

	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	~IOTargetFile();


	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	bool readData(float* dataBuffer_p, int dataBufferSize_p) override;

	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	bool writeData(float* dataBuffer_p, int dataBufferSize_p) override;

	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	void setIOMode();

private:

	// Only one of these will be open at a time
	std::ofstream outputFile;
	std::ifstream inputFile;

	std::string fileName;
};

/*!***************************************************************************************
\par class: IOTargetBuffer
\brief
*****************************************************************************************/
class IOTargetBuffer : public IOTargetBase
{
public:
	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	IOTargetBuffer(int internalB);

	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	~IOTargetBuffer();


	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	bool readData(float* dataBuffer_p, int dataBufferSize_p) override;

	/*!***************************************************************************************
	\brief
	\param
	\return
	*****************************************************************************************/
	bool writeData(float* dataBuffer_p, int dataBufferSize_p) override;


private:

	float* buffer;
	int bufferSize;
};