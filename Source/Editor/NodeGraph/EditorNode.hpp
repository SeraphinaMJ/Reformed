/*!***************************************************************************************
\file       EditorNode.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#pragma once

#include <imgui.h>
#include <string>
#include <vector>

#include <Reflection.hpp>

struct NodeLink
{
  int     InputIdx, InputSlot, OutputIdx, OutputSlot;

  NodeLink(int input_idx, int input_slot, int output_idx, int output_slot)
  {
    InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot;
  }

  bool operator==(const NodeLink & p_rhs) const
  {
    if(InputIdx == p_rhs.InputIdx && OutputIdx == p_rhs.OutputIdx)
    {
      return true;
    }

    return false;
  }
};

class EditorNode
{
  friend class NodeManager;
public:
  EditorNode();

  void render(bool p_engine_Active, const ImVec2 & p_render_position);

  std::pair<bool, std::string> validate();

  int getId() const { return m_id; }

  const std::string & getName() const { return m_name; }
  const std::string & getTypeName() const { return m_type_name; }

  int getInputs() const { return m_inputs; }
  int getOutputs() const { return m_outputs; }

  int getNumChildren() const { return m_num_children; }
  int getNumParents() const { return m_num_parents; }

  ImVec2 getPosition() const { return m_position; }
  ImVec2 getScale() const { return m_scale; }
  float getZoomScale() const { return m_zoom_scalar; }
  ImVec4 getColor() const { return m_color; }
  ImVec2 getScaleAfterZoom() const { return ImVec2(m_scale.x * m_zoom_scalar, m_scale.y * m_zoom_scalar); }

  void setPosition(ImVec2 p_position) { m_position = p_position; }
  void setScale(ImVec2 p_scale) { m_scale = p_scale; }
  void setZoomScale(float p_zoom_scalar) { m_zoom_scalar = p_zoom_scalar; }

  ImVec2 GetInputSlotPos(int slot_no) const;
  ImVec2 GetOutputSlotPos(int slot_no) const;

  bool getActive() const { return m_active; }

  std::vector<NodeLink> getNodeLinks() { return m_node_links; }

  int getDepth() const { return m_depth; }

  const std::string & getClassName() const { return m_class_name; }

protected:
  void setName(const std::string & p_name) { m_name = p_name; }
  void setTypeName(const std::string & p_type_name) { m_type_name = p_type_name; }

  void setIntputs(int p_inputs) { m_inputs = p_inputs; }
  void setOutputs(int p_outputs) { m_outputs = p_outputs; }

  void setColor(ImVec4 p_color) { m_color = p_color; }

  void setId(int p_id) { m_id = p_id; }

  void setActive(bool p_active) { m_active = p_active; }

  void setRenderData(const typeRT & p_render_data);

  void addNodeLink(NodeLink p_link);

  void removeNodeLink(NodeLink p_link);

  void setDepth(int p_depth) { m_depth = p_depth; }

  void setClassName(const std::string & p_class_name) { m_class_name = p_class_name; }

  void setScaleBasedOnData();

private:
  int m_id;
  int m_depth;
  std::string m_name;
  std::string m_class_name;
  std::string m_type_name;
 
  int m_inputs;
  int m_outputs;

  int m_num_children;
  int m_num_parents;

  ImVec2 m_position;
  ImVec2 m_scale;
  float m_zoom_scalar;

  ImVec4 m_color; //CMYK

  typeRT m_render_data;

  bool m_active; //Whether to be highlighted

  std::vector<NodeLink> m_node_links;

  void disableModifications();
  void enableModifications();
};