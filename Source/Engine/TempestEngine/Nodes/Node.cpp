/*!***************************************************************************************
\file       Node.cpp
\author     Ryan Booth
\date       9/12/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

#include "Node.hpp"

Node::Node() : m_id(0), m_depth(0), m_name(""), m_inputs(0), m_outputs(0), m_num_children(0), m_num_parents(0)
{

}

typeRT Node::render() const
{
  return toTypeRT();
}