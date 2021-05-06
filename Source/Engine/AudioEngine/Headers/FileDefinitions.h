/*!***************************************************************************************
\file       FileDefinitions.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include <string>

struct RIFFChunk /* size - byte offset - totalOffset */
{
	/* TOTAL SIZE: 12 bytes*/
    char ID[4];     /* 4 - 0 - 0 */
	int size;       /* 4 - 4 - 4 */
	char type[4]; /* 4 - 8 - 8 */
};

struct fmt_Chunk /* size - byte offset - totalOffset */
{
	/* TOTAL SIZE: 24 bytes*/
	char ID[4];          /* 4 - 0  - 12 */
	int size;			 /* 4 - 4  - 16 */
	short format;		 /* 2 - 8  - 20 */
	short channels;		 /* 2 - 10 - 22 */
	int sampleRate;		 /* 4 - 12 - 24 */
	int byteRate;		 /* 4 - 16 - 28 */
	short blockAlign;    /* 2 - 20 - 32 */
	short bitsPerSample; /* 2 - 22 - 34 */
};

struct dataChunk /* size - byte offset */
{
	/* TOTAL SIZE: 8 bytes*/
	char ID[4]; /* 4 - 0 - 36 */
	int size;   /* 4 - 4 - 40 */
};

struct NodeFormatChunk /* size - byte offset */
{
	char ID[4];      /* 4 - 0  */
	short chunkSize; /* 2 - 4  */ 
	short nodeType;  /* 2 - 6  */
	int sampleRate;  /* 4 - 8  */
	short channels;  /* 2 - 12 */ 
	short bitDepth;  /* 2 - 14 */
	int numAssets;   /* 2 - 16 */
	int garbage;     /* 4 - 20 */

	// operator<< overloads?
};

// TODO: look at putting this information into the xml instead of the RIFF
struct MixDataChunk    /* size - byte offset */
{
	char ID[4];        /* 4 -  0 */
	int chunkSize;     /* 4 -  4 */
	int volumeDB;      /* 4 -  8 */
	float fadeInTime;  /* 4 - 12 */
	float fadeOutTime; /* 4 - 16 */
	int pitchChange;   /* 4 - 20 */
	short panning;     /* 2 - 24 */
	short looping;     /* 2 - 26 */
	int outputBus;     /* 4 - 28 */ // currently an int for padding
};

const std::string NodeFileIDs[] = { "RIFF", "NODE", "fmt ", "NMIX", "NAME", "AMIX", "ADAT" };
	
enum nodefileID
{
	fileTitle, 
	fileType,
	format,
	nodeMix,
	audiofileName,
	audiofileMix,
	audiofileData,
};

