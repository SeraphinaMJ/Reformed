/*!***************************************************************************************
\file       SystemManagerInterface.hpp
\author     Aaron Damyen
\date       4/13/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the interface for a System Manager
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
//======== 3rd Party Includes ==========================================================//
#include <Configuration.hpp>
#include <functional>
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
class gameObjectGatherer;
class systemBase;
class systemBundle;
class task;


/*!***************************************************************************************
\par class: systemManagerInterface
\brief   This is an interface for any System Manager used by the engine
*****************************************************************************************/
class systemManagerInterface
{
public:
	virtual ~systemManagerInterface() = default;

	virtual gameObjectGatherer * getGameObjectGatherer() const = 0;
	virtual systemBase * getSystem(const std::string & p_system_name) = 0;
	virtual systemBundle * getBundle(const std::string & p_bundle_name) = 0;
	virtual void setGameObjectGatherer(gameObjectGatherer * p_go_filter) = 0;
	virtual configurationFunctionPtr getConfigurationFunction() const = 0;
	template<typename T>
	T * getSystem() { return reinterpret_cast<T *>(getSystem(T::getName())); }
	virtual bool isSystemActive(systemBase * p_system_ptr) = 0;

	virtual void addSystemBundles(const std::vector<systemBundle*> & p_system_bundles) = 0;
	virtual void initializeSystemBundles(const std::vector<std::string> & p_system_bundles) = 0;
	virtual void activateSystemBundles(const std::vector<std::string> & p_system_bundles) = 0;
	virtual void enableBundle(const std::string & p_bundle_name) = 0;
  virtual void suspendBundle(const std::string & p_bundle_name) = 0;
	virtual void disableBundle(const std::string & p_bundle_name) = 0;
	virtual std::vector<std::string> getAllBundleNames() const = 0;
	virtual std::vector<std::string> getActiveBundleNames() const = 0;
	virtual std::vector<std::string> getSystemNamesInBundle(const std::string & p_bundle_name) = 0;

	virtual void sendTask(task & p_task) = 0;

	virtual void initialize(std::function<configuration()> p_conf_function) = 0;
	virtual void update() = 0;
	virtual void shutdown() = 0;
	virtual void sendMessage(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data) = 0;

    virtual void pauseEngine() = 0;
    virtual void resumeEngineFromPause() = 0;
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/
