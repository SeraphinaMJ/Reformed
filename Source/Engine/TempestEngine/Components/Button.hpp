/*!**********************************************************************************************************
\file       Button.hpp
\author     Ryan Booth
\date       10/19/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      This file contains the declarations of the button component.
************************************************************************************************************/
#pragma once

//========  1st Party Includes  ===========================================================================//
#include "../SceneManagement/GameObject.hpp"

//========  3rd Party Includes  ===========================================================================//
#include <Component.hpp>
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

/*!***************************************************************************************
\brief UI Component for 2D assets or buttons.
*****************************************************************************************/
class button : public componentCRTP<button>
{
public:

  /*!***************************************************************************************
  \brief Directions for buttons to have neighbors.
  *****************************************************************************************/
  enum buttonDirection 
  { DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_INVALID
  };


  //====  Static  =======================================================================================//
  //====  Pure-Virtual  ========== =======================================================================//
  //====  Constructor/Destructor  =======================================================================//

  /*!***************************************************************************************
  \brief Default constructor. Initalizes private data.
  *****************************************************************************************/
  button();

  /*!***************************************************************************************
  \brief Conversion constructor from typeRT.
  \param p_type typeRT that holds the UI data.
  *****************************************************************************************/
  button(typeRT & p_type);

  //====  Accessor Methods  =============================================================================//
  //====  Virtual/Overrides  ============================================================================//

  /*!***************************************************************************************
  \brief Converts the private data into a typeRT to serialize.
  \return Constructed typeRT.
  *****************************************************************************************/
  typeRT toTypeRT() const override;

  componentType const type() const override { return getType(); }
  static componentType const getType() { static componentType const type = componentType::forConcrete<button>(); return type; }
  static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Button", true, true, false); }

  /*!***************************************************************************************
  \brief Changes any private data inside the UI component that is stored in the typeRT.
  \param p_type typeRT to update private values from.
  *****************************************************************************************/
  void updateFromTypeRT(typeRT& p_type) override;

  //====  Operators  ====================================================================================//
  //====  Interface Declarations (For each interface Implemented)  ======================================//
  //====  Everything Else  ==============================================================================//

  /*!***************************************************************************************
  \brief Sets a button's neighbor that can be accesed from this button in a givren direction. 
  \param p_neighbor The gameObject that will be your neighbor and can be moved to.
  \param p_direction The direction the neighbor will be in.
  \param p_mirrored Whether or not to apply the same operation in the opposite direction for
         the neighbor to this button.
  *****************************************************************************************/
  void setNeighbor(std::shared_ptr<gameObject> p_neighbor, buttonDirection p_direction, bool p_mirrored);

  /*!***************************************************************************************
  \brief Removes a button's neighbor in a given direction.
  \param p_direction The direction to remove.
  \return The object id of the removed neighbor.
  *****************************************************************************************/
  objID removeNeighbor(buttonDirection p_direction);

  /*!***************************************************************************************
  \brief If a button is active, it is visible.
  \param p_active True or false.
  *****************************************************************************************/
  void setActive(bool p_active);

  /*!***************************************************************************************
  \brief Whether or not the button is currently active and visible.
  \return True or false.
  *****************************************************************************************/
  bool isActive() const { return m_isActive; }

  /*!***************************************************************************************
  \brief Whether or not this button is currently selected so that it can be pressed.
  \param p_selected True or false.
  *****************************************************************************************/
  void setSelected(bool p_selected);

  /*!***************************************************************************************
  \brief Whether or not this button is currently pressed.
  \param p_pressed True or false.
  *****************************************************************************************/
  void setPressed(bool p_pressed);

  /*!***************************************************************************************
  \brief Retrieves this button's neighbor in a given direction if it exists.
  \param p_direction The direction to retrieve the neighbor from.
  \return A pointer to the gameObject of the neighbor, or nullptr.
  *****************************************************************************************/
  std::shared_ptr<gameObject> getNeighbor(buttonDirection p_direction);

  /*!***************************************************************************************
  \brief If true, its a button, if false its a UI.
  \param p_selectable True for button, false for UI.
  *****************************************************************************************/
  void setSelectable(bool p_selectable);

  /*!***************************************************************************************
  \brief Whether or not the button is currently pressed.
  \return True or false.
  *****************************************************************************************/
  bool isPressed() const { return m_isPressed;  }

  /*!***************************************************************************************
  \brief Whether or not the button is currently selected.
  \return True or false.
  *****************************************************************************************/
  bool isSelected() const { return m_isSelected; }

  /*!***************************************************************************************
  \brief True for button, false for UI.
  \return True or false.
  *****************************************************************************************/
  bool isSelectable() const { return m_isSelectable; }


protected:
  
private:
  //====  Data  =========================================================================================//

  /*!***************************************************************************************
  \brief If the UI component is active (visible).
  *****************************************************************************************/
  bool m_isActive = true;

  /*!***************************************************************************************
  \brief If the button is selected.
  *****************************************************************************************/
  bool m_isSelected = false;

  /*!***************************************************************************************
  \brief If the button is currently pressed.
  *****************************************************************************************/
  bool m_isPressed = false;

  /*!***************************************************************************************
  \brief If a UI component is a button or a UI.
  *****************************************************************************************/
  bool m_isSelectable = false;

  /*!***************************************************************************************
  \brief Map of this button's neighbors that stores the direction (key) and a pair:
         gameObject of the neighbor and if it is mirrored.
  *****************************************************************************************/
  std::map<buttonDirection, std::pair<std::shared_ptr<gameObject>, bool> > m_neighborMap;

  /*!***************************************************************************************
  \brief A map of the gameObjectID's of this button's neighbors. Seperate from the other map
         due to unknown sequence of when objects and components are created
         during deserialization.
  *****************************************************************************************/
  std::map<buttonDirection, objID> m_neighborObjectIDs;

  /*!***************************************************************************************
  \brief Given a direction, it returns the opposite direction.
  \param p_direction The direction to reverse.
  \return The opposite of the given direction.
  *****************************************************************************************/
  buttonDirection reverseDirection(buttonDirection p_direction) const;

  /*!***************************************************************************************
  \brief Initializes the maps of neighbors to nullptrs.
  *****************************************************************************************/
  void initializeData();

  /*!***************************************************************************************
  \brief Converts the direction enum to a string for serialization.
  \param p_direction The direction of a neighbor.
  \return A string version of the direction.
  *****************************************************************************************/
  std::string convertNeighborToString(buttonDirection p_direction) const;

};

