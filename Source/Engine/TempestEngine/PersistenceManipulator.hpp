/*!***************************************************************************************
\file       PersistenceManipulator.hpp
\author     David Roberts
\date       4/13/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Pure virtual base class for persistence requests. Used by PersistenceSystem.
*****************************************************************************************/
#pragma once

#include "PersistenceManipulatorInterface.hpp"
#include "PersistenceRequest.hpp"

class systemManagerInterface;

class persistenceManipulator : public persistenceManipulatorInterface
{
public:
	persistenceManipulator(systemManagerInterface * p_system_manager);
	virtual ~persistenceManipulator() = default;

	void setPersistenceLocation(const std::string& p_data_location);
	void saveCurrentScene() override;
	void saveCurrentSceneAs(std::string p_name) override;
	std::shared_ptr<scene> loadScene(std::string p_name);
	void load(std::string p_name) override;
	void new_scene() override;

    /*!***************************************************************************************
    \brief  Deletes particular scene
    \param name - name of scene to delete
    *****************************************************************************************/
    void delete_scene(const std::string & name);

	void quickStoreTo() override;
	void quickRestoreFrom() override;

private:
	systemManagerInterface * m_system_manager;
	std::string m_persistence_location;
	bool m_is_storing;
};

