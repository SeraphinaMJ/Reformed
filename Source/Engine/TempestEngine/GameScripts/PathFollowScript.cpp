/*!***************************************************************************************
\file       PathFollowScript.cpp
\author     Cody Cannell, Alex Kudebeh
\date       10/03/2019
\copyright  All content  2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief   Script to give objects a path to follow
*****************************************************************************************/
#include "PathFollowScript.hpp"
#include "Transform.hpp"
#include "PathingStateScript.hpp"
#include "RigidBody.hpp"

//!The squared distance from a node to consider it "reached"
#define REACHED 1.0f

void pathFollowScript::onStart()
{
	//Do nothing
}

pathFollowScript::pathFollowScript(typeRT p_data) : scriptCPP(p_data)
{
	auto l_members = p_data.members();

	if (l_members.find("moveSpeed") != l_members.end())
	{
		m_moveSpeed = p_data.member("moveSpeed").getFloat();
	}
}

typeRT pathFollowScript::toTypeRT() const
{
	//Generate the base script data
	typeRT l_result = scriptCPP::toTypeRT();

	typeRT l_playerNumber("moveSpeed", m_moveSpeed);
	l_result.insertMember(l_playerNumber);

	return l_result;
}

void pathFollowScript::onUpdate()
{
	//If disabled, don't bother doing anything
	if (m_pathingEnabled == false)
		return;

	componentHandle<rigidBody> myrb = this->getGameObject().lock()->getComponent<rigidBody>();

	//Get a new path if I don't have one yet
	if (m_currentPath == nullptr)
	{
		if (/*m_paths.size()*/!m_paths.empty())
		{
			if (setNextPathAsCurrentPath())
			{
				this->getGameObject().lock()->getComponent<pathingStateScript>()->setState(pathingStateScript::PSPathing);
			}
			//New path failed
			else
				return;
		}
		else
		{
			//No paths exist, do nothing
			return;
		}
	}

	componentHandle<transform> myTrans = this->getGameObject().lock()->getComponent<transform>();




	//If current path bad, get a new one
	if (m_currentPath->isBad())
	{
		redoPath(m_currentPath->points.back());
		//Error check - path found?
		if (m_currentPath->isBad())
		{
			return;
		}
	}

	//Guarenteed to have good path now
	vector3 moveVector = (*m_nodeIndex) - myTrans->getGlobalPosition();
	//If reached node
	if (moveVector.distanceSquared() <= REACHED)
	{
		if (setNextNode() == false)
			return; //Something failed
		moveVector = (*m_nodeIndex) - myTrans->getGlobalPosition();
	}

	moveVector.normalized();
    
	myrb->setVelocity(moveVector * m_moveSpeed);
}

bool pathFollowScript::setNextNode()
{
	if (m_reversing)
	{
		if (m_nodeIndex == m_currentPath->points.begin())
		{
			if (setNextPathAsCurrentPath() == false)
			{
				redoPath(m_currentPath->points.back());
				if (m_currentPath->isBad())
					return false;
			}
			return true;
		}
		else
		{
			--m_nodeIndex;
			return true;
		}
	}

	//Get next node
	++m_nodeIndex;
	//If reached the end of the current path
	if (m_nodeIndex == m_currentPath->points.end())
	{
		//Get next path
		if (m_paths.size() != 0)
		{
			return setNextPathAsCurrentPath();
		}
		else //if (m_paths.size() == 0)
		{
			m_currentPath = nullptr;
			this->getGameObject().lock()->getComponent<pathingStateScript>()->setState(pathingStateScript::PSHome);
			if (!m_goingHome && m_hasHomePosition && !m_patrol)
			{
				m_goingHome = true;
				redoPath(m_homePosition);
				if (m_currentPath->isBad())
					return false;
				m_nodeIndex = m_currentPath->points.begin();
				return true;
			}
			//No home path
			return false;
		}
	}

	return true;
}

void pathFollowScript::onShutdown()
{
	//Probably not needed, but just in case
	dumpPaths();
}

void pathFollowScript::addPathToObject(std::shared_ptr<graphPath>& p_path)
{
	m_paths.push_back(p_path);

	if (m_goingHome)
	{
		m_goingHome = false;
		m_currentPath = nullptr;
	}
}

void pathFollowScript::dumpPaths()
{
	m_currentPath = nullptr;
	if (m_paths.size() > 0)
		m_paths.clear();

	m_reversing = false;
	m_pathIndex = -1;
	m_prePatrol = true;

	this->getGameObject().lock()->getComponent<pathingStateScript>()->setState(pathingStateScript::PSHome);
}

void pathFollowScript::addPathsToObject(std::vector<std::shared_ptr<graphPath>>& p_paths)
{
	for (auto l_path : p_paths)
	{
		m_paths.push_back(l_path);
	}
	if (m_goingHome)
	{
		m_goingHome = false;
		m_currentPath = nullptr;
	}
}

bool pathFollowScript::setNextPathAsCurrentPath()
{
	if (!m_patrol || (m_patrol && m_prePatrol))
	{
		m_prePatrol = false;
		m_currentPath = m_paths.front();
		m_paths.erase(m_paths.begin());
		m_nodeIndex = m_currentPath->points.begin();

		return !(m_currentPath->isBad());
	}
	else //Patrol logic
	{
		//Set Path
		if (m_reversing)
		{
			if (m_pathIndex == 0) //Just finished first path, switch back to non-reversing
				m_reversing = false;
			else
				--m_pathIndex;
		}
		else
		{
			if (m_pathIndex == static_cast<int>(m_paths.size() - 1)) //Finished last path, reverse now
				m_reversing = true;
			else
				++m_pathIndex;
		}

		std::list<std::shared_ptr<graphPath>>::iterator it = m_paths.begin();
		std::advance(it, m_pathIndex);
		m_currentPath = *it;

		//Set Node
		m_nodeIndex = m_currentPath->points.begin();

		if (m_reversing)
		{
			m_patrolNodeIndex = m_currentPath->points.size() - 1;
			std::advance(m_nodeIndex, m_patrolNodeIndex);
		}

		return !(m_currentPath->isBad());
	}
}

void pathFollowScript::redoPath(vector3 target)
{
	vector3 myPos = this->getGameObject().lock()->getComponent<transform>()->getGlobalPosition();

	//If patrolling, need a permanent new path for the list as well as currentPath
	if (m_patrol && !m_prePatrol)
	{
		auto pathsIterator = m_paths.begin();
		std::advance(pathsIterator, m_pathIndex);
		(*pathsIterator) = this->getSystemManager()->getSystem<waypointGraphPathfinder>()->getPath(myPos, target);
		m_currentPath = (*pathsIterator);
		m_nodeIndex = m_currentPath->points.begin();
		return;
	}

	m_currentPath = this->getSystemManager()->getSystem<waypointGraphPathfinder>()->getPath(myPos, target);

	m_nodeIndex = m_currentPath->points.begin();
}

bool pathFollowScript::reachedEndOfCurrentPath() const
{
	return hasPaths();
}

void pathFollowScript::setHomePosition(vector3 p_pos)
{
	m_hasHomePosition = true;
	m_homePosition = p_pos;
}

vector3 pathFollowScript::getHomePosition()
{
	return m_homePosition;
}

bool pathFollowScript::hasPaths() const
{
	return (m_paths.size() != 0);
}

void pathFollowScript::StopPathing()
{
	m_pathingEnabled = false;
}

void pathFollowScript::ContinuePathing()
{
	m_pathingEnabled = true;
}

void pathFollowScript::setIsPatrolUnit(bool p_isPatrolUnit)
{
	m_patrol = p_isPatrolUnit;
}
