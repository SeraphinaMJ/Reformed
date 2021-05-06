/*!***************************************************************************************
\file       PathFollowScript.hpp
\author     Cody Cannell, Alex Kudebeh
\date       10/03/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief   Script to give objects a path to follow
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Components/ScriptCPP.hpp"
#include "../DynamicWaypoints/WaypointGraphPathfinder.hpp"
#include <Vector3.hpp>
#include <list>


class pathFollowScript final : public scriptCPP
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Default constructor. Does nothing
	*****************************************************************************************/
	pathFollowScript() = default;

	/*!***************************************************************************************
	\brief  Conversion constructor. Copies move speed
	\param  p_data - The typeRT to convert this from
	*****************************************************************************************/
	pathFollowScript(typeRT p_data);

	/*!***************************************************************************************
	\brief  Type getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const override { return pathFollowScript::getType(); }

	/*!***************************************************************************************
	\brief  Static type getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forInherited<pathFollowScript, scriptCPP>(); return s_type; }

	/*!***************************************************************************************
	\brief  Override for type detail getter
	\retrun The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "pathFollowScript", true, true, true); }

	/*!***************************************************************************************
	\brief  Converts the script to a typeRT.
	\return The scripts as a typeRT
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Overrides onStart. Does nothing
	*****************************************************************************************/
	void onStart() override;

	/*!***************************************************************************************
	\brief  Overrides onUpdate. Causes the agent to follow their path, if enabled and has a path
	*****************************************************************************************/
	void onUpdate() override;

	/*!***************************************************************************************
	\brief  Overrides onShutdown. Clears memory
	*****************************************************************************************/
	void onShutdown() override;

	/*!***************************************************************************************
	\brief  Disables all pathing operations until ContinuePathing is called
	*****************************************************************************************/
	void StopPathing();

	/*!***************************************************************************************
	\brief  Enables all pathing operations. Call after StopPathing to resume pathing
	*****************************************************************************************/
	void ContinuePathing();

	/*!***************************************************************************************
	\brief  Adds a path to the path list
	\param  p_path - The path to add
	*****************************************************************************************/
	void addPathToObject(std::shared_ptr<graphPath>& p_path);

	/*!***************************************************************************************
	\brief  Adds a vector of paths to the path list
	\param  p_paths - The vector of paths to add
	*****************************************************************************************/
	void addPathsToObject(std::vector<std::shared_ptr<graphPath>>& p_paths);

	/*!***************************************************************************************
	\brief  Removes all paths from the list of paths
	*****************************************************************************************/
	void dumpPaths();

	/*!***************************************************************************************
	\brief  Returns if the agent has any paths left
	\return True if there are no paths left, false if there is more pathing to do
	*****************************************************************************************/
	bool hasPaths() const;

	/*!***************************************************************************************
	\brief  Returns if the agent has reached the end of all paths given
	\return True if there are no paths left, false if there is more pathing to do
	*****************************************************************************************/
	bool reachedEndOfCurrentPath() const;

	/*!***************************************************************************************
	\brief  Sets the home position of the agent
	\param  p_pos - The vec 3 that hold the postision
	*****************************************************************************************/
	void setHomePosition(vector3 p_pos);

	/*!***************************************************************************************
	\brief  Gets the home position
	\return The position
	*****************************************************************************************/
	vector3 getHomePosition();

	/*!***************************************************************************************
	\brief  Setter for if this should use patrol logic or one and done logic
	\param  p_isPatrolUnit - If the unit should patrol or not
	*****************************************************************************************/
	void setIsPatrolUnit(bool p_isPatrolUnit);

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:
	/*!***************************************************************************************
	\brief  Sets the next path as the current path
	\return True if the path is good, false if the path is bad
	*****************************************************************************************/
	bool setNextPathAsCurrentPath();

	/*!***************************************************************************************
	\brief  Sets the current path to a new path using current position to the target
	\param  The target to set the current path to
	*****************************************************************************************/
	void redoPath(vector3 target);

	//returns success. If false, current path invalid
	bool setNextNode();

	//!The vector to move along
	vector3 m_moveVector;
	//!How fast to move towards next node
    float m_moveSpeed = 10.5f; // 10.5f;

	//!If this script should follow paths or not
	bool m_pathingEnabled = true;

	//!The place we return to after following the path
	vector3 m_homePosition;

	//!Do we have a place to return to
	bool m_hasHomePosition = false;

	//!Are we going home?
	bool m_goingHome = false;

	//!The node to move towards
	std::list<vector3>::iterator m_nodeIndex;
	//!The current path to follow
	std::shared_ptr<graphPath> m_currentPath;
	//!The list of paths to follow
	std::list<std::shared_ptr<graphPath>> m_paths;

	//!If this unit is patrolling or not
	bool m_patrol = false;
	//!If true, go through m_reversePaths instead of m_paths
	bool m_reversing = false;
	//!Which path we are currently following. Used exclusively for patrolling
	int m_pathIndex = -1;
	//!Which node in the path we are moving towards
	int m_patrolNodeIndex = -1;
	//!If the unit is in "pre-patrol" state
	bool m_prePatrol = true;
};
