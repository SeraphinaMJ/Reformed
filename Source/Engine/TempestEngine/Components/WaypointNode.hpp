/*!***************************************************************************************
\file       WaypointNode.hpp
\author     Alex Kudebeh
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Component for the waypoint node in the graph pathfinding system
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "Component.hpp"
#include "../EngineInterfaces/Public/IComponentRepository.hpp"
//======== 3rd Party Includes ==========================================================//
#include <string>
//======== Forward Declarations=========================================================//
class nodeSet;
class waypointNode;
class waypointNodeSet;

//======== Types =======================================================================//
struct connectedNode
{
	connectedNode(componentHandle<waypointNode> node_, float cost_, bool sharesObjectParameter_);

	//!<The node itself
	componentHandle<waypointNode> node;
	//!<The distance from the node to the connected node
	float cost;
	//!<If the node is on the same object parameter as this one
	bool sharesObjectParameter;
};

//======== Defines =====================================================================//

/*!***************************************************************************************
\par class: waypointNode
\brief   The waypointNode class is used with the dynamic waypoint graph pathfinding system
*****************************************************************************************/
class waypointNode : public componentCRTP<waypointNode>
{
	public:
		//!<Readable name for debugging
		std::string debugName = "Waypoint Node";

		//!<The connections to other nodes - The nodes, and the "cost"
		std::vector<connectedNode> connections;

		/*!***************************************************************************************
		\brief  componentType getter
		\return The type of component this is
		*****************************************************************************************/
		componentType const type() const { static componentType const s_type = componentType::forConcrete<waypointNode>(); return s_type; }

		/*!***************************************************************************************
		\brief  static componentType getter
		\return The type of component this is
		*****************************************************************************************/
		static componentType const getType() { static componentType const s_type = componentType::forAbstract<waypointNode>(); return s_type; }

		/*!***************************************************************************************
		\brief  static componentTypeDetails getter
		\return Getter for the type's details
		*****************************************************************************************/
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Waypoint Node", true, true, false); }

		void updateFromTypeRT(typeRT & p_type);
		typeRT toTypeRT() const;
};
// namespace <namespace name>

