/*!***************************************************************************************
\file       DynamicWaypointGraph.cpp
\author     Alex Kudebeh
\date       9/10/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Implementation for the dynamic waypoint graph system used for AI pathfinding
*****************************************************************************************/
//======== Self Include ================================================================//
#include "DynamicWaypointGraph.hpp"
//======== 1st Party Includes ==========================================================//
#include "Transform.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "GameObjectGatherer.hpp"
#include "DebugLines.hpp"
#include "TestingScript.hpp"
#include "WaypointGraphPathfinder.hpp"
#include "WaypointNode.hpp"
#include "WaypointNodeSet.hpp"
//======== 3rd Party Includes ==========================================================//
#include <Vector3.hpp>
#include <Vector2.hpp>
#include <random>
//======== Types =======================================================================//

/*!***************************************************************************************
\par struct: intersection
\brief   Holds information regarding two connections intersecting
*****************************************************************************************/
struct intersection
{
	/*!***************************************************************************************
	\brief  intersection constructor
	\param oldCon - The connection that was being tested against
	\param newCon - The connection that was looking for intersections
	\param intersectionPoint_ - The point of intersection in world space
	*****************************************************************************************/
	intersection(nodeConnection oldCon, nodeConnection newCon, vector3 intersectPoint_)
		: oldConnection(oldCon), newConnection(newCon), intersectPoint(intersectPoint_)
	{
	}

	//!<The already valid connection being tested against
	nodeConnection oldConnection;
	//!<The new connection that is being tested
	nodeConnection newConnection;

	/*!***************************************************************************************
	\brief  comparison operator
	\param other - The intersection to compare against
	\return True if old and new connections match
	*****************************************************************************************/
	bool operator== (const intersection & other)
	{
		return (this->oldConnection == other.oldConnection && this->newConnection == other.newConnection);
	}

	//!<The point of intersection
	vector3 intersectPoint;
};

//======== Defines =====================================================================//
#define DEBUG_LINE_TAG "graph"
#define PATH_LINE_TAG "pathing"

//======== Static Declarations =========================================================//

//======== Constructors & Destructor ===================================================//
parameterNode::parameterNode(componentHandle<waypointNode> node_, componentHandle<waypointNodeSet> otherOwner_, bool connectsForward_) :
	node(node_), otherOwner(otherOwner_), connectsForward(connectsForward_)
{
}

nodeConnection::nodeConnection(componentHandle<waypointNode> firstNode_, componentHandle<waypointNode> secondNode_, bool onObjectParameter_) :
	firstNode(firstNode_), secondNode(secondNode_), onObjectParameter(onObjectParameter_)
{
	name = firstNode->debugName + " to " + secondNode->debugName;
}

connectedNode::connectedNode(componentHandle<waypointNode> node_, float cost_, bool sharesObjectParameter_)
	: node(node_), cost(cost_), sharesObjectParameter(sharesObjectParameter_)
{}

parameterConnection::parameterConnection(componentHandle<waypointNode> node1, componentHandle<waypointNode> node2)
{
	nodeList.push_back(parameterNode(node1, componentHandle<waypointNodeSet>(), true));
	nodeList.push_back(parameterNode(node2, componentHandle<waypointNodeSet>(), false));
}

//======== Getters & Setters ===========================================================//

//======== Overrides ===================================================================//
bool nodeConnection::operator==(nodeConnection other) const
{
	if ((this->firstNode == other.firstNode && this->secondNode == other.secondNode) ||
		(this->firstNode == other.secondNode && this->secondNode == other.firstNode))
		return true;
	return false;
}

void dynamicWaypointGraph::onLevelLoad(const levelLoadEvent & event)
{
	createNodeSetPaths();
}

void dynamicWaypointGraph::onLevelUnload(const levelUnloadEvent & event)
{
	clear(false);
}

//======== Functionality ===============================================================//
//----------------
//EDITOR FUNCTIONS
//----------------

void dynamicWaypointGraph::createLevelPath()
{
	GameObjectFiltering::componentTypeSet node_pattern;
	node_pattern.setType(waypointNode::getType());
	std::vector<std::shared_ptr<gameObject>> nodes = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

	//This creates paths between every node in the scene, IGNORING LOS
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		componentHandle<waypointNode> n = nodes[i]->getComponent<waypointNode>();
		for (size_t j = i + 1; j < nodes.size(); ++j)
		{
			componentHandle<waypointNode> n2 = nodes[j]->getComponent<waypointNode>();
			nodeConnection newConnection = connectNodesWithConnection(n, n2);
			validConnections.push_back(newConnection);
			addDebugLine(newConnection);
		}
	}
}

void dynamicWaypointGraph::createNodeSetPaths()
{
	clear(false);
	//Get all objects with node sets
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	for (size_t i = 0; i < nodeSets.size(); ++i)
	{
		componentHandle<waypointNodeSet> n = nodeSets[i]->getComponent<waypointNodeSet>();
		createNodeSet(n);
		handleNodeSetCreation(n);
	}
	systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
	if (sys != nullptr)
	{
		waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
		wgpf->cleanupMemory();
	}
}

void dynamicWaypointGraph::stitchPath()
{
	clear();
	createLevelPath();

	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	for (size_t i = 0; i < nodeSets.size(); ++i)
	{
		createNodeSet(nodeSets[i]->getComponent<waypointNodeSet>());
		handleNodeSetCreation(nodeSets[i]->getComponent<waypointNodeSet>());
	}
}

std::vector<objID> dynamicWaypointGraph::clear(bool inEditor)
{
	//clear all valid and invalid connections
	dumpMemory();

	if (!inEditor)
		return std::vector<objID>();

	std::vector<objID> toReturn;

	systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
	if (sys != nullptr)
	{
		waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
		wgpf->cleanupMemory();
	}

	//delete node set node children, and clear their list
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator setIterator;
	for (setIterator = nodeSets.begin(); setIterator != nodeSets.end(); ++setIterator)
	{
		componentHandle<waypointNodeSet> set = (*setIterator)->getComponent<waypointNodeSet>();

		set->nodes.clear();
	}

	//clear waypointNodes
	GameObjectFiltering::componentTypeSet node_pattern;
	node_pattern.setType(waypointNode::getType());
	std::vector<std::shared_ptr<gameObject>> nodes = getSystemManager()->getGameObjectGatherer()->getList(node_pattern);

	std::vector<std::shared_ptr<gameObject>>::iterator nodeIterator;
	for (nodeIterator = nodes.begin(); nodeIterator != nodes.end(); ++nodeIterator)
	{
		toReturn.push_back((*nodeIterator)->getID());
		//(*nodeIterator)->markedForDeletion();
	}

	return toReturn;
}

void dynamicWaypointGraph::dumpMemory()
{
	validConnections.clear();
	invalidConnections.clear();

	objectSets.clear();
	nodeOwnerList.clear();
}

void dynamicWaypointGraph::deleteRandomNodeSet()
{
	GameObjectFiltering::componentTypeSet nodeSet_pattern;
	nodeSet_pattern.setType(waypointNodeSet::getType());
	std::vector<std::shared_ptr<gameObject>> nodeSets = getSystemManager()->getGameObjectGatherer()->getList(nodeSet_pattern);

	std::random_device generator;
	int rand = generator() % nodeSets.size();

	componentHandle<waypointNodeSet> toDelete = nodeSets[rand]->getComponent<waypointNodeSet>();

	handleNodeSetDeletion(toDelete);
}

void dynamicWaypointGraph::testFunction()
{
	GameObjectFiltering::componentTypeSet testScriptPattern;
	testScriptPattern.setType(testingScript::getType());
	std::vector<std::shared_ptr<gameObject>> objects = getSystemManager()->getGameObjectGatherer()->getList(testScriptPattern);

	if (objects.size() < 2)
	{
		return;
	}
	else
	{
		systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
		if (sys != nullptr)
		{
			waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
			vector3 pos1 = objects[0]->getComponent<transform>()->getGlobalPosition();
			vector3 pos2 = objects[1]->getComponent<transform>()->getGlobalPosition();
			std::shared_ptr<graphPath> path = wgpf->getPath(pos1, pos2);
			std::list<vector3> resultingPath = path->points;

			componentHandle<debugLines> pathLines = debugLines::getLinesByTag(objects[0], PATH_LINE_TAG);
			std::vector<lineSegment> & lineBuffer = pathLines->getLineBuffer();
			lineBuffer.clear();

			if (path->isBad())
				return;

			vector3 color(1, 0, 1);
			float weight(.1f);

			lineBuffer.push_back(lineSegment(objects[0]->getComponent<transform>()->getGlobalPosition(), resultingPath.front(), color, weight));
			std::list<vector3>::iterator it = resultingPath.begin();
			std::list<vector3>::iterator previous = it;
			while (++it != resultingPath.end())
			{
				lineBuffer.push_back(lineSegment(*it, *previous, color, weight));
				++previous;
			}

			wgpf->cleanupMemory();
		}
	}
}

//--------------------
//NON-EDITOR FUNCTIONS
//--------------------


void dynamicWaypointGraph::deleteNode(componentHandle<waypointNode> node)
{
	//Remove all connections and debug draw lines
	std::vector<connectedNode> & connections = node->connections;

	//Clear debug lines on this node
	//componentHandle<debugLines> pathLines = debugLines::getLinesByTag(node->getGameObject().lock(), PATH_LINE_TAG);
	//std::vector<lineSegment> & lineBuffer = pathLines->getLineBuffer();
	//lineBuffer.clear();

	//Clear any debug lines to this from other nodes
	for (unsigned i = 0; i < connections.size(); ++i)
	{
		removeDebugLine(nodeConnection(connections[i].node, node));
		removeDebugLine(nodeConnection(node, connections[i].node));
	}
	//Remove connections to this on other nodes
	for (int i = connections.size() - 1; i >= 0; --i)
	{
		removeNodeFromNodesConnections(connections[i].node, node);
	}

	std::list<nodeConnection>::iterator it = validConnections.begin();
	while (it != validConnections.end())
	{
		if (it->firstNode == node || it->secondNode == node)
		{
			std::list<nodeConnection>::iterator toDelete = it;
			++it;
			validConnections.erase(toDelete);
		}
		else
			++it;
	}
	it = invalidConnections.begin();
	while (it != invalidConnections.end())
	{
		if (it->firstNode == node || it->secondNode == node)
		{
			std::list<nodeConnection>::iterator toDelete = it;
			++it;
			invalidConnections.erase(toDelete);
		}
		else
			++it;
	}

	//delete the object
	//space * s = node->getGameObject().lock()->getSpace();
	//s->destroyGameObject(node->getGameObject().lock());
	if (node->getGameObject().expired() == false)
		node->getGameObject().lock()->markForDeletion();
}

std::shared_ptr<space> dynamicWaypointGraph::getGraphSpace() const
{
	std::shared_ptr<space> graphSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->getSpace("WaypointGraph");
	if (graphSpace == nullptr)
		graphSpace = getSystemManager()->getSystem<sceneSystem>()->getCurrentScene().lock()->createSpace("WaypointGraph");

	return graphSpace;
}

std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> dynamicWaypointGraph::handlePathRequest(vector3 start, vector3 end, bool addEndVisual, bool addStartVisual)
{
	//Create a node at start and end points
	std::shared_ptr<space> graphSpace = getGraphSpace();

	std::shared_ptr<gameObject> startNode = makeNodeGameobject(&(*graphSpace), addStartVisual, vector3(.10f, 5.0f, .10f), "solidgreen.mtl");
	std::shared_ptr<gameObject> endNode = makeNodeGameobject(&(*graphSpace), addEndVisual, vector3(.10f, 5.0f, .10f), "solidgreen.mtl");
	startNode->setName("StartNode");
	endNode->setName("EndNode");

	startNode->getComponent<transform>()->setPosition(start);
	endNode->getComponent<transform>()->setPosition(end);

	componentHandle<waypointNode> startWN = startNode->getComponent<waypointNode>();
	componentHandle<waypointNode> endWN = endNode->getComponent<waypointNode>();
	startWN->debugName = "StartPath";
	endWN->debugName = "EndPath";

	addPath(startWN, endWN);

	return std::make_pair(startWN, endWN);
}

/*
attempt to connect start to end
if fail and intersect with other, attempt to connect start to ALL closest connection parameter nodes
then, recurse on every closest connection param nodes to end
if fail and intersect with self, skip
*/

void dynamicWaypointGraph::addPath(componentHandle<waypointNode> start, componentHandle<waypointNode> end)
{
	//Attempt to connect
	nodeConnection newConnection(start, end);

	std::list<intersection> intersections = findIntersections(newConnection);

	//Base case - no intersections
	if (intersections.size() == 0)
	{
		if (nodesAreConnected(start, end))
			return;
		connectNodes(start, end, false);
		validConnections.push_back(newConnection);
		addDebugLine(newConnection);
		return;
	}

	vector3 startNodePos = getNodePosition(start);
	nodeConnection closestCon = getClosestIntersection(startNodePos, intersections);


	//On self intersect, skip this
	if (nodesShareMergedObject(closestCon.firstNode, start))
	{
		return;
	}
	if (nodesShareMergedObject(closestCon.firstNode, end))
	{
		return;
	}

	std::list<componentHandle<waypointNode> > otherObjectNodes = getObjectParameterNodes(closestCon.firstNode);

	//connect each node in opposing set to start
	//then connect each node in opposing set to end
	for (std::list<componentHandle<waypointNode> >::iterator it = otherObjectNodes.begin(); it != otherObjectNodes.end(); ++it)
	{
		addPath(start, *it);
		addPath(*it, end);
	}
}

bool dynamicWaypointGraph::nodesShareMergedObject(componentHandle<waypointNode> node1, componentHandle<waypointNode> node2)
{
	componentHandle<waypointNodeSet> node1Owner;
	componentHandle<waypointNodeSet> node2Owner;

	bool n1Found = false;
	bool n2Found = false;

	//Get owner of nodes (if two exist, either one works)
	auto it = nodeOwnerList.begin();
	while (it != nodeOwnerList.end() && (!n1Found || !n2Found))
	{
		if (it->first == node1)
		{
			node1Owner = it->second.front();
			n1Found = true;
		}
		if (it->first == node2)
		{
			node2Owner = it->second.front();
			n2Found = true;
		}

		++it;
	}

	if (!n1Found || !n2Found)
	{
		return false;
	}

	std::string node1OwnerName = node1Owner->debugName;
	std::string node2OwnerName = node2Owner->debugName;

	//"Path" through objectSets using objectParameter owners and mergedParameters
	std::list<componentHandle<waypointNodeSet> > checked;
	std::queue<componentHandle<waypointNodeSet> > toCheck;
	toCheck.push(node1Owner);
	checked.push_back(node1Owner);

	while (toCheck.size() > 0)
	{
		componentHandle<waypointNodeSet> checking = toCheck.front();
		toCheck.pop();

		std::string checkName = checking->debugName;

		//Match found, they share a merged object
		if (checking == node2Owner)
			return true;

		//add list of merged owners to toCheck by finding my object set, and getting merged
		std::list<objectParameter>::iterator objectParameterIt = objectSets.begin();
		while (objectParameterIt != objectSets.end() && !(objectParameterIt->owner == checking))
		{
			std::string ositName = objectParameterIt->owner->debugName;
			++objectParameterIt;
		}

		if (objectParameterIt == objectSets.end())
		{
			//THIS SHOULD NEVER BE CALLED
			//DebugBreak();
			continue;
		}

		for (auto mergedIt = objectParameterIt->mergedParameters.begin(); mergedIt != objectParameterIt->mergedParameters.end(); ++mergedIt)
		{
			for (componentHandle<waypointNodeSet> & checkedIt : checked)
			{
				std::string checkedName = checkedIt->debugName;
				std::string mergedName = (*mergedIt)->debugName;
				if (!(checkedIt == *mergedIt))
				{
					checked.push_back(*mergedIt);
					toCheck.push(*mergedIt);
				}
			}
		}
	}

	return false;
}

std::list<componentHandle<waypointNode>> dynamicWaypointGraph::getObjectParameterNodes(componentHandle<waypointNode> node)
{
	std::list<componentHandle<waypointNode>> toReturn;

	std::queue<componentHandle<waypointNode>> toCheck;
	std::list<componentHandle<waypointNode>> seen;

	toCheck.push(node);
	seen.push_back(node);

	//Build list based on neighbors that share an object parameter
	while (toCheck.empty() == false)
	{
		componentHandle<waypointNode> current = toCheck.front();
		std::string debugName = current->debugName;
		toCheck.pop();
		toReturn.push_back(current);

		std::vector<connectedNode> & connections = current->connections;

		for (std::vector<connectedNode>::iterator it = connections.begin(); it != connections.end(); ++it)
		{
			//If doesn't share parameter, skip it
			if (it->sharesObjectParameter == false)
				continue;

			//If seen already, skip it
			std::list<componentHandle<waypointNode>>::iterator j = seen.begin();
			while (j != seen.end())
			{
				if (it->node == *j)
					break;

				++j;
			}

			if (j != seen.end())
				continue;

			std::string newName = it->node->debugName;

			//new connected neighbor found
			toCheck.push(it->node);
			seen.push_back(it->node);
		}
	}

	return toReturn;
}

void dynamicWaypointGraph::addIntersectNode(componentHandle<waypointNode> & iNode, vector3 & iPoint, float dot, parameterConnection & p1, componentHandle<waypointNodeSet> owningSet)
{
	auto trailer = p1.nodeList.begin();
	auto front = p1.nodeList.begin();
	while ((++front) != p1.nodeList.end())
	{
		vector3 fPos(getNodePosition(front->node));
		vector3 tPos(getNodePosition(trailer->node));
		if (pointIsOnSegment(vector2(iPoint.x, iPoint.z), vector2(fPos.x, fPos.z), vector2(tPos.x, tPos.z)))
		{
			//p1.nodeList.insert(front, (iNode, owningSet));

			nodeConnection oldConnection(trailer->node, front->node, true);
			deleteConnection(oldConnection);
			removeDebugLine(oldConnection);

			if (dot > 0) //I want to connect backwards
			{
				if (trailer->connectsForward == true) //If previous connects forwards
				{
					p1.nodeList.insert(front, parameterNode(iNode, owningSet, false));
					connectNodes(trailer->node, iNode, true);
					nodeConnection newCon = nodeConnection(trailer->node, iNode, true);
					addDebugLine(newCon);
					validConnections.push_back(newCon);
				}
				else
				{
					p1.nodeList.insert(front, parameterNode(iNode, owningSet, false));
				}
			}
			else //I want to connect forwards
			{
				if (front->connectsForward == false) //If next connects backwards
				{
					p1.nodeList.insert(front, parameterNode(iNode, owningSet, true));
					connectNodes(iNode, front->node, true);
					nodeConnection newCon = nodeConnection(iNode, front->node, true);
					addDebugLine(newCon);
					validConnections.push_back(newCon);
				}
				else
				{
					p1.nodeList.insert(front, parameterNode(iNode, owningSet, true));
				}
			}
			break;
		}
		++trailer;
	}
}

void dynamicWaypointGraph::mergeParameterNodeOwners(objectParameter & op1, objectParameter & op2)
{
	//if already merged, skip
	for (componentHandle<waypointNodeSet> merged : op1.mergedParameters)
	{
		if (merged == op2.owner)
			return;
	}

	std::string debug1 = op1.owner->debugName;
	std::string debug2 = op2.owner->debugName;

	//Set as merged
	op1.mergedParameters.push_back(op2.owner);
	op2.mergedParameters.push_back(op1.owner);
}

void dynamicWaypointGraph::checkObjObjOverlap(componentHandle<waypointNodeSet> newSet)
{
	objectParameter newParameter;
	newParameter.owner = newSet;
	newParameter.debugName = newSet->debugName;

	std::list<nodeConnection> conList = getConnections(newSet);
	for (auto c : conList)
	{
		newParameter.baseConnections.push_back(c);
		newParameter.parameterConnections.push_back(parameterConnection(c.firstNode, c.secondNode));
	}

	//Check for intersections with all other object sets
	for (std::list<objectParameter>::iterator other = objectSets.begin(); other != objectSets.end(); ++other)
	{
		for (unsigned oIndex = 0; oIndex < other->baseConnections.size(); ++oIndex)
		{
			for (unsigned myIndex = 0; myIndex < newParameter.baseConnections.size(); ++myIndex)
			{
				//If intersection found
				std::pair<bool, vector3> intersect = doConnectionsIntersect(other->baseConnections[oIndex], newParameter.baseConnections[myIndex]);
				if (intersect.first == true)
				{
					vector3 otherDir = getConnectionVector(other->baseConnections[oIndex]);
					vector3 myDir = getConnectionVector(newParameter.baseConnections[myIndex]);

					//rotate new connection 90degrees
					vector2 rotatedMe = vector2(-myDir.z, myDir.x);
					float dot = rotatedMe.dotP(vector2(otherDir.x, otherDir.z));

					//Tell each node of each set they are now shared
					mergeParameterNodeOwners(*other, newParameter);

					//Create Inode
					std::shared_ptr<gameObject> iNode = makeNodeGameobject(&(*getGraphSpace()), false);// , vector3(.25f, .25f, .25f), "darkgrey.mtl");
					componentHandle<waypointNode> iNodeHandle = iNode->getComponent<waypointNode>();
					iNode->getComponent<transform>()->setPosition(intersect.second);
					std::list<componentHandle<waypointNodeSet> > owners;
					owners.push_back(newSet);
					owners.push_back(other->owner);
					//Add iNode to nodeOwnerList
					nodeOwnerList.push_back(std::make_pair(iNodeHandle, owners));

					//Insert into both connections at proper point
					addIntersectNode(iNodeHandle, intersect.second, dot, newParameter.parameterConnections[myIndex], other->owner);
					addIntersectNode(iNodeHandle, intersect.second, -dot, other->parameterConnections[oIndex], newSet);
				}
			}
		}
	}

	//Add to list of object sets
	objectSets.push_back(newParameter);
}

void dynamicWaypointGraph::handleNodeSetCreation(componentHandle<waypointNodeSet> newSet)
{
	checkObjObjOverlap(newSet);

	//Find new intersections for valid paths
	//Foreach valid path
	//Check if new connection in the set of nodes intersects with valid connection
	std::list<intersection> newIntersections = findIntersections(newSet);

	std::string setDebugName = newSet->debugName;

	//If no intersection found, skip it:
	if (newIntersections.size() == 0)
		return;

	//If intersection found:
	std::list<intersection>::iterator it;

	for (it = newIntersections.begin(); it != newIntersections.end(); ++it)
	{
		//Case: 1 open, 1 object
		//Non object path must stitch around the object
		if (it->oldConnection.onObjectParameter == false)
		{
			nodeConnection & openConnection = (it->oldConnection.onObjectParameter == false) ? it->oldConnection : it->newConnection;
			nodeConnection & objectConnection = (it->oldConnection.onObjectParameter == true) ? it->oldConnection : it->newConnection;

			//Convert vector3 into vector 2 for line intersections
			vector3 objectVector3 = getConnectionVector(objectConnection);
			vector3 openVector3 = getConnectionVector(openConnection);

			//Rotate object connection that is intersecting -90 degrees (clockwise 90 degrees)
			vector2 objectVector = vector2(objectVector3.z, -objectVector3.x);
			vector2 openVector = vector2(openVector3.x, openVector3.z);

			/* NOTE: It is assumed that node connections surround an object clockwise,
			 * thus -90 degree rotation will point towards the object that causes collision */

			//Dot old connection and newly rotated connection
			float dot = objectVector.dotP(openVector);

			//If <0, possible connection with second -> first from open path
			if (dot < 0)
			{
				//Connect second open path node to new object nodes
				nodeConnection newConnectionOne = connectNodesWithConnection(openConnection.secondNode, objectConnection.firstNode, false);
				nodeConnection newConnectionTwo = connectNodesWithConnection(openConnection.secondNode, objectConnection.secondNode, false);

				//Validate new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}
			//If >= 0, possible connection with first -> second from open path
			else
			{
				//Connect first open node to new object nodes
				nodeConnection newConnectionOne = connectNodesWithConnection(openConnection.firstNode, objectConnection.firstNode, false);
				nodeConnection newConnectionTwo = connectNodesWithConnection(openConnection.firstNode, objectConnection.secondNode, false);

				//Valide new paths
				validateConnection(newConnectionOne);
				validateConnection(newConnectionTwo);
			}

			invalidateConnection(openConnection);

			removeDebugLine(openConnection);
		}
	}
}

//For each shared iNode in removeFrom, if the other owner is toRemove.owner, remove the iNode and stitch self
void dynamicWaypointGraph::unMergeObjects(objectParameter & removeFrom, objectParameter & toRemove)
{
	std::list<componentHandle<waypointNode> > sharedINodes;
	//foreach parameter connection
	for (parameterConnection & myPC : removeFrom.parameterConnections)
	{
		//foreach shared node
		auto it = myPC.nodeList.begin();
		auto previous = myPC.nodeList.begin();
		for (++it; it != --(myPC.nodeList.end()); /*do nothing*/) //First and last nodes are not shared, guaranteed
		{
			//If the shared node matches the toRemove object
			if (it->otherOwner == toRemove.owner)
			{
				auto toDelete = it;
				++it; //it is now "next"

				//If toDelete is connected either way, remove the connection and invalidate it
				if (toDelete->connectsForward == false && previous->connectsForward == true)
				{
					nodeConnection conToDelete = nodeConnection(previous->node, toDelete->node, true);
					deleteConnection(conToDelete);
					removeDebugLine(conToDelete);
				}
				else if (toDelete->connectsForward == true && it->connectsForward == false)
				{
					nodeConnection conToDelete = nodeConnection(toDelete->node, it->node, true);
					deleteConnection(conToDelete);
					removeDebugLine(conToDelete);
				}

				myPC.nodeList.erase(toDelete);

				//if previous and it want to connect to each other, connect
				if (previous->connectsForward && it->connectsForward == false)
				{
					connectNodes(previous->node, it->node, true);
					nodeConnection newCon = nodeConnection(previous->node, it->node, true);
					addDebugLine(newCon);
					validConnections.push_back(newCon);
				}
			}
			else
			{
				++it;
				++previous;
			}
		}
	}

	//remove from mergedParameter list
	for (std::list<componentHandle<waypointNodeSet> >::iterator it = removeFrom.mergedParameters.begin(); it != removeFrom.mergedParameters.end(); ++it)
	{
		if (*it == toRemove.owner)
		{
			removeFrom.mergedParameters.erase(it);
			return;
		}
	}
}

void dynamicWaypointGraph::handleNodeSetDeletion(componentHandle<waypointNodeSet> set)
{
	//Get my object parameter
	std::list<objectParameter>::iterator myObjectParameter = objectSets.begin();
	while (myObjectParameter != objectSets.end() && (myObjectParameter->owner == set) == false)
		++myObjectParameter;

	//Error checking - needed only in editor
	if (myObjectParameter == objectSets.end())
		return;

	std::string debugName = set->debugName;
	std::string actualDebugName = myObjectParameter->owner->debugName;

	//For each merged parameter, tell it we are no longer merged
	for (componentHandle<waypointNodeSet> & mergedSet : myObjectParameter->mergedParameters)
	{
		std::string debugMergedName = mergedSet->debugName;
		//Find the objectParameter we are removing in the op list
		std::list<objectParameter>::iterator opIt = objectSets.begin();
		while (!(opIt->owner == mergedSet))
			++opIt;

		unMergeObjects(*opIt, *myObjectParameter);
	}

	//Get my nodes
	std::list<componentHandle<waypointNode> > myNodes;
	for (parameterConnection & pc : myObjectParameter->parameterConnections)
	{
		//Skip begin and end. Those nodes are shared between 2 parameter connections
		for (std::list<parameterNode>::iterator it = (++pc.nodeList.begin()); it != (--pc.nodeList.end()); ++it)
		{
			myNodes.push_back(it->node);
		}
	}
	//Get the skipped nodes
	for (componentHandle<waypointNode> & n : set->nodes)
		myNodes.push_back(n);

	std::list<nodeConnection> connectionsToDelete;
	for (componentHandle<waypointNode> myNode : myNodes)
	{
		for (connectedNode conNode : myNode->connections)
		{
			connectionsToDelete.push_back(nodeConnection(myNode, conNode.node));
		}
	}

	for (nodeConnection & nc : connectionsToDelete)
	{
		deleteConnection(nc);
		removeDebugLine(nc);
	}

	for (componentHandle<waypointNode> & n : myNodes)
	{
		bool found = false;
		for (auto it = nodeOwnerList.begin(); (!found) && it != nodeOwnerList.end(); ++it)
		{
			if (it->first == n)
			{
				it = nodeOwnerList.erase(it);
				if (it == nodeOwnerList.end())
					break;
				found = true;
				continue;
			}
		}

		//space * s = n->getGameObject().lock()->getSpace();
		//s->destroyGameObject(n->getGameObject().lock());
		if (n->getGameObject().expired() == false)
			n->getGameObject().lock()->markForDeletion();
	}

	objectSets.erase(myObjectParameter);

	//space * s = set->getGameObject().lock()->getSpace();
	//s->destroyGameObject(set->getGameObject().lock());
	if (set->getGameObject().expired() == false)
		set->getGameObject().lock()->markForDeletion();
}

//======== Helper Functions ============================================================//
void dynamicWaypointGraph::createNodeSet(componentHandle<waypointNodeSet> set)
{
	//Make new node
	std::shared_ptr<gameObject> setObject = set->getGameObject().lock();
	std::shared_ptr<gameObject> node1 = makeNodeGameobject(&(*getGraphSpace()), false, vector3(.25f, .25f, .25f));

	//Prefab made here
	std::shared_ptr<gameObject> node2 = node1->cloneObject();
	std::shared_ptr<gameObject> node3 = node1->cloneObject();
	std::shared_ptr<gameObject> node4 = node1->cloneObject();

	//Set node positions
	vector3 offset = setObject->getComponent<transform>()->getPosition();
	vector3 forward = setObject->getComponent<transform>()->getForward();
	vector3 right = setObject->getComponent<transform>()->getRight();

	std::list<componentHandle<waypointNodeSet> > ownerList; //For adding to nodeOwnerList
	ownerList.push_back(set);

	//noe 1 pos & debugName
	vector3 nodePos = offset + set->nodeOne.x * right + set->nodeOne.z * forward;
	node1->getComponent<transform>()->setPosition(nodePos);
	node1->setName("Node Set Point One");
	node1->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 1");
	nodeOwnerList.push_back(std::make_pair(node1->getComponent<waypointNode>(), ownerList));

	//node 2
	nodePos = offset + set->nodeTwo.x * right + set->nodeTwo.z * forward;
	node2->getComponent<transform>()->setPosition(nodePos);
	node2->setName("Node Set Point Two");
	node2->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 2");
	nodeOwnerList.push_back(std::make_pair(node2->getComponent<waypointNode>(), ownerList));

	//node 3
	nodePos = offset + set->nodeThree.x * right + set->nodeThree.z * forward;
	node3->getComponent<transform>()->setPosition(nodePos);
	node3->setName("Node Set Point Three");
	node3->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 3");
	nodeOwnerList.push_back(std::make_pair(node3->getComponent<waypointNode>(), ownerList));

	//node 4
	nodePos = offset + set->nodeFour.x * right + set->nodeFour.z * forward;
	node4->getComponent<transform>()->setPosition(nodePos);
	node4->setName("Node Set Point Four");
	node4->getComponent<waypointNode>()->debugName = std::string(set->debugName + " 4");
	nodeOwnerList.push_back(std::make_pair(node4->getComponent<waypointNode>(), ownerList));

	//Get node pointers
	componentHandle<waypointNode> nodeOneComponent = node1->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeTwoComponent = node2->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeThreeComponent = node3->getComponent<waypointNode>();
	componentHandle<waypointNode> nodeFourComponent = node4->getComponent<waypointNode>();

	//Add to node set list
	set->nodes.push_back(nodeOneComponent);
	set->nodes.push_back(nodeTwoComponent);
	set->nodes.push_back(nodeThreeComponent);
	set->nodes.push_back(nodeFourComponent);

	//Connect nodes, add debug lines
	connectNodeSet(set);
}

std::shared_ptr<gameObject> dynamicWaypointGraph::makeNodeGameobject(space * s, bool addNodeVisual, vector3 scale, std::string materialName) const
{
	std::shared_ptr<gameObject> toReturn = s->instantiateGameObject("Waypoint Graph Node");
	if (addNodeVisual)
	{
		componentHandle<renderer> l_renderer_comp = toReturn->addComponent<renderer>();
		l_renderer_comp->setPrimitiveType(CUBE_FILE);
		l_renderer_comp->setProgramType(programType::enm_forward);
		toReturn->getComponent<renderer>()->setMaterialName(materialName);
		toReturn->getComponent<transform>()->setScale(scale);
	}
	toReturn->addComponent<waypointNode>();

	return toReturn;
}

bool dynamicWaypointGraph::nodesAreConnected(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	for (size_t i = 0; i < n1->connections.size(); ++i)
	{
		if (n1->connections[i].node == n2)
			return true;
	}
	return false;
}

void dynamicWaypointGraph::validateConnection(nodeConnection & toValidate)
{
	//Check new connection against all valid connections
	std::list<intersection> intersections = findIntersections(toValidate);

	//No intersections, all good
	if (intersections.size() == 0)
	{
		validConnections.push_back(toValidate);
		addDebugLine(toValidate, { 0,1,1 });
		return;
	}
	//Edge case when a path hits only one wall
	//Try to connect that wall to outside node instead
	if (intersections.size() == 1)
	{
		vector3 otherWallDir = getConnectionVector(intersections.front().oldConnection);
		vector3 myDir = getConnectionVector(toValidate);
		vector2 rotatedOther = vector2(-otherWallDir.z, otherWallDir.x);
		float dot = rotatedOther.dotP(vector2(myDir.x, myDir.z));
		if (dot >= 0)
		{
			attemptConnectionAndValidation(toValidate.secondNode, intersections.front().oldConnection.firstNode);
			attemptConnectionAndValidation(toValidate.secondNode, intersections.front().oldConnection.secondNode);
		}
		else
		{
			attemptConnectionAndValidation(toValidate.firstNode, intersections.front().oldConnection.firstNode);
			attemptConnectionAndValidation(toValidate.firstNode, intersections.front().oldConnection.secondNode);
		}
		return;
	}

	//If intersection found, create new connection(s) with the closest hit for each node
	bool intersectFound = false;
	float closestDist = std::numeric_limits<float>().max();
	float farthestDist = -1;

	nodeConnection closest = intersections.front().oldConnection;
	nodeConnection farthest = intersections.front().oldConnection;

	//Compare intersection points to this node
	vector3 firstNodePosition = getNodePosition(toValidate.firstNode);

	//Loop through intersections, keeping track of farthest and closest object path intersections only
	std::list<intersection>::iterator it;
	for (it = intersections.begin(); it != intersections.end(); ++it)
	{
		//Ignore intersections with open paths
		if (it->oldConnection.onObjectParameter == false)
			continue;

		intersectFound = true;
		float dist = (it->intersectPoint - firstNodePosition).distanceSquared();
		if (dist > farthestDist)
		{
			farthestDist = dist;
			farthest = it->oldConnection;
		}
		if (dist < closestDist)
		{
			closestDist = dist;
			closest = it->oldConnection;
		}
	}

	//Recursive call validate on new connections
	if (intersectFound)
	{
		invalidateConnection(toValidate);
		removeDebugLine(toValidate);

		attemptConnectionAndValidation(toValidate.firstNode, closest.firstNode);
		attemptConnectionAndValidation(toValidate.firstNode, closest.secondNode);

		attemptConnectionAndValidation(toValidate.secondNode, farthest.firstNode);
		attemptConnectionAndValidation(toValidate.secondNode, farthest.secondNode);
	}
}

void dynamicWaypointGraph::attemptConnectionAndValidation(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	if (nodesAreConnected(n1, n2))
		return;

	nodeConnection newConnection = connectNodesWithConnection(n1, n2, false);
	validateConnection(newConnection);
}

//Returns all connections, not just the closest one
std::list<intersection> dynamicWaypointGraph::findIntersections(componentHandle<waypointNodeSet> newSet) const
{
	std::list<intersection> toReturn;

	//For each valid connection, check for intersection with new set
	std::list<nodeConnection> setConnections = getConnections(newSet);

	std::list<nodeConnection>::iterator i;
	for (i = setConnections.begin(); i != setConnections.end(); ++i)
	{
		//Find all intersections and merge them into toReturn
		std::list<intersection> newIntersections = findIntersections(*i);
		for (std::list<intersection>::iterator j = newIntersections.begin(); j != newIntersections.end(); ++j)
		{
			toReturn.push_back(*j);
		}
	}

	return toReturn;
}

//Returns all connections, not just the closest one
std::list<intersection> dynamicWaypointGraph::findIntersections(nodeConnection connection) const
{
	std::list<intersection> toReturn;

	std::list<nodeConnection>::const_iterator j;
	for (j = validConnections.begin(); j != validConnections.end(); ++j)
	{
		if (connection.onObjectParameter == false && j->onObjectParameter == false)
			continue;

		//If intersection found, add to list
		std::pair<bool, vector3> intersect = doConnectionsIntersect(connection, *j);
		if (intersect.first)
			toReturn.push_back(intersection(*j, connection, intersect.second));
	}

	return toReturn;
}

std::list<nodeConnection> dynamicWaypointGraph::getConnections(componentHandle<waypointNodeSet> set) const
{
	std::list<nodeConnection> toReturn;

	for (size_t i = 0; i < set->nodes.size() - 1; ++i)
	{
		toReturn.push_back(nodeConnection(set->nodes[i], set->nodes[i + 1], true));
	}
	toReturn.push_back(nodeConnection(set->nodes[set->nodes.size() - 1], set->nodes[0], true));

	return toReturn;
}

vector3 dynamicWaypointGraph::getConnectionVector(const nodeConnection & c) const
{
	vector3 firstPos = getNodePosition(c.firstNode);
	vector3 secondPos = getNodePosition(c.secondNode);

	return (secondPos - firstPos);
}

vector3 dynamicWaypointGraph::getNodePosition(componentHandle<waypointNode> n) const
{
	return n->getGameObject().lock()->getComponent<transform>()->getGlobalPosition();
}

nodeConnection dynamicWaypointGraph::connectNodesWithConnection(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool isObjectPath)
{
	nodeConnection newConnection = nodeConnection(a, b, isObjectPath);
	connectNodes(a, b, isObjectPath);
	return newConnection;
}

void dynamicWaypointGraph::connectNodes(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool sharesParameter)
{
	std::string namea = a->debugName;
	std::string nameb = b->debugName;
	if (nodesAreConnected(a, b))
		return;

	vector3 diffVector = getNodePosition(a) - getNodePosition(b);
	float cost = diffVector.distance();

	a->connections.push_back(connectedNode(b, cost, sharesParameter));
	b->connections.push_back(connectedNode(a, cost, sharesParameter));
}

void dynamicWaypointGraph::invalidateConnection(nodeConnection & nc)
{
	//If it was valid, erase it
	std::list<nodeConnection>::iterator it;
	for (it = validConnections.begin(); it != validConnections.end(); ++it)
	{
		if (nc == *it)
		{
			systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
			if (sys != nullptr)
			{
				waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
				wgpf->nodeConnectionInvalidated(nc);
			}
			invalidConnections.push_back(nc);
			validConnections.erase(it);
			break;
		}
	}

	std::vector<connectedNode>::iterator conWalker;

	//clear connections between nodes
	removeNodeFromNodesConnections(nc.firstNode, nc.secondNode);
	removeNodeFromNodesConnections(nc.secondNode, nc.firstNode);
}

void dynamicWaypointGraph::connectNodeSet(componentHandle<waypointNodeSet> nodeSet)
{
	std::vector<componentHandle<waypointNode>>::iterator it;
	for (it = nodeSet->nodes.begin(); it != nodeSet->nodes.end(); ++it)
	{
		std::vector<componentHandle<waypointNode>>::iterator it2 = it;
		++it2;

		if (it2 == nodeSet->nodes.end())
			it2 = nodeSet->nodes.begin();

		nodeConnection newConnection = connectNodesWithConnection(*it, *it2, true);
		validConnections.push_back(newConnection);

	    addDebugLine(newConnection, vector3(1, 0, 0));
	}
}

void dynamicWaypointGraph::deleteConnection(const nodeConnection & c)
{
	validConnections.remove(c);
	invalidConnections.remove(c);

	//Tell the nodes they are no longer connected
	removeNodeFromNodesConnections(c.firstNode, c.secondNode);
	removeNodeFromNodesConnections(c.secondNode, c.firstNode);

	systemBase * sys = getSystemManager()->getSystem("waypointGraphPathfindSystem");
	if (sys != nullptr)
	{
		waypointGraphPathfinder * wgpf = dynamic_cast<waypointGraphPathfinder *>(sys);
		wgpf->cleanupMemory();
	}
}

void dynamicWaypointGraph::removeNodeFromNodesConnections(componentHandle<waypointNode> toRemoveFrom, componentHandle<waypointNode> toRemove) const
{
	std::vector<connectedNode>::iterator it;
	for (it = toRemoveFrom->connections.begin(); it != toRemoveFrom->connections.end(); ++it)
	{
		if (it->node == toRemove)
		{
			toRemoveFrom->connections.erase(it);
			break;
		}
	}
}

nodeConnection dynamicWaypointGraph::getClosestIntersection(vector3 position, std::list<intersection> intersections)
{
	float closest = std::numeric_limits<float>::max();
	nodeConnection closestCon = intersections.front().oldConnection;

	for (std::list<intersection>::iterator it = intersections.begin(); it != intersections.end(); ++it)
	{
		float newDist = (it->intersectPoint - position).distanceSquared();
		if (newDist < closest)
		{
			closest = newDist;
			closestCon = it->oldConnection;
		}
	}

	return closestCon;
}

void dynamicWaypointGraph::removeDebugLine(nodeConnection nc)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(nc.firstNode->getGameObject().lock(), DEBUG_LINE_TAG);
	std::vector<lineSegment> & lines = pathLines->getLineBuffer();

	for (std::vector<lineSegment>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		if (it->end == getNodePosition(nc.secondNode))
		{
			lines.erase(it);
			return;
		}
	}
	return;
}

void dynamicWaypointGraph::removeDebugLine(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(n1->getGameObject().lock(), DEBUG_LINE_TAG);
	std::vector<lineSegment> & lines = pathLines->getLineBuffer();

	for (std::vector<lineSegment>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		if (it->end == getNodePosition(n2))
		{
			lines.erase(it);
			return;
		}
	}

	//Wasn't found, must be on second node
	pathLines = debugLines::getLinesByTag(n2->getGameObject().lock(), DEBUG_LINE_TAG);
	lines = pathLines->getLineBuffer();

	for (std::vector<lineSegment>::iterator it = lines.begin(); it != lines.end(); ++it)
	{
		if (it->end == getNodePosition(n1))
		{
			lines.erase(it);
			return;
		}
	}
}

//Adds debug line to first node in connection
void dynamicWaypointGraph::addDebugLine(nodeConnection nc, vector3 color)
{
	componentHandle<debugLines> pathLines = debugLines::getLinesByTag(nc.firstNode->getGameObject().lock(), DEBUG_LINE_TAG);
	pathLines->getLineBuffer().push_back(lineSegment(getNodePosition(nc.firstNode), getNodePosition(nc.secondNode), color));
}

//========MATH FUNCTIONS=======================================================//

//Line intersections:
//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
std::pair<bool, vector3> dynamicWaypointGraph::doConnectionsIntersect(nodeConnection first, nodeConnection second) const
{
	//If paths share a common node, ignore it
	if ((first.firstNode == second.firstNode) ||
		(first.firstNode == second.secondNode) ||
		(first.secondNode == second.firstNode) ||
		(first.secondNode == second.secondNode))
		return std::make_pair<bool, vector3>(false, vector3(0, 0, 0));

	//Given two lines A-B and C-D,
	//If the orientation of ABC and ABD are not equal && CDA and CDB are not equal
	//then no intersection

	std::vector<std::string> debugNames = { first.firstNode->debugName,
		first.secondNode->debugName,
		second.firstNode->debugName,
		second.secondNode->debugName
	};

	//Get positions of all nodes
	vector3 pointA = getNodePosition(first.firstNode);
	vector3 pointB = getNodePosition(first.secondNode);
	vector3 pointC = getNodePosition(second.firstNode);
	vector3 pointD = getNodePosition(second.secondNode);

	int ABCOrientation = getLineOrientation(pointA, pointB, pointC);
	int ABDOrientation = getLineOrientation(pointA, pointB, pointD);
	int CDAOrientation = getLineOrientation(pointC, pointD, pointA);
	int CDBOrientation = getLineOrientation(pointC, pointD, pointB);

	//Definite intersect
	if (ABCOrientation != ABDOrientation && CDAOrientation != CDBOrientation)
	{
		vector2 intersectPoint = lineIntersectionPoint(vector2(pointA.x, pointA.z),
			vector2(pointB.x, pointB.z),
			vector2(pointC.x, pointC.z),
			vector2(pointD.x, pointD.z));

		return std::make_pair(true, vector3(intersectPoint.x, 0, intersectPoint.y));
	}

	//Special cases: colinear
	//Lines are colinear if a point on one line lies between the two points on the other line
	//In case of ABC, determine if C is between A and B
	if (ABCOrientation == 0 &&
		pointIsOnSegment(vector2(pointC.x, pointC.z), vector2(pointA.x, pointA.z), vector2(pointB.x, pointB.z)))
	{
		return std::make_pair(true, pointC);
	}
	if (ABDOrientation == 0 &&
		pointIsOnSegment(vector2(pointD.x, pointD.z), vector2(pointA.x, pointA.z), vector2(pointB.x, pointB.z)))
	{
		return std::make_pair(true, pointD);
	}
	if (CDAOrientation == 0 &&
		pointIsOnSegment(vector2(pointA.x, pointA.z), vector2(pointC.x, pointC.z), vector2(pointD.x, pointD.z)))
	{
		return std::make_pair(true, pointA);
	}
	if (CDBOrientation == 0 &&
		pointIsOnSegment(vector2(pointB.x, pointB.z), vector2(pointC.x, pointC.z), vector2(pointD.x, pointD.z)))
	{
		return std::make_pair(true, pointB);
	}

	return std::make_pair(false, vector3());
}

bool dynamicWaypointGraph::pointIsOnSegment(vector2 point, vector2 segmentPointA, vector2 segmentPointB) const
{
	float epsilon = .0001f; //Some float errors occur
	//X needs to be between the segment x's, and Y between segment y's
	if (point.x <= std::max(segmentPointA.x, segmentPointB.x) + epsilon &&
		point.x >= std::min(segmentPointA.x, segmentPointB.x) - epsilon &&
		point.y <= std::max(segmentPointA.y, segmentPointB.y) + epsilon &&
		point.y >= std::min(segmentPointA.y, segmentPointB.y) - epsilon)
		return true;

	return false;
}

//Line orientation:
//https://www.geeksforgeeks.org/orientation-3-ordered-points/
//THIS IS ACTUALLY 2D. REWRITE BEFORE MOVING TO MATH LIBRARY
int dynamicWaypointGraph::getLineOrientation(vector3 a, vector3 b, vector3 c) const
{
	float floatValue = (b.z - a.z) * (c.x - b.x) - (c.z - b.z) * (b.x - a.x);
	if (floatValue > 0)
		return 1;
	if (floatValue < 0)
		return -1;
	return 0;
}

//Line intersection point
//https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
vector2 dynamicWaypointGraph::lineIntersectionPoint(vector2 a, vector2 b, vector2 c, vector2 d) const
{
	float bottom = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);

	//LINES COLINEAR
	if (bottom == 0)
		return vector2(0, 0);

	float x = ((a.x * b.y - a.y * b.x) * (c.x - d.x) - (a.x - b.x) * (c.x * d.y - c.y * d.x)) / bottom;
	float y = ((a.x * b.y - a.y * b.x) * (c.y - d.y) - (a.y - b.y) * (c.x * d.y - c.y * d.x)) / bottom;

	return vector2(x, y);
}
