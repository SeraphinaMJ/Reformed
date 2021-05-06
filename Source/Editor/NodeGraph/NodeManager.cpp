/*!***************************************************************************************
\file       NodeManager.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include <fstream>
#include <sstream>
//#include <objbase.h>

#include "NodeManager.hpp"

#include "../BehaviorSystem/BehaviorInterpreter.hpp"

#include "../EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>
#include "../EditorLogger.hpp"

#ifdef max
#undef max // fuck you microsoft
#endif

//RapidJSON
#include <rapidjson.h>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "istreamwrapper.h"
#include "filereadstream.h"


NodeManager::NodeManager(Editor::engineController * p_engine_controller, Editor::EditorLogger & p_logger) : m_num_nodes(0), m_num_active_nodes(0), m_node_selected(-1), m_nodes(new std::unordered_map<int, EditorNode*>()), m_engine_controller(p_engine_controller), m_logger(p_logger)
{
  
}

NodeManager::~NodeManager()
{
	reset();

	delete m_nodes;
}

void NodeManager::reset()
{
	//Delete all the nodes
	for (auto i_node = m_nodes->begin(); i_node != m_nodes->end(); ++i_node)
	{
		delete i_node->second;
	}

	m_nodes->clear();

	m_num_nodes = 0;
  m_num_active_nodes = 0;
	m_node_selected = -1;
  m_logger.AddLog("[NOTICE] Resetting Node Manager.\n");
}

void NodeManager::addNode(EditorNode * p_node, int p_id)
{
	//Generate id for // node SHOULD BE GUID
  //GUID l_gidReference;
  //HRESULT hCreateGuid = CoCreateGuid(&l_gidReference);

  //int l_new_id = 0;

  //if(p_id != 0)
  //{
  //  l_new_id = m_num_nodes;
  //}
  //else
  //{
  //  l_new_id = p_id;
  //}

	//Make sure a node with given id doesn't already exist
	if (m_nodes->find(p_id/*l_new_id*/) == m_nodes->end())
	{
		p_node->setId(/*l_new_id*/p_id);

		bool l_result = m_nodes->insert(std::make_pair(p_id/*l_new_id*/, p_node)).second;

		if (!l_result)
		{
          m_logger.AddLog("[ERROR] Failed to insert node with id: %i into manager.\n", p_id/*l_new_id*/);
		}
		else
		{
			m_num_nodes += 1;
            m_num_active_nodes += 1;
		}
	}
	else
	{
      m_logger.AddLog("[ERROR] Node with given id: %i already exists!\n", p_id/*l_new_id*/);
	}
}

void NodeManager::setSelectedNodeId(int p_node_id)
{
	//Validate node with given id exists
	if (m_nodes->find(p_node_id) != m_nodes->end())
	{
		m_node_selected = p_node_id;
	}
	else if(p_node_id == -1)
	{
		m_node_selected = -1;
	}
	else
	{
    m_logger.AddLog("[ERROR] No node can be selected with id: %i.\n", p_node_id);

		m_node_selected = -1;
	}
}

const std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>>& NodeManager::getNodeDefinitions()
{
	return m_node_factory_list;
}

EditorNode * NodeManager::createEditorNodeFromScratch(const std::string & p_type_name, 
    const std::pair<std::string, std::string> & p_node_type_name)
{
  EditorNode * l_new_node = new EditorNode();

  if(l_new_node != nullptr)
  {
    l_new_node->setName(p_node_type_name.second);
    l_new_node->setClassName(p_node_type_name.first);
    l_new_node->setTypeName(p_type_name);

    //node manipulator ask for typeRT
    auto Node_Manipulator = m_engine_controller->getEngineRunner()->getEngine()->getNodeManipulator().lock();
    

    typeRT l_data = BehaviorInterpreter::GetInstance().GetDefaultTypeRT(p_node_type_name.first);
    l_new_node->setRenderData(l_data);

    return l_new_node;
  }
  m_logger.AddLog("[ERROR] Failed to create node!\n");

  return nullptr;
}

EditorNode * NodeManager::createEditorNodeJSON(const std::string & p_type_name,
    std::pair<std::string, std::string> p_node_type_name, const rapidjson::Value & json_data)
{
    EditorNode * l_new_node = new EditorNode();

    if (l_new_node != nullptr)
    {
        l_new_node->setName(p_node_type_name.second);
        l_new_node->setClassName(p_node_type_name.first);
        l_new_node->setTypeName(p_type_name);

        //node manipulator ask for typeRT
        auto Node_Manipulator = m_engine_controller->getEngineRunner()->getEngine()->getNodeManipulator().lock();

        typeRT specializedData;
        BehaviorInterpreter::GetInstance().FillTypeRT(specializedData, p_node_type_name.first, json_data);

        l_new_node->setRenderData(specializedData);

        return l_new_node;
    }
    m_logger.AddLog("[ERROR] Failed to create node!\n");

    return nullptr;
}

void NodeManager::removeNode(int p_node_id)
{
  //Validate node with given id exists
  auto i_node = m_nodes->find(p_node_id);

  if (i_node != m_nodes->end())
  {
    //Delete node links associated with this node
    for(auto i_node_linked = m_nodes->begin(); i_node_linked != m_nodes->end(); ++i_node_linked)
    {
      std::vector<NodeLink> l_links = i_node_linked->second->getNodeLinks();

      for(auto i_node_link = l_links.begin(); i_node_link != l_links.end(); ++i_node_link)
      {
        if(i_node_link->OutputIdx == p_node_id || i_node_link->InputIdx == p_node_id)
        {
          i_node_linked->second->removeNodeLink(*i_node_link);
        }
      }
    }

    delete i_node->second;

    m_nodes->erase(i_node);

    m_num_active_nodes -= 1;
  }

  else
  {
    m_logger.AddLog("[ERROR] Can not delete node with given id: %i.\n", p_node_id);
  }
}

void NodeManager::retrieveNodeFactoryList()
{
  //Get the types of nodes we can create
  m_node_factory_list = m_engine_controller->getEngineRunner()->getEngine()->getNodeManipulator().lock()->getNodeDefinitions();
}

void NodeManager::setActiveNodeData()
{
  //Retrieve the data of only the active node and constantly update its data from the engine
}

void NodeManager::linkNodes(int p_output_id, int p_input_id, int p_output_slot, int p_input_slot)
{
  if(p_input_id != p_output_id)
  {
    EditorNode * l_input_node = getNode(p_input_id);
    EditorNode * l_output_node = getNode(p_output_id);

    if(l_input_node != nullptr && l_output_node != nullptr)
    {
      //Check if this link already exists
      if(linkAlreadyExists(p_output_id, p_input_id))
      {
        return;
      }

      //TODO: Is this necessary anymore?
      //Remove old link if it exists
      //removeLink(p_output_id, p_input_id);

      //Set Link slot
      int l_output_slot = p_output_slot;
      int l_input_slot = p_input_slot;

      //Check if the output slot and input slot is open
      if(checkOutputSlotTaken(p_output_id, p_output_slot) || checkInputSlotTaken(p_input_id, p_input_slot))
      {
        findFreeLinkSlots(p_output_id, p_input_id, l_output_slot, l_input_slot);
      }

      //Add new link
      NodeLink l_new_link(p_input_id, l_input_slot, p_output_id, l_output_slot);

      l_output_node->addNodeLink(l_new_link);
      l_input_node->addNodeLink(l_new_link);

      //Only do this if all slots will be full after adding
      //Add extra output / input slot
      if(l_output_slot == l_output_node->getOutputs() - 1)
      {
        l_output_node->setOutputs(l_output_node->getOutputs() + 1);
      }
      if (l_input_slot == l_input_node->getInputs() - 1)
      {
        l_input_node->setIntputs(l_input_node->getInputs() + 1);
      }

    }
  }
  else
  {
    m_logger.AddLog("[ERROR] Can not link the same node ids.\n");
  }
}

void NodeManager::removeLink(int p_output_id, int p_input_id)
{
  EditorNode * l_output_node = getNode(p_output_id);
  EditorNode * l_input_node = getNode(p_input_id);

  if(l_output_node != nullptr && l_input_node != nullptr)
  {
    //input/output slot don't matter, only compare ids
    NodeLink l_temp_link(p_input_id, 0, p_output_id, 0);

    l_output_node->removeNodeLink(l_temp_link);
    l_input_node->removeNodeLink(l_temp_link);
  }
  else
  {
    m_logger.AddLog("[ERROR] Can not remove link. One of the node ids is invalid.\n");
  }
}

bool NodeManager::removeLinkFromOutputSlot(int p_output_id, int p_output_slot)
{
  EditorNode * l_output_node = getNode(p_output_id);

  if(l_output_node != nullptr)
  {
    std::vector<NodeLink> l_node_links = l_output_node->getNodeLinks();

    for(auto i_link = l_node_links.begin(); i_link != l_node_links.end(); ++i_link)
    {
      if(i_link->OutputIdx == p_output_id && i_link->OutputSlot == p_output_slot)
      {
        int l_input_id = i_link->InputIdx;

        EditorNode * l_input_node = getNode(l_input_id);

        if(l_input_node != nullptr)
        {
          //input/output slot don't matter, only compare ids
          NodeLink l_temp_link(l_input_id, 0, p_output_id, 0);

          l_output_node->removeNodeLink(l_temp_link);
          l_input_node->removeNodeLink(l_temp_link);

          return true;
        }
      }
    }
  }

  return false;
}

bool NodeManager::removeLinkFromInputSlot(int p_input_id, int p_input_slot)
{
  EditorNode * l_input_node = getNode(p_input_id);

  if (l_input_node != nullptr)
  {
    std::vector<NodeLink> l_node_links = l_input_node->getNodeLinks();

    for (auto i_link = l_node_links.begin(); i_link != l_node_links.end(); ++i_link)
    {
      if (i_link->InputIdx == p_input_id && i_link->InputSlot == p_input_slot)
      {
        int l_output_id = i_link->OutputIdx;

        EditorNode * l_output_node = getNode(l_output_id);

        if (l_output_node != nullptr)
        {
          //input/output slot don't matter, only compare ids
          NodeLink l_temp_link(p_input_id, 0, l_output_id, 0);

          l_output_node->removeNodeLink(l_temp_link);
          l_input_node->removeNodeLink(l_temp_link);

          return true;
        }
      }
    }
  }

  return false;
}

EditorNode* NodeManager::getNode(int p_node_id)
{
  //Validate node with given id exists
  auto i_node = m_nodes->find(p_node_id);

  if (i_node != m_nodes->end())
  {
    return i_node->second;
  }

  m_logger.AddLog("[ERROR] Can not retrieve node with given id: %i.\n", p_node_id);
  return nullptr;
}

bool NodeManager::linkAlreadyExists(int p_output_id, int p_input_id)
{
  EditorNode * l_output_node = getNode(p_output_id);

  if(l_output_node != nullptr)
  {
    std::vector<NodeLink> l_node_links = l_output_node->getNodeLinks();

    for(size_t i = 0; i < l_node_links.size(); ++i)
    {
      if(l_node_links[i].InputIdx == p_input_id && l_node_links[i].OutputIdx == p_output_id)
      {
        return true;
      }
    }
  }

  return false;
}

void NodeManager::findFreeLinkSlots(int p_output_id, int p_input_id, int & p_output_slot, int & p_input_slot)
{
  EditorNode * l_output_node = getNode(p_output_id);
  EditorNode * l_input_node = getNode(p_input_id);

  if(l_output_node != nullptr && l_input_node != nullptr)
  {
    //Find free output slot
    std::vector<NodeLink> l_output_links = l_output_node->getNodeLinks();

    int l_max_output_slot = -1;
    int l_max_input_slot = -1;

    for(size_t i = 0; i < l_output_links.size(); ++i)
    {
      if(l_output_links[i].OutputIdx == p_output_id)
      {
        if(l_max_output_slot < l_output_links[i].OutputSlot)
        {
          l_max_output_slot = l_output_links[i].OutputSlot;
        }
      }
    }

    p_output_slot = l_max_output_slot + 1;

    //Find free output slot
    std::vector<NodeLink> l_input_links = l_input_node->getNodeLinks();

    for (size_t j = 0; j < l_input_links.size(); ++j)
    {
      if (l_input_links[j].InputIdx == p_input_id)
      {
        if (l_max_input_slot < l_input_links[j].InputSlot)
        {
          l_max_input_slot = l_input_links[j].InputSlot;
        }
      }
    }

    p_input_slot = l_max_input_slot + 1;
  }
}

bool NodeManager::checkOutputSlotTaken(int p_output_id, int p_output_slot)
{
  EditorNode * l_output_node = getNode(p_output_id);

  if (l_output_node != nullptr)
  {
    //Find free output slot
    std::vector<NodeLink> l_output_links = l_output_node->getNodeLinks();

    for (size_t i = 0; i < l_output_links.size(); ++i)
    {
      if (l_output_links[i].OutputIdx == p_output_id)
      {
        if(l_output_links[i].OutputSlot == p_output_slot)
        {
          return true;
        }
      }
    }

    return false;
  }

  return true;
}

bool NodeManager::checkInputSlotTaken(int p_input_id, int p_input_slot)
{
  EditorNode * l_input_node = getNode(p_input_id);

  if (l_input_node != nullptr)
  {
    //Find free input slot
    std::vector<NodeLink> l_input_links = l_input_node->getNodeLinks();

    for (size_t i = 0; i < l_input_links.size(); ++i)
    {
      if (l_input_links[i].InputIdx == p_input_id)
      {
        if (l_input_links[i].InputSlot == p_input_slot)
        {
          return true;
        }
      }
    }

    return false;
  }

  return true;
}
rapidjson::Value nodeLinkToStream(NodeLink p_node_link, rapidjson::Document::AllocatorType& p_allocator)
{
  rapidjson::Value l_result(rapidjson::kArrayType);

  rapidjson::Value l_output_id;
  l_output_id.SetInt(p_node_link.OutputIdx);

  rapidjson::Value l_input_id;
  l_input_id.SetInt(p_node_link.InputIdx);

  rapidjson::Value l_output_slot;
  l_output_slot.SetInt(p_node_link.OutputSlot);

  rapidjson::Value l_input_slot;
  l_input_slot.SetInt(p_node_link.InputSlot);

  l_result.PushBack(l_input_id, p_allocator);
  l_result.PushBack(l_input_slot, p_allocator);
  l_result.PushBack(l_output_id, p_allocator);
  l_result.PushBack(l_output_slot, p_allocator);


  return l_result;
}

//the thread has exited with code 0xc000007b

//http://rapidjson.org/md_doc_tutorial.html

bool NodeManager::serializeNodes(const std::string & p_path, const std::string & p_graph_name)
{
  if(!m_nodes->empty())
  {
    rapidjson::Document l_document;
    l_document.SetObject();

    rapidjson::Document::AllocatorType & l_allocator = l_document.GetAllocator();

    rapidjson::Value l_graph(rapidjson::kArrayType);

    for(auto i_node = m_nodes->begin(); i_node != m_nodes->end(); ++i_node)
    {
        const char* EditorNodeName = i_node->second->getName().c_str();

      rapidjson::Document l_node;
      l_node.SetObject();

      //Type Name
      rapidjson::Value l_type_name;
      l_type_name.SetString(i_node->second->getTypeName().c_str(), l_allocator);
      l_node.AddMember("type name", l_type_name, l_allocator);

      //Class Name
      rapidjson::Value l_class_name;
      l_class_name.SetString(i_node->second->getClassName().c_str(), l_allocator);
      l_node.AddMember("class name", l_class_name, l_allocator);

      //Name
      rapidjson::Value l_name;
      l_name.SetString(i_node->second->getName().c_str(), l_allocator);
      l_node.AddMember("name", l_name, l_allocator);

      //Id
      rapidjson::Value l_id;
      l_id.SetInt(i_node->second->getId());
      l_node.AddMember("id", l_id, l_allocator);

      //Position
      ImVec2 l_node_position = i_node->second->getPosition();
      rapidjson::Value l_position(rapidjson::kArrayType);
      l_position.PushBack(l_node_position.x, l_allocator);
      l_position.PushBack(l_node_position.y, l_allocator);
      l_node.AddMember("position", l_position, l_allocator);

      //Create each node link (output only)
      std::vector<NodeLink> l_node_links = i_node->second->getNodeLinks();

      rapidjson::Value l_node_output_links_array(rapidjson::kArrayType);
      rapidjson::Value l_node_input_links_array(rapidjson::kArrayType);

      for(auto i_node_link = l_node_links.begin(); i_node_link != l_node_links.end(); ++i_node_link)
      {
        if(i_node_link->OutputIdx == i_node->second->getId())
        {
          l_node_output_links_array.PushBack(nodeLinkToStream(*i_node_link, l_allocator), l_allocator);
        }
        else if(i_node_link->InputIdx == i_node->second->getId())
        {
          l_node_input_links_array.PushBack(nodeLinkToStream(*i_node_link, l_allocator), l_allocator);
        }
      }

      l_node.AddMember("node output links", l_node_output_links_array, l_allocator);
      l_node.AddMember("node input links", l_node_input_links_array, l_allocator);

      // add on specialized data
      auto interpreter = BehaviorInterpreter::GetInstance();
      //auto visitor = BehaviorVisitor::GetInstance();
      if (interpreter.IsSpecialized(i_node->second->getName()))
      {
        interpreter.FillJSON( i_node->second->m_render_data.member("Node Render Data"), i_node->second->getName(), l_node);
      }
      // end of specialized data

      l_graph.PushBack(l_node, l_allocator);
    }

    l_document.AddMember("graph", l_graph, l_allocator);

    rapidjson::StringBuffer l_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(l_buffer);
    l_document.Accept(writer);

    //Combine path
    std::string p_file = p_path + "/Assets/graphs/" + p_graph_name;

    //Create file
    std::ofstream l_file(p_file.c_str());

    if(l_file.is_open())
    {
      l_file << l_buffer.GetString();

      l_file.close();
      m_logger.AddLog("[NOTICE] Successfully saved graph: %s. \n", p_file.c_str());
      return true;
    }
    else
    {
      m_logger.AddLog("[ERROR] Failed to create file: %s.\n", p_file.c_str());
      return false;
    }
  }
  return true;
}

void NodeManager::readFromFile(const std::string & p_file_name)
{
  std::ifstream l_if_stream(p_file_name);

  if(l_if_stream.is_open())
  {
    std::string file_as_string((std::istreambuf_iterator<char>(l_if_stream)), std::istreambuf_iterator<char>());

    rapidjson::Document l_document;
    l_document.Parse(file_as_string.c_str());

    if(l_document.HasMember("graph"))
    {
      const rapidjson::Value & l_graph = l_document["graph"];

      for(rapidjson::SizeType i = 0; i < l_graph.Size(); ++i)
      {
        const rapidjson::Value & l_node = l_graph[i];

        if(l_node.IsObject())
        {
          std::string l_type_name = "";
          std::string l_class_name = "";
          std::string l_name = "";
          int l_id = 0;
          ImVec2 l_position(0.0f, 0.0f);

          //Type Name
          if (l_node.HasMember("type name"))
          {
            l_type_name = l_node["type name"].GetString();
          }
          //Class Name
          if (l_node.HasMember("class name"))
          {
            l_class_name = l_node["class name"].GetString();
          }
          //Name
          if(l_node.HasMember("name"))
          {
            l_name = l_node["name"].GetString();
          }
          //Id
          if(l_node.HasMember("id"))
          {
            l_id = l_node["id"].GetInt();
          }
          //Position
          if(l_node.HasMember("position"))
          {
            l_position.x = l_node["position"][0].GetFloat();
            l_position.y = l_node["position"][1].GetFloat();
          }

          EditorNode * l_new_node;
          if (BehaviorInterpreter::GetInstance().IsSpecialized(l_name))
          {
             l_new_node = createEditorNodeJSON(l_type_name,
                  std::pair<std::string, std::string>(l_class_name, l_name), l_node);
          }
          else
          {
              l_new_node = createEditorNodeFromScratch(l_type_name,
                  std::pair<std::string, std::string>(l_class_name, l_name));
          }
          

          //Failed to create node
          if(l_new_node != nullptr)
          {
            if (l_node.HasMember("node output links"))
            {
              const rapidjson::Value & l_node_output_links = l_node["node output links"];

              for (size_t j = 0; j < l_node_output_links.Size(); ++j)
              {
                NodeLink l_new_link(l_node_output_links[j][0].GetInt(), l_node_output_links[j][1].GetInt(), l_node_output_links[j][2].GetInt(), l_node_output_links[j][3].GetInt());

                l_new_node->addNodeLink(l_new_link);
              }
            }

            if(l_node.HasMember("node input links"))
            {
              const rapidjson::Value & l_node_input_links = l_node["node input links"];

              for (size_t j = 0; j < l_node_input_links.Size(); ++j)
              {
                NodeLink l_new_link(l_node_input_links[j][0].GetInt(), l_node_input_links[j][1].GetInt(), l_node_input_links[j][2].GetInt(), l_node_input_links[j][3].GetInt());

                l_new_node->addNodeLink(l_new_link);
              }
            }
            //For old graphs that haven't followed the new saving format
            if (l_node.HasMember("node links"))
            {
              const rapidjson::Value & l_node_links = l_node["node links"];

              for (size_t j = 0; j < l_node_links.Size(); ++j)
              {
                NodeLink l_new_link(l_node_links[j][0].GetInt(), l_node_links[j][1].GetInt(), l_node_links[j][2].GetInt(), l_node_links[j][3].GetInt());

                l_new_node->addNodeLink(l_new_link);
              }
            }

            addNode(l_new_node, l_id);
            l_new_node->setPosition(l_position);
          }
        }
      }
      m_logger.AddLog("[NOTICE] Successfully read graph: %s. \n", p_file_name.c_str());
    }

    l_if_stream.close();
  }
  else
  {
    m_logger.AddLog("[ERROR] Failed to open file to read: %s. \n", p_file_name.c_str());
  }
}

int NodeManager::createUniqueID() const
{
    int uniqueID = 0;
    while (this->m_nodes->find(uniqueID) != m_nodes->end())
    {
        uniqueID++;
    }

    return uniqueID;
}

bool NodeManager::validateGraph()
{
  int l_num_roots = 0;

  //Loop over each node
  for(auto i_node = m_nodes->begin(); i_node != m_nodes->end(); ++i_node)
  {
    //TODO: Call the validate function
    //auto l_result = i_node->second->validate();
    //if(!l_result.first)
    //{
    //  m_logger.AddLog("[ERROR] %s.\n", l_result.second.c_str());
    //  return false;
    //}

    //Loop over all the node links
    std::vector<NodeLink> l_node_links = i_node->second->getNodeLinks();

    int l_num_input_links = 0;

    for(auto i_link = l_node_links.begin(); i_link != l_node_links.end(); ++i_link)
    {
      //This node is an input node (so it has a parent)
      if(i_node->first == i_link->InputIdx)
      {
        l_num_input_links += 1;
      }
    }
    //This node has no inputs so it is a root
    if(l_num_input_links == 0)
    {
      l_num_roots += 1;
    }
  }

  if(l_num_roots > 1)
  {
    m_logger.AddLog("[ERROR] Graph can not have more than 1 root node.\n");
    return false;
  }

  m_logger.AddLog("[NOTICE] Graph validated successfully.\n");
  return true;
}

void NodeManager::setEditorNodesScale(float p_scalar, float p_difference)
{
  for (auto i_node = m_nodes->begin(); i_node != m_nodes->end(); ++i_node)
  {
    i_node->second->setZoomScale(p_scalar);

    //Change position based off of new scale
    ImVec2 l_new_position = i_node->second->getPosition();

    //l_new_position.x += l_new_position.x * (p_difference);
    //l_new_position.y += l_new_position.y * (p_difference);

    ImVec2 l_scale = i_node->second->getScaleAfterZoom();

    l_new_position.x += (l_scale.x / 2.0f) * (p_difference);
    l_new_position.y += (l_scale.y / 2.0f) * (p_difference);

    i_node->second->setPosition(l_new_position);
  }
}
