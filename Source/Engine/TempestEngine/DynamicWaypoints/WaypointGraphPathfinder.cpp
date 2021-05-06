/*!***************************************************************************************
\file       WaypointGraphPathfinder.cpp
\author     Alex Kudebeh
\date       10/11/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation file for pathfinding with a waypoint graph
*****************************************************************************************/

/*
Notes to future self
can speed up by using binary heap for pathfinding
ask for help with structure regarding holding graphpaths and clearing memory
	pathfinder needs them for invalidation
	could use weak_ptr, but then how do I delete old nodes?
	scripts need them for pathing
*/

//======== Self Include ================================================================//
#include "WaypointGraphPathfinder.hpp"
//======== 1st Party Includes ==========================================================//
#include "Engine.hpp"
#include "GameObject.hpp"
#include "GameObjectGatherer.hpp"
#include "ComponentTypeSet.hpp"
#include "Transform.hpp"
//======== 3rd Party Includes ==========================================================//
#include <queue>
#include <limits>
#include <memory>
//======== Types =======================================================================//
/*!***************************************************************************************
\par struct: pathNode
\brief   A node made for pathfinding using the graph
*****************************************************************************************/
struct pathNode
{
	/*!***************************************************************************************
	\brief  constructor taking all necessary elements
	\param ln - The waypoint node component handle this pathNode uses
	\param c - The cost to reach this node
	*****************************************************************************************/
	pathNode(componentHandle<waypointNode> ln, float c, std::shared_ptr<pathNode> parent) :
		parentNode(parent), levelNode(ln), cost(c), name(ln->debugName)
	{}

	/*!***************************************************************************************
	\brief  copy constructor
	\param other - The other node to copy
	*****************************************************************************************/
	pathNode(const pathNode & other)
	{
		parentNode = other.parentNode;
		levelNode = other.levelNode;
		cost = other.cost;
		name = other.name;
	}

	//!<The parent of this node
	std::shared_ptr<pathNode> parentNode;
	//!<The node this refers to in the waypoint graph
	componentHandle<waypointNode> levelNode;
	//!<The cost to reach this node
	float cost;
	//!<The name of this path node (for debugging)
	std::string name;
};

/*!***************************************************************************************
\par class pathNodeComparer
\brief   Used to compare two shared pointers to path nodes in a priority queue
*****************************************************************************************/
class pathNodeComparer
{
public:
	/*!***************************************************************************************
	\brief  overloaded operator(); compares two shared ptrs to path Nodes
	\param a - One shared ptr to compare
	\param b - The other shared ptr to compare
	\return True if a is greater than b, false otherwise
	*****************************************************************************************/
	bool operator() (const std::shared_ptr<pathNode> & a, const std::shared_ptr<pathNode> & b) const
	{
		return (a->cost > b->cost);
	}
};

//======== Defines =====================================================================//
//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
graphPath::graphPath(std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> nodes_)
	: nodes(nodes_)
{
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
void waypointGraphPathfinder::onUpdate()
{
	cleanupMemory();
}

//======== Functionality ===============================================================//
bool graphPath::isBad() const
{
	return bad;
}

void waypointGraphPathfinder::cleanupMemory()
{
	//Check all graphPaths for non-usage
	systemBase * s = getSystemManager()->getSystem("dynamicWaypointGraphSystem");
	dynamicWaypointGraph * dwg = dynamic_cast<dynamicWaypointGraph *>(s);

	std::list<std::shared_ptr<graphPath>>::iterator it = paths.begin();

	while (it != paths.end())
	{
		if (it->use_count() == 1)
		{
			std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> nodes = (*it)->nodes;
			dwg->deleteNode(nodes.first);
			dwg->deleteNode(nodes.second);

			it = paths.erase(it);
		}
		else
			++it;
	}
}

std::shared_ptr<graphPath> waypointGraphPathfinder::getPath(vector3 startPoint, vector3 endPoint, bool addEndVisualization, bool addStartVisualization)
{
	std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> startEndNodes =
		getSystemManager()->getSystem<dynamicWaypointGraph>()->handlePathRequest(startPoint, endPoint, addEndVisualization, addStartVisualization);

	//A* from start node to end node
	std::priority_queue<std::shared_ptr<pathNode>, std::vector<std::shared_ptr<pathNode> >, pathNodeComparer> queue;
	std::list<std::shared_ptr<pathNode>> closedList;

	//push start node on open list
	std::shared_ptr<pathNode> newNode (std::make_shared<pathNode>(pathNode(startEndNodes.first, 0, nullptr)));
	queue.push(newNode);

	//while open not empty
	while (queue.empty() == false)
	{
		//pop cheapest
		std::shared_ptr<pathNode> cheapest = queue.top();
		queue.pop();

		//if goal, path found
		if (cheapest->levelNode == startEndNodes.second)
		{
			//construct and return path
			std::shared_ptr<graphPath> path = std::make_shared<graphPath>(graphPath(startEndNodes));
			while (cheapest != nullptr)
			{
				if (cheapest->parentNode != nullptr)
				{
				path->points.push_front(cheapest->levelNode->getGameObject().lock()->getComponent<transform>()->getGlobalPosition());
					path->usedConnections.push_back(nodeConnection(cheapest->levelNode, cheapest->parentNode->levelNode));
				}
				else
					path->points.push_front(cheapest->levelNode->getGameObject().lock()->getComponent<transform>()->getGlobalPosition());
					
				cheapest = cheapest->parentNode;
			}
			paths.push_back(path);
			return path;
		}
		//else for all neighbors
		std::vector<connectedNode> neighbors = cheapest->levelNode->connections;
		for (unsigned i = 0; i < neighbors.size(); ++i)
		{
			std::string neighborName = neighbors[i].node->debugName;

			//compute cost - current travel cost + heuristic
			float newCost = neighbors[i].cost + cheapest->cost;
			bool nodeFound = false;

			//if neighbor on open or closed, and this parent cheaper, remove old expensive one and put new cheaper on open
			//cannot iterate through a queue, so only check closed list
			//could be optimized with a binary heap

			std::list<std::shared_ptr<pathNode>>::iterator it = closedList.begin();
			//check closed list
			while (it != closedList.end())
			{
				if ((*it)->levelNode == neighbors[i].node)
				{
					nodeFound = true;
					//On closed list
					if ((*it)->cost > newCost)
					{
						//cost greater
						closedList.erase(it);
						newNode = std::make_shared<pathNode>(pathNode(neighbors[i].node, newCost, cheapest));
						queue.push(newNode);
					}
					break;
				}
				++it;
			}
			//if not on open or closed, put on open
			if (!nodeFound)
			{
				newNode = std::make_shared<pathNode>(pathNode(neighbors[i].node, newCost, cheapest));
				queue.push(newNode);
			}
		}
		//place parent on closed
		closedList.push_back(cheapest);
	}
	//open list empty, thus no path (return "fail")
	std::shared_ptr<graphPath> badPath = std::make_shared<graphPath>(graphPath(startEndNodes));
	badPath->points.push_back(startPoint);
	badPath->points.push_back(endPoint);
	badPath->bad = true;

	return badPath;
}

void waypointGraphPathfinder::nodeConnectionInvalidated(const nodeConnection & invalidated)
{
	//foreach graph path
	std::list<std::shared_ptr<graphPath>>::iterator it = paths.begin();
	while (it != paths.end())
	{
		//if graph path uses any of the invalidated connections
		std::list<nodeConnection>::iterator j = (*it)->usedConnections.begin();
		while (j != (*it)->usedConnections.end())
		{
			if (*j == invalidated)
			{
				//mark graph path as bad
				(*it)->bad = true;
				break;
			}
			++j;
		}
		++it;
	}
}

//======== Helper Functions ============================================================//

