/*!***************************************************************************************
\file       FilterFactory.h
\author     Morgen Hyde
\date       1/28/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Contains all of the filter definitions used throughout the audio engine as well
			as the base class all filters should inherit from
*****************************************************************************************/
#pragma once

#include <map>
#include <string>
#include <vector>

namespace Audio
{
	class FilterBase;

	template <typename T>
	FilterBase* createFilterType() { return new T; }

	typedef std::map<std::string, FilterBase*(*)(void)> FilterMap;

	// Singleton class
	class FilterFactory
	{
	public:
		static FilterFactory& getInstance()
		{
			if (!instance) instance = new FilterFactory;
			return *instance;
		}
		//static void cleanMemory();

		bool registerFilter(const std::string& typeName_p, FilterBase*(*function_p)(void));
		FilterBase* createFilter(const std::string& typeName_p) const;

		std::vector<std::string> getFilterNameList() const;

		// delete constructors we don't want anyone to access
		FilterFactory(FilterFactory&) = delete;
		FilterFactory& operator=(const FilterFactory&) = delete;

	private:
		FilterFactory() = default;
		~FilterFactory() = default;

		FilterMap objectCreationMap;
		static FilterFactory* instance;

	};

} // namespace Audio
