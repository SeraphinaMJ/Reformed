/*!***************************************************************************************
\file       DSPFactory.cpp
\author     Morgen Hyde
\date       1/23/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief	
*****************************************************************************************/

#include "DSPFactory.h"

namespace Audio
{
	DSPFactory* DSPFactory::instance = nullptr;

	bool DSPFactory::registerDSP(const std::string& typeName_p, DSPBase*(*function_p)(void))
	{
		objectCreationMap[typeName_p] = function_p;
		return true;
	}

	DSPBase* DSPFactory::createDSP(const std::string& typeName_p)
	{
		DSPMap::iterator iter = objectCreationMap.find(typeName_p);
		if (iter == objectCreationMap.end())
			return nullptr;

		return iter->second();
	}

	std::vector<std::string> DSPFactory::getDSPNameList() const
	{
		DSPMap::const_iterator start = objectCreationMap.begin();
		DSPMap::const_iterator end = objectCreationMap.end();

		std::vector<std::string> DSPList;

		for(; start != end; ++start)
			DSPList.push_back(start->first);
		
		return DSPList;
	}

} // namespace Audio