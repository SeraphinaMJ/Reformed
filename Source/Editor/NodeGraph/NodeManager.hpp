/*!***************************************************************************************
\file       NodeManager.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/
#pragma once

#include <unordered_map>
#include <rapidjson.h>
#include "document.h"

#include "EditorNode.hpp"

#define NOMINMAX



namespace Editor
{
  class engineController;
  struct EditorLogger;
}


class NodeManager
{
public:
	NodeManager(Editor::engineController * p_engine_controller, Editor::EditorLogger & p_logger);

	~NodeManager();

	void reset();

	void addNode(EditorNode * p_node, int p_id = 0);

	void setSelectedNodeId(int p_node_id);

	int getSelectedNodeId() const { return m_node_selected; }

	std::unordered_map<int, EditorNode *> * getNodeList() { return m_nodes; };

	const std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> & getNodeDefinitions();
	
	//Hard coded node factory function
	EditorNode * createNode(const std::string & p_type_name, const std::pair<std::string, std::string> & p_node_type_name);

  void removeNode(int p_id);

  void retrieveNodeFactoryList();

  void setActiveNodeData();

  void linkNodes(int p_output_id, int p_input_id, int p_output_slot = 0, int p_input_slot = 0);

  void removeLink(int p_output_id, int p_input_id);

  bool removeLinkFromOutputSlot(int p_output_id, int p_output_slot);
  bool removeLinkFromInputSlot(int p_input_id, int p_input_slot);

  EditorNode * getNode(int p_node_id);

  bool linkAlreadyExists(int p_output_id, int p_input_id);

  void findFreeLinkSlots(int p_output_id, int p_input_id, int & p_output_slot, int & p_input_slot);

  bool checkOutputSlotTaken(int p_output_id, int p_output_slot);
  bool checkInputSlotTaken(int p_input_id, int p_input_slot);

  bool serializeNodes(const std::string & p_path, const std::string & p_graph_name);

  EditorNode * createEditorNodeFromScratch(const std::string & p_type_name,
      const std::pair<std::string, std::string> & p_node_type_name);


  // creates node from file given
  void readFromFile(const std::string & p_file_name);

  // create an id that is not currently in use with the node manager
  int createUniqueID() const;

  bool validateGraph();

  void setEditorNodesScale(float p_scalar, float p_difference);

private:
     // takes in json object and uses it to fill in the node appropriately
     EditorNode* createEditorNodeJSON(const std::string& p_type_name,
     std::pair<std::string, std::string> p_node_type_name, const rapidjson::Value &);


	int m_num_nodes;
  int m_num_active_nodes;
	int m_node_selected;

	std::unordered_map<int, EditorNode *> * m_nodes;

  Editor::engineController * m_engine_controller;

	//List of types of nodes and their names
	//Key = behavior tree, script, shader, etc. pair = class name, friendly name (can be the same)
	std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> m_node_factory_list;

  Editor::EditorLogger & m_logger;
};
