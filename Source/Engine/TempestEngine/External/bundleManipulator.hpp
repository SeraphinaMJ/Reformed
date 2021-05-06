/*!***************************************************************************************
\file       bundleManipulator.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include "bundleManipulatorInterface.hpp"

class systemManagerInterface;

class bundleManipulator : public bundleManipulatorInterface
{
public:
	bundleManipulator(systemManagerInterface * p_system_manager);
	virtual ~bundleManipulator() = default;

	void enableBundle(std::string p_bundle_name) override;
	void suspendBundle(std::string p_bundle_name) override;
	void disableBundle(std::string p_bundle_name) override;

	std::vector<std::string> getAllBundles() override;
	std::vector<std::string> getActiveBundles() override;

private:
	systemManagerInterface * m_system_manager;
};

