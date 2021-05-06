/*!***************************************************************************************
\file       bundleManipulatorInterface.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include <vector>
#include <string>

class bundleManipulatorInterface
{
public:
	//virtual void resumeSystem(std::string p_bundle_name) = 0;
	//virtual void haltSystem(std::string p_bundle_name) = 0;
	virtual void enableBundle(std::string p_bundle_name) = 0;
  virtual void suspendBundle(std::string p_bundle_name) = 0;
	virtual void disableBundle(std::string p_bundle_name) = 0;

	virtual std::vector<std::string> getAllBundles() = 0;
	virtual std::vector<std::string> getActiveBundles() = 0;
};

