/*!***************************************************************************************
\file       DSPBase.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		
*****************************************************************************************/
#pragma once

#include <string>
#include "DSPFactory.h"

namespace Audio
{
	class DisplayPrimer;
	class Serializer;

	class DSPBase
	{
	public:
		virtual ~DSPBase() = default;
	
		virtual void editDSP(DisplayPrimer* displayClass_p) = 0;
		virtual void dispatchSerialize(Serializer* serializer_p) = 0;

		virtual void process(float* dataBuffer_p, int frameCount_p) = 0;

		virtual const std::string& getDSPName() const = 0;
	};

	template <typename T>
	class DSPTemplate : public DSPBase
	{
	public:
		virtual ~DSPTemplate() = default;

		virtual void editDSP(DisplayPrimer* displayClass_p) = 0;
		virtual void dispatchSerialize(Serializer* serializer_p) = 0;

		virtual void process(float* dataBuffer_p, int frameCount_p) = 0;

		virtual const std::string& getDSPName() const = 0;
	
		// In order for your type to actually register you have to implement this method to use the boolean
		// otherwise it might optimize it out preventing the constructor we want getting called
		virtual bool isRegistered() = 0;

	protected:
		static bool registered;
	};

	template <typename T>
	bool DSPTemplate<T>::registered = DSPFactory::getInstance().registerDSP(T::getName(), &createDSPType<T>);


} // namespace Audio

