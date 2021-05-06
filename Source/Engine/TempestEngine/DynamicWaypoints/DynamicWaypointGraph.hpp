/*!***************************************************************************************
\file       DynamicWaypointGraph.hpp
\author     Alex Kudebeh
\date       9/10/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Header for Dynamic Waypoint Graphs used for AI pathfinding
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../SystemBase.hpp"
#include "WaypointNode.hpp"
#include "WaypointNodeSet.hpp"
//======== 3rd Party Includes ==========================================================//
#include <list>
#include <vector>
#include <Vector3.hpp>
#include <queue>
//======== Types =======================================================================//
/*!***************************************************************************************
\par struct: nodeConnection
\brief   A connection to another node
*****************************************************************************************/
struct nodeConnection
{
	/*!***************************************************************************************
	\brief  constructor
	\param firstNode_ - The first node in the connection
	\param secondNode_ - The first node in the connection
	\param onObjectParameter_ - If the connection is on an object parameter
	\param isValid_ - If the path is being blocked or not
	*****************************************************************************************/
	nodeConnection(componentHandle<waypointNode> firstNode_, componentHandle<waypointNode> secondNode_, bool onObjectParameter_ = false);

	/*!***************************************************************************************
	\brief  Compares a node connection with another based only on if the connected nodes match
	\param other - The other connection to compare to
	\return True if the first and second nodes match, false otherwise
	*****************************************************************************************/
	bool operator==(nodeConnection other) const;

	//!<Name kept for debugging purposes
	std::string name;

	//!<First node in the connection
	componentHandle<waypointNode> firstNode;
	//!<Second node in the connection
	componentHandle<waypointNode> secondNode;
	//!<If this object is on an object parameter, or an open path
	bool onObjectParameter;
};

struct parameterNode
{
	parameterNode(componentHandle<waypointNode> node_, componentHandle<waypointNodeSet> otherOwner_, bool connectsForward_);

	componentHandle<waypointNode> node;
	componentHandle<waypointNodeSet> otherOwner;
	bool connectsForward;
};

/*!***************************************************************************************
\par struct: parameterConnection
\brief   A list of all nodes that form a parameter connection

	The node list is the list of all nodes (in order, clockwise) that form a single parameter
	connection. isConnected states whether the node connections are valid or not.
*****************************************************************************************/
struct parameterConnection
{
	/*!***************************************************************************************
	\brief  Constructor for parameter connection
	\param node1 - The first node
	\param node2 - The second node
	*****************************************************************************************/
	parameterConnection(componentHandle<waypointNode> node1, componentHandle<waypointNode> node2);

	//!List of nodes that form the parameter, and their "other" parent (if not null)
	std::list<parameterNode> nodeList;
};

/*!***************************************************************************************
\par struct: objectParameter
\brief   A highly detailed list of connections that form the parameter of an object

	Starting with the base connections, this keeps track of all intersection nodes
*****************************************************************************************/
struct objectParameter
{
	//!The owning waypointNodeSet
	componentHandle<waypointNodeSet> owner;
	//!The base connections that came with the set
	std::vector<nodeConnection> baseConnections;
	//!Details for every connection in the set
	std::vector<parameterConnection> parameterConnections;
	//!List of parameters already merged with
	std::list<componentHandle<waypointNodeSet> > mergedParameters;

	//!Debug name
	std::string debugName;
};
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
struct intersection;
struct vector2;
class transform;
class space;

/*!***************************************************************************************
\par class: DynamicWaypointGraph
\brief   This class acts as a container for waypoint nodes and path management
*****************************************************************************************/
class dynamicWaypointGraph : public systemBase
{
	public:
		/*!***************************************************************************************
		\brief  Name getter for this system
		\return The name of this system
		*****************************************************************************************/
		const std::string& name() const override { static const std::string n("dynamicWaypointGraphSystem"); return n; }

		/*!***************************************************************************************
		\brief  Static name getter for this system
		\return The name of this system
		*****************************************************************************************/
		static const std::string& getName() { static const std::string n("dynamicWaypointGraphSystem"); return n; }

		/*!***************************************************************************************
		\brief  Handles stitching the waypoint graph back together when a node set is deleted
		*****************************************************************************************/
		void handleNodeSetDeletion(componentHandle<waypointNodeSet> set);

		/*!***************************************************************************************
		\brief  Handles connecting a set of new nodes into the current graph
		*****************************************************************************************/
		void handleNodeSetCreation(componentHandle<waypointNodeSet> newSet);

		/*!***************************************************************************************
		\brief  Removes all connections to/from a given node, and deletes it from the game
		\param node - The node to remove
		*****************************************************************************************/
		void deleteNode(componentHandle<waypointNode> node);

		//TEST FUNCTIONS - will be rewritten/reworked later
		void createLevelPath();
		void createNodeSetPaths();
		void stitchPath();

		/*!***************************************************************************************
		\brief  Clears all memory. If in editor, returns vector of all game objects to delete
		\param inEditor - If this function is called from in editor
		*****************************************************************************************/
		std::vector<objID> clear(bool inEditor = true);
		void deleteRandomNodeSet();
		void testFunction();

	protected:

		void onLevelLoad(const levelLoadEvent & /*p_event*/) override;
		void onLevelUnload(const levelUnloadEvent & /*p_event*/) override;

	private:
		void dumpMemory();

		/*!***************************************************************************************
		\brief Given two waypoint nodes, stitches them into the waypoint graph
		\param start - The start waypoint node
		\param end - The end waypoint node
		*****************************************************************************************/
		void addPath(componentHandle<waypointNode> start, componentHandle<waypointNode> end);

		/*!***************************************************************************************
		\brief Given two positions, create nodes and stitch them into the graph
		\param start - The position of one node
		\param end - The position of the other node
		\return The two nodes created, to be used with pathfinding
		*****************************************************************************************/
		std::pair<componentHandle<waypointNode>, componentHandle<waypointNode>> handlePathRequest(vector3 start, vector3 end, bool addEndVisual, bool addStartVisual);

		/*!***************************************************************************************
		\brief  Compares a new connection with all other valid connections, creating new paths on intersection
				Adds the connection to valid or invalid connections based on results
		\param newConnection - The new connection to validate against all valid connections
		*****************************************************************************************/
		void validateConnection(nodeConnection & newConnection);

		/*!***************************************************************************************
		\brief  Gets a list of connections from a node set (no duplicates)
		\param set - The node set to get connections from
		\return The list of all unique connections
		*****************************************************************************************/
		std::list<nodeConnection> getConnections(componentHandle<waypointNodeSet> set) const;

		/*!***************************************************************************************
		\brief  Checks if two connections intersect
		\param first - The first connection to check
        \param second - The second connection to check
		\return If the two connections intersect or not, and the intersect position if true
		*****************************************************************************************/
        std::pair<bool, vector3> doConnectionsIntersect(nodeConnection first, nodeConnection second) const;

		/*!***************************************************************************************
		\brief  Helper function that attempts to connect two nodes and validate them
				If the nodes are already connected, returns immediately.
				Otherwise, calls validateConnection, which places them in the valid/invalid list
		\param n1 - The first node to connect
		\param n2 - The second node to connect
		*****************************************************************************************/
		void attemptConnectionAndValidation(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2);

		/*!***************************************************************************************
		\brief  Finds intersections of a given node set with open valid connections
		\param newSet - The node set to get intersections for
		\return The list of all intersections with the node set
		*****************************************************************************************/
		std::list<intersection> findIntersections(componentHandle<waypointNodeSet> newSet) const;

		/*!***************************************************************************************
		\brief  Finds intersections of a given node set with open valid connections
		\param connection - The connection to find intersections for
		\return The list of all intersections with the connection
		*****************************************************************************************/
		std::list<intersection> findIntersections(nodeConnection connection) const;

		/*!***************************************************************************************
		\brief  Gets the directional vector of a connection (first to second)
		\param c - The connection to get the direction of
		\return The direction vector for the connection
		*****************************************************************************************/
		vector3 getConnectionVector(const nodeConnection & c) const;

		/*!***************************************************************************************
		\brief  Gets the world space position of the node
		\param n - The node to get the position of
		\return The node's world space position
		*****************************************************************************************/
		vector3 getNodePosition(componentHandle<waypointNode> n) const;

		/*!***************************************************************************************
		\brief  Returns the orientation of three points. Used for line intersection tests
		\param a - The first point
		\param b - The second point
		\param c - The third point
		\return The orientation of the nodes (1 is clockwise, -1 counter clockwise, 0 colinear)
		*****************************************************************************************/
		int getLineOrientation(vector3 a, vector3 b, vector3 c) const;

		/*!***************************************************************************************
		\brief  Finds the intersection point of two lines
		\param a - Line 1, point 1
		\param b - Line 1, point 2
		\param c - Line 2, point 1
		\param d - Line 2, point 2
		\return The intersection point
		*****************************************************************************************/
		vector2 lineIntersectionPoint(vector2 a, vector2 b, vector2 c, vector2 d) const;

		/*!***************************************************************************************
		\brief  Determines if a point is on a given
		\param point - The point to check
		\param segmentPointA - The first point on a segment
		\param segmentPointB - The second point on the segment
		\return If the point lies on the segment or not
		*****************************************************************************************/
		bool pointIsOnSegment(vector2 point, vector2 segmentPointA, vector2 segmentPointB) const;

		/*!***************************************************************************************
		\brief  Creates a new connection between two nodes. Does NOT add it to valid connections
		\param a - The first node to connect
		\param b - The second node to connect
		\param isObjectPath - If this object is
		\return The new node connection
		*****************************************************************************************/
		nodeConnection connectNodesWithConnection(componentHandle<waypointNode> a, componentHandle<waypointNode> b,
												  bool isObjectPath = false);

		/*!***************************************************************************************
		\brief  Tells two nodes they are connected, without creating a nodeConnection
		\param a - The first node to connect
		\param b - The second node to connect
		\param sharesParameter - If the two objects are on the same parameter or not
		*****************************************************************************************/
		void connectNodes(componentHandle<waypointNode> a, componentHandle<waypointNode> b, bool sharesParameter);

		/*!***************************************************************************************
		\brief  Handles creation of a single waypoint node set
		\param set - The set to create
		*****************************************************************************************/
		void createNodeSet(componentHandle<waypointNodeSet> set);

		/*!***************************************************************************************
		\brief  Moves a node connection from valid to invalid
		\param nc - The node connection to invalidate
		*****************************************************************************************/
		void invalidateConnection(nodeConnection & nc);

		/*!***************************************************************************************
		\brief  Deletes a connection from the graph; valid or invalid.
				Also removes connection from nodes
		\param c - The connection to delete
		*****************************************************************************************/
		void deleteConnection(const nodeConnection & c);

		/*!***************************************************************************************
		\brief  Helper function to connect a waypoint set's nodes together
		\param nodeSet - The set to connect
		*****************************************************************************************/
		void connectNodeSet(componentHandle<waypointNodeSet> nodeSet);

		/*!***************************************************************************************
		\brief  Helper function to check if two nodes are connected
		\param n1 - The first node to check
		\param n2 - The second node to check
		\return True if they are already connected, false otherwise
		*****************************************************************************************/
		bool nodesAreConnected(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2);

		/*!***************************************************************************************
		\brief  Adds a debug line to the object of the first node in the connection
		\param nc - The node connection to add the debug line to
		\param color - The color of the line
		*****************************************************************************************/
		void addDebugLine(nodeConnection nc, vector3 color = {1,1,1});

		/*!***************************************************************************************
		\brief  Removes a debug line from the node connection
		\param nc - The connection to remove the debug line from
		*****************************************************************************************/
		void removeDebugLine(nodeConnection nc);

		/*!***************************************************************************************
		\brief  Removes a debug line from two given nodes - used when unsure which node the line is on
		\param n1 - The first node
		\param n2 - The second node
		*****************************************************************************************/
		void removeDebugLine(componentHandle<waypointNode> n1, componentHandle<waypointNode> n2);
		
		/*!***************************************************************************************
		\brief  Makes a node game object in the space given. Two exist because of how spaces are received
				Getting space from gameobject gives space *, getting space from scene gives shared_ptr
		\param s - The space of the node set to create the object
		\param c - The color of the node. Default gray
		\return Shared ptr to the new node
		*****************************************************************************************/
		std::shared_ptr<gameObject> makeNodeGameobject(space * s, bool addNodeVisual = false, vector3 scale = vector3(1, 1, 1), std::string materialName = "solidred.mtl") const;

		/*!***************************************************************************************
		\brief  Helper function to remove a node from another nodes connections list
		\param toRemoveFrom - The node to remove a connection from
		\param toRemove - The node to look for in toRemoveFrom's connections list
		*****************************************************************************************/
		void removeNodeFromNodesConnections(componentHandle<waypointNode> toRemoveFrom, componentHandle<waypointNode> toRemove) const;

		/*!***************************************************************************************
		\brief  Helper function to get the closest intersection to a point from a list of intersections
		\param position - The position to find the closest intersection to
		\param intersections - The intersections to search through
		\return The closest connection that was intersecting
		*****************************************************************************************/
		nodeConnection getClosestIntersection(vector3 position, std::list<intersection> intersections);

		/*!***************************************************************************************
		\brief  Helper function to get an object's parameter nodes (including merged node sets)
		\param node - A node that is from the object's parameter
		\return  List of all nodes in the object's parameter
		*****************************************************************************************/
		std::list<componentHandle<waypointNode>> getObjectParameterNodes(componentHandle<waypointNode> node);

		void unMergeObjects(objectParameter & removeFrom, objectParameter & toRemove);

		void addIntersectNode(componentHandle<waypointNode> & iNode, vector3 & iPoint, float dot, parameterConnection & p1, componentHandle<waypointNodeSet> owningSet);
		void checkObjObjOverlap(componentHandle<waypointNodeSet> newSet);
		void mergeParameterNodeOwners(objectParameter & op1, objectParameter & op2);

		std::shared_ptr<space> getGraphSpace() const;

		bool nodesShareMergedObject(componentHandle<waypointNode> node1, componentHandle<waypointNode> node2);

		//!List of valid connections for path finding
		std::list<nodeConnection> validConnections;
		//!List of currently invalid connections
		std::list<nodeConnection> invalidConnections;
		//!The list of object waypoint sets, used in handleNodeSetCreation
		std::list<objectParameter> objectSets;

		//!Ineffecient way of holding onto owners of nodes
		std::list<std::pair<componentHandle<waypointNode>, std::list<componentHandle<waypointNodeSet> > > > nodeOwnerList;

		friend class waypointGraphPathfinder;
};
