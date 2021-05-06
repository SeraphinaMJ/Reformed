/*!***************************************************************************************
\file       DSPFactory.h
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/
#pragma once

#include <map>
#include <vector>

#include <iostream>

namespace Audio
{
	class DSPBase;

	template <typename T>
	DSPBase* createDSPType() { return new T; }

	typedef std::map<std::string, DSPBase*(*)(void)> DSPMap;

	// Singleton class
	class DSPFactory
	{
	public:
		static DSPFactory& getInstance()
		{
			if (!instance) instance = new DSPFactory;
			return *instance;
		}

		bool registerDSP(const std::string& typeName_p, DSPBase*(*function_p)(void));
		DSPBase* createDSP(const std::string& typeName_p);
		
		// Delete constructors and assignment operator to prevent copies being made
		DSPFactory(DSPFactory&) = delete;
		DSPFactory& operator=(const DSPFactory&) = delete;
		std::vector<std::string> getDSPNameList() const;

	private:

		DSPFactory() = default;
		~DSPFactory() = default;

		std::map<std::string, DSPBase*(*)(void)> objectCreationMap;
		static DSPFactory* instance;
	};

} // namespace Audio