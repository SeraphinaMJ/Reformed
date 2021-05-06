/*!***************************************************************************************
\file       bundleManipulator.cpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/

#include "bundleManipulator.hpp"
#include "../SystemManagement/SystemManagerInterface.hpp"

bundleManipulator::bundleManipulator(systemManagerInterface * p_system_manager)
	: m_system_manager{p_system_manager}
{
}

void bundleManipulator::enableBundle(std::string p_bundle_name)
{
	m_system_manager->enableBundle(p_bundle_name);
}

void bundleManipulator::suspendBundle(std::string p_bundle_name)
{
  m_system_manager->suspendBundle(p_bundle_name);
}

void bundleManipulator::disableBundle(std::string p_bundle_name)
{
	m_system_manager->disableBundle(p_bundle_name);
}

std::vector<std::string> bundleManipulator::getAllBundles()
{
	return m_system_manager->getAllBundleNames();
}

std::vector<std::string> bundleManipulator::getActiveBundles()
{
	return m_system_manager->getActiveBundleNames();
}


