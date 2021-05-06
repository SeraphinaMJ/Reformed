/*!***************************************************************************************
\file       StyleKeeper.cpp
\author     Aaron Damyen
\date       3/3/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a manager for rendering styles within the Editor
*****************************************************************************************/
#include "StyleKeeper.hpp"

unsigned int Editor::styleKeeper::getStyle(std::string p_component_type, std::string p_variable_name, std::string p_property) const
{
	// look for component/variable/property specific first
	auto [l_specific_found, l_specific_value] = getStyleSpecific(p_component_type, p_variable_name, p_property);
	if (l_specific_found) return l_specific_value;
	// look for component/*/property specific
	auto[l_variablewide_found, l_variablewide_value] = getStyleSpecific(p_component_type, "*", p_property);
	if (l_variablewide_found) return l_variablewide_value;
	// look for */variable/property specific
	auto[l_componentwide_found, l_componentwide_value] = getStyleSpecific("*", p_variable_name, p_property);
	if (l_componentwide_found) return l_componentwide_value;
	// look for */*/property specific
	auto[l_global_found, l_global_value] = getStyleSpecific("*", "*", p_property);
	if (l_global_found) return l_global_value;
	// look for */*/* specific
	auto[l_universal_found, l_universal_value] = getStyleSpecific("*", "*", "*");
	if (l_universal_found) return l_universal_value;

	return 0;
}

void Editor::styleKeeper::setStyle(std::string p_component_type, std::string p_variable_name, std::string p_property,
	int p_value)
{
	m_maps[p_component_type][p_variable_name][p_property] = p_value;
}

void Editor::styleKeeper::clearStyle(std::string p_component_type, std::string p_variable_name, std::string p_property)
{
	m_maps[p_component_type][p_variable_name][p_property] = 0;
}

std::tuple<bool, unsigned int> Editor::styleKeeper::getStyleSpecific(std::string p_component_type, std::string p_variable_name, std::string p_property) const
{
	auto l_iter1 = m_maps.find(p_component_type);
	if (l_iter1 != m_maps.end())
	{
		auto l_iter2 = l_iter1->second.find(p_variable_name);
		if(l_iter2 != l_iter1->second.end())
		{
			auto l_iter3 = l_iter2->second.find(p_property);
			if(l_iter3 != l_iter2->second.end())
			{
				return std::make_tuple(true, l_iter3->second);
			}
		}
	}

	return std::make_tuple(false, 0);
}
