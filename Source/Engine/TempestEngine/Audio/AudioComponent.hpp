/*!***************************************************************************************
\file       AudioComponent.hpp
\author     Morgen Hyde
\date       9/11/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Definition of Audio Component
*****************************************************************************************/
#pragma once

#include "AE_Events.hpp"

//========3rd Party Includes============================================================//
#include <string>
#include <map>
#include <utility>
#include <Component.hpp>

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////
namespace Audio
{

	////////==========================================================================////////
	//////// Classes                                                                  ////////
	////////==========================================================================////////
	class AudioComponent : public componentCRTP<AudioComponent>
	{

		///////============================================================================///////
		///////   Public                                                                   ///////
		///////============================================================================///////
	public:

		/*!***************************************************************************************
		\brief  Default audio component constructor
		*****************************************************************************************/
		AudioComponent();
		AudioComponent(typeRT data)
		{
			updateFromTypeRT(data);
		}

		/*!***************************************************************************************
		\brief  Default audio component deconstructor
		*****************************************************************************************/
		~AudioComponent() = default;

		/*!***************************************************************************************
		\brief  Sends an event to the audio engine to create a voice for this component
		\param AudioFile - name of the file to load
		\param ID - ID to reference this voice with later
		\preLoad - determines if the sound should be streamed or not
		\return void
		*****************************************************************************************/
		void Load(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Unloads audio data and removes the attached voice
		\param  ID - The ID to unbind
		\return void
		*****************************************************************************************/
		void Unload(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Starts the specified voice
		\param  ID - ID of which attached voice to start playback of
		\return void
		*****************************************************************************************/
		void Start(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Stops the specified voice
		\param  ID - ID of which attached voice to stop playback of
		\return void
		*****************************************************************************************/
		void Stop(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Pauses the specified voice
		\param  ID - ID of which attached voice to pause playback of
		\return void
		*****************************************************************************************/
		void Pause(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Determines if a voice is currently paused
		\param  ID - ID of which attached voice to find the pause status of
		\return bool - true if the sound is paused, false otherwise
		*****************************************************************************************/
		bool IsPaused(const std::string& AudioFile);

		/*!***************************************************************************************
		\brief  Gets component type
		\return The component type
		*****************************************************************************************/
		componentType const type() const override { return getType(); }

		/*!***************************************************************************************
		\brief  Gets component type
		\return The component type
		*****************************************************************************************/
		static componentType const getType() { static componentType const type = componentType::forConcrete<AudioComponent>(); return type; }

		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Audio", true, true, false); }

		typeRT toTypeRT() const override;
		void updateFromTypeRT(typeRT& p_type) override;

		///////============================================================================///////
		///////   Private                                                                  ///////
		///////============================================================================///////
	private:

		unsigned int ObjectID;
		bool paused;
		static unsigned int NumAudioComponents;
	};

} // namespace Audio