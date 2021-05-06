/*!**********************************************************************************************************
\file       SystemManagerSTA.hpp
\author     Aaron Damyen
\date       11/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#pragma once

#include <Configuration.hpp>
#include <algorithm>
#include <map>
#include "../SystemBundle.hpp"
#include "../SystemManagement/SystemManagerInterface.hpp"


class gameObjectGatherer;

class systemManagerSTA final : public systemManagerInterface
{
public:
	systemManagerSTA();

	void addSystemBundles(const std::vector<systemBundle*>& p_system_bundles) override;
	void initializeSystemBundles(const std::vector<std::string> & p_system_bundle_names) override;
	void activateSystemBundles(const std::vector<std::string> & p_system_bundle_names) override;

	void waitForBundleInitialization(std::vector<std::string> const & p_system_bundle_names);
	systemBundle * getBundle(const std::string & p_bundle_name) override;
	systemBase * getSystem(const std::string & p_system_name) override;
	template<typename T>
	T * getSystem() { return reinterpret_cast<T *>(getSystem(T::getName())); }
	configurationFunctionPtr getConfigurationFunction() const override;
	bool isSystemActive(systemBase* p_system_ptr) override;

	std::vector<std::string> getAllBundleNames() const override;
	std::vector<std::string> getActiveBundleNames() const override;
	std::vector<std::string> getSystemNamesInBundle(const std::string& p_bundle_name) override;
	void enableBundle(const std::string & p_bundle_name) override;
  void suspendBundle(const std::string & p_bundle_name) override;
	void disableBundle(const std::string & p_bundle_name) override;

	void sendTask(task & p_task) override;

	void setGameObjectGatherer(gameObjectGatherer * p_gatherer) override;
	gameObjectGatherer * getGameObjectGatherer() const override;
	void initialize(std::function<configuration()> p_conf_function) override;
	void update() override;
	void shutdown() override;

    void pauseEngine() override;
    void resumeEngineFromPause() override;
private:
	void activateSystemBundle(const std::string & p_system_bundle_name);
	void suspendSystemBundle(const std::string & p_system_bundle_name);
  void shutdownSystemBundle(const std::string & p_system_bundle_name);
	bool alreadyActive(const systemBundle * p_bundle);
	
	std::map<std::string, systemBundle*> m_system_bundle_master;
	std::map<std::string, systemBase*> m_system_master;
	std::vector<systemBundle*> m_active_system_bundles;

	configurationFunctionPtr m_conf_function;
	gameObjectGatherer* m_go_gatherer;
	void sendMessage(std::string p_sender, std::string p_destination, unsigned int p_message_code, void * p_data) override;

    // Oh god this is such a horrible way to do it
	decltype(std::chrono::high_resolution_clock::now()) m_last_time = std::chrono::high_resolution_clock::now();
};
