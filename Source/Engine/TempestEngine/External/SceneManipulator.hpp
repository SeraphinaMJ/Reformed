/*!***************************************************************************************
\file       SceneManipulator.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include "SceneManipulatorInterface.hpp"

#include <vector>
#include <string>
#include <memory>

class engine;
class systemManagerInterface;
class scene;
class space;
class gameObject;
class gameObjectGatherer;

class sceneManipulator : public sceneManipulatorInterface
{
public:
	sceneManipulator(engine * p_engine, systemManagerInterface * p_system_manager, gameObjectGatherer * p_game_object_gatherer);

    objID getParentID(objID p_object_id) override;
    objID getSpaceIDForObject(objID p_object_id) override;
    objID getSpaceIDFromName(std::string name) override;
	std::vector<objID> getSceneIDs() override;
    std::string getSceneName(objID p_scene_id) override;
    std::vector<objID> getSpaceIDsForScene(objID p_scene_id) override;
    std::string getSpaceName(objID p_space_id) override;
	std::vector<objID> getObjectIDsForSpace(objID p_space_id) override;
	std::vector<objID> getTopObjectIDsForSpace(objID p_space_id) override;
	std::vector<objID> getChildObjectIDsForObject(objID p_object_id) override;
	std::string getObjectName(objID p_object_id) override;
	typeRT getTypeRT(objID p_id) override;
	void applyTypeRT(objID p_id, typeRT & p_type) override;

  std::shared_ptr<scene> addEmptyScene() override;
  std::shared_ptr<space> addEmptySpace() override;
  std::shared_ptr<space> addEmptySpace(objID p_scene_id) override;
	objID addEmptySpace( const std::string& p_name) override;

  std::shared_ptr<gameObject> addEmptyGameObject() override;
  std::shared_ptr<gameObject> addEmptyGameObject(objID p_parent_id) override;
	objID addEmptyGameObject(objID p_parent_id, const std::string& p_name) override;
	void addRenderedGameObject(float p_x, float p_y, float p_z, std::string p_object_name, std::string p_texture) override;

	void addGameObjectComponent(objID p_object_id, const std::string & p_component_type) override;
	void removeGameObjectComponent(objID p_object_id, const std::string & p_component_type) override;


	void readObjectTransform(objID p_object_id, matrixArray1D& p_transform_matrix) override;
	void writeObjectTransform(objID p_object_id, matrixArray1D const& p_transform_matrix) override;
	void createEditorCamera() override;
	void removeEditorCamera() override;
  void tellBulletToOverwriteTransform(objID p_object_id) override;

  void setButtonNeighbor(objID p_objectID, objID p_neighborID, int p_neighbor) override;
  objID removeButtonNeighbor(objID p_objectID, int p_neighbor) override;
  objID getObjectID(std::string p_objectName) override;

  void setCollisionLayer(objID p_object_id, unsigned int p_data, unsigned int p_type) override;

  void removeGameObject(objID p_objectID) override;

  void addParentToGameObject(objID p_parent_id, objID p_child_id) override;
  void removeParent(objID p_child_id) override;

  void setGameObjectName(objID p_object_id, const std::string & p_name) override;
  void setSceneName(objID p_scene_id, const std::string & p_name) override;
  void setSpaceName(objID p_space_id, const std::string & p_name) override;

  void deleteSpace(objID p_space_id) override;
	void deleteSpace(const std::string& p_space_name) override;

  void setUISpriteMaterial(objID p_sprite_id, const std::string & p_name) override;

  void moveObjectToSpace(objID p_object_id, const std::string & p_space) override;
  void moveObjectToSpace(objID p_object_id, objID p_space_id) override;

  void dynamicWaypointGraphCreatePath() override;
  void dynamicWaypointGraphCreateSetPaths() override;
  void dynamicWaypointGraphStitchPaths() override;
  std::vector<objID> dynamicWaypointGraphClear() override;
  void dynamicWaypointGraphDeleteRandomNodeSet() override;
  void dynamicWaypointGraphTestFunction() override;

  objID getObjectFromClick(float p_mousePosX, float p_mousePosY) override;

  void setPhysicsSystemTimeStep(float p_time_step) override;
  void setPhysicsEngineRunning(bool p_running) override;
  void addPhysicsObjectFromEditor(objID p_object) override;
  void setPhysicsIsDirtyFlag(objID p_object) override;
  void setPhysicsGravity(vector3 p_gravity) override;

private:
	engine * m_engine;
	systemManagerInterface * m_system_manager;
	gameObjectGatherer * m_game_object_gatherer;

	std::shared_ptr<scene> findSceneByID(objID p_scene_id);
	std::shared_ptr<space> findSpaceByID(objID p_space_id);
  std::shared_ptr<space> findSpaceByName(const std::string & p_space);
	std::shared_ptr<gameObject> findGameObjectByID(objID p_object_id);
  std::shared_ptr<gameObject> findGameObjectByName(std::string p_objectName);
};
