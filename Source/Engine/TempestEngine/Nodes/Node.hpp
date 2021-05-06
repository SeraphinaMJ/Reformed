/*!***************************************************************************************
\file       Node.hpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#pragma once

#include <string>

#include <Reflection.hpp>

class Node
{
public:
  Node();

  virtual ~Node() = default;

  typeRT render() const;

  virtual typeRT toTypeRT() const = 0;

  virtual void updateFromTypeRT(typeRT & p_data) = 0;

  virtual std::pair<bool, std::string> validate() = 0;

  int getId() const { return m_id; }
  void setId(const int& newID) { m_id = newID; }

  const std::string & getName() const { return m_name; }
  void SetName(const std::string& name) { this->m_name = name; }

  int getInputs() const { return m_inputs; }
  int getOutputs() const { return m_outputs; }

  int getNumChildren() const { return m_num_children; }
  int getNumParents() const { return m_num_parents; }

protected:
  void setName(const std::string & p_name) { m_name = p_name; }

  void setIntputs(int p_inputs) { m_inputs = p_inputs; }
  void setOutputs(int p_outputs) { m_outputs = p_outputs; }

  //void setId(int p_id) { m_id = p_id; }

private:
  int m_id;
  int m_depth;
  std::string m_name;
 
  int m_inputs;
  int m_outputs;

  int m_num_children;
  int m_num_parents;
};