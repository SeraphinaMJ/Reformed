/*!***************************************************************************************
\file       BusInterface.h
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include <string>

namespace Audio
{
	enum AudioBusType {
		bus_Default,
		bus_Music,
		bus_SoundEffects,
		bus_UI,
		bus_BusCount
	};

	class BusInterface
	{
	public:
		virtual ~BusInterface() = default;

		virtual float getBusGain(AudioBusType busID_p) = 0;
		virtual void setBusGain(int busID_p, float gain_p) = 0;

		virtual float getBusSpeedupFactor(int busID_p) = 0;
		virtual void setBusSpeedupFactor(int busID_p, float speedFactor_p) = 0;

		virtual int getBusPanning(int busID_p) = 0;
		virtual void setBusPanning(int busID_p, int panningAmount_p) = 0;
	};
}
