/*!***************************************************************************************
\file       AE_Events.hpp
\author     Morgen Hyde
\date       9/12/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Event type definitions and declarations for the audio event manager
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "EventHandler.hpp"

#include <string>

//#include "TriggerTypes.hpp"
//#include "AudioAttributes.hpp"

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

class IOTargetBase;

namespace Audio
{

	typedef unsigned int GameObjectID;
	typedef short AssetID;

	typedef enum AudioObjectType
	{
		type_none = -1,
		type_audiodata,
		type_node,
		type_graph

	} AudioObjectType;

	////////==========================================================================////////
	//////// Classes                                                                  ////////
	////////==========================================================================////////

	/*!***************************************************************************************
	\par class: AudioEngine
	\brief		Event type to signal the audio engine to start playing a voice
	*****************************************************************************************/
	class PlayEvent : public EventTemplate<PlayEvent>
	{
	public:
		/*!***************************************************************************************
		\brief  Constructor for play event, stores filename on class
		\param p_file_name - file the event needs to affect
		*****************************************************************************************/
		PlayEvent(GameObjectID ID, const std::string& name, std::string parentName_p = "NoParent")
			: ObjectID(ID), parentName(parentName_p), objectName(name)
		{
			if (name.find(".wav") != std::string::npos)
				audioObjType = type_audiodata;

			if (name.find(".node") != std::string::npos)
				audioObjType = type_node;
		}

		GameObjectID ObjectID;
		std::string parentName;
		std::string objectName;

		AudioObjectType audioObjType;
	};

	class PauseEvent : public EventTemplate<PauseEvent>
	{
	public:
		PauseEvent(GameObjectID ID, std::string fileName = 0) : ObjectID(ID), fileName(fileName) {}

		GameObjectID ObjectID;
		std::string fileName;
	};

	class StopEvent : public EventTemplate<StopEvent>
	{
	public:
		StopEvent(GameObjectID ID, std::string fileName = 0) : ObjectID(ID), fileName(fileName) {}

		GameObjectID ObjectID;
		std::string fileName;
	};

	class MoveEvent : public EventTemplate<MoveEvent>
	{
	public:
		MoveEvent(std::string source, std::string dest, AudioObjectType sourceType, AudioObjectType destType)
			: sourceName(source), destName(dest), sourceType(sourceType), destType(destType) { }

		std::string sourceName, destName;
		AudioObjectType sourceType, destType;
	};

	// IO related events
	// NOTE: These may not be things we want the rest of the game to have control of.
	//       Removing them means we'll need some sort of async loader in audio engine
	//       and to optimize load and voice creation times to reduce delay between
	//       sending a play event and the node actually being played

	/*!***************************************************************************************
	\par class: BindEvent
	\brief      Event type to signal the audio engine to load an audio file into memory
	*****************************************************************************************/
	class LoadEvent : public EventTemplate<LoadEvent>
	{
	public:
		/*!***************************************************************************************
		\brief  Constructor for play event, stores filename on class
		\param p_file_name - file the event needs to affect
		*****************************************************************************************/
		LoadEvent(const std::string& name) : objName(name)
		{
			if (name.find(".wav") != std::string::npos)
				objType = type_audiodata;

			if (name.find(".node") != std::string::npos)
				objType = type_node;
		}

		std::string objName;
		AudioObjectType objType;
	};

	class UnloadEvent : public EventTemplate<UnloadEvent>
	{
	public:

		UnloadEvent(std::string fileName) : fileName(fileName) {}

		std::string fileName;
	};

	class IOTargetSwapEvent : public EventTemplate<IOTargetSwapEvent>
	{
	public:
		IOTargetSwapEvent(IOTargetBase* newIOTarget_p) : IOTarget(newIOTarget_p) {}

		IOTargetBase* IOTarget;
	};
} // namespace Audio
