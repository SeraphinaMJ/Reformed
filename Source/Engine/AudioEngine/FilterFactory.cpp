/*!***************************************************************************************
\file       FilterFactory.cpp
\author     Morgen Hyde
\date       1/28/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains all of the filter definitions used throughout the audio engine as well
			as the base class all filters should inherit from
*****************************************************************************************/

#include "FilterFactory.h"

namespace Audio
{

	FilterFactory* FilterFactory::instance = nullptr;

	bool FilterFactory::registerFilter(const std::string& typeName_p, FilterBase*(*function_p)(void))
	{
		objectCreationMap[typeName_p] = function_p;
		return true;
	}

	FilterBase* FilterFactory::createFilter(const std::string& typeName_p) const
	{
		FilterMap::const_iterator iter = objectCreationMap.find(typeName_p);
		if (iter == objectCreationMap.end())
			return nullptr;

		return iter->second();
	}

	std::vector<std::string> FilterFactory::getFilterNameList() const
	{
		FilterMap::const_iterator start = objectCreationMap.begin();
		FilterMap::const_iterator end = objectCreationMap.end();

		std::vector<std::string> filterList;

		for (; start != end; ++start)
			filterList.push_back(start->first);

		return filterList;
	}

} // namespace Audio