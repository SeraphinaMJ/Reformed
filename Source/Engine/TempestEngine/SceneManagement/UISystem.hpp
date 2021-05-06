/*!**********************************************************************************************************
\file       UISystem.hpp
\author     Ryan Booth
\date       10/29/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#pragma once

//========  1st Party Includes  ===========================================================================//
#include "../Components/Button.hpp"
#include "../SceneManagement/Transform.hpp"
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

class uiSystem final : public systemBase
{
public:
  //====  Static  =======================================================================================//
  //====  Pure-Virtual  =================================================================================//
  //====  Constructor/Destructor  =======================================================================//
  //====  Accessor Methods  =============================================================================//
  //====  Virtual/Overrides  ============================================================================//
  static const std::string& getName() { static const std::string n("uiSystem"); return n; }
  const std::string& name() const override { return getName(); }
  //====  Operators  ====================================================================================//
  //====  Interface Declarations (For each interface Implemented)  ======================================//
  //====  Everything Else  ==============================================================================//

  /*!***************************************************************************************
  \brief Initialize function for UISystem. Creates the uiSystemSpace and sets up the input
         component for the game object this system holds that controls menu input.
  *****************************************************************************************/
  void onInitialize() override;

  /*!***************************************************************************************
  \brief Checks the input component for any button presses and manages all buttons.
  *****************************************************************************************/
  void onUpdate() override;

  /*!***************************************************************************************
  \brief Sets init to false.
  *****************************************************************************************/
  void onShutdown() override;

  /*!***************************************************************************************
  \brief Creates the UiSystem space and game object for input.
  *****************************************************************************************/
  void onLevelLoad(const levelLoadEvent & /*p_event*/) override;

  /*!***************************************************************************************
  \brief Set's init to false.
  *****************************************************************************************/
  void onLevelUnload(const levelUnloadEvent & /*p_event*/) override;

  /*!***************************************************************************************
  \brief Adds a button to the uiSystem to track and manage.
  \param p_button Button component to add to the uiSystem to track and update.
  *****************************************************************************************/
  void addButton(componentHandle<button> p_button);

  /*!***************************************************************************************
  \brief Wrapping will loop to the top most available button in the opposite direction if a
         button does not have a neighbor in the requested direction.
  \param p_wrapping True or False.
  *****************************************************************************************/
  void setWrapping(bool p_wrapping);

  /*!***************************************************************************************
  \brief Sets the currently selected button (not pressed) in the uiSystem.
  \param p_button Pointer to the button component.
  *****************************************************************************************/
  void setCurrentSelectedButton(componentHandle<button> p_button);

  /*!***************************************************************************************
  \brief Sets the currently selected button (not pressed) in the uiSystem.
  \param p_buttonID ObjectID of the button object.
  *****************************************************************************************/
  void setCurrentSelectedButton(const objID p_buttonID);

  /*!***************************************************************************************
  \brief Checks for new buttons to add to the uiSystem and sets all buttons pressed state to
         false.
  *****************************************************************************************/
  void updateListOfButtons();

  void disableAllButtons();

  void enableAllButtons();

protected:

private:
  //====  Data  =========================================================================================//

  /*!***************************************************************************************
  \brief List of button object ID's uiSystem is managing.
  *****************************************************************************************/
  std::vector<objID> m_listOfButtons;

  /*!***************************************************************************************
  \brief The object ID of the currently selected button.
  *****************************************************************************************/
  objID m_currentSelectedButton;

  /*!***************************************************************************************
  \brief The name of the uiSystem object that has the input component to control buttons.
  *****************************************************************************************/
  std::string m_uiGameObjectName;

  /*!***************************************************************************************
  \brief Boolean for the state of uiSystem wrapping.
  *****************************************************************************************/
  bool m_wrapping = false;

  /*!***************************************************************************************
  \brief Checks whether a point is inside a rectangle. NEVER TESTED or used...
  \param p_center Center of the rectangle to check.
  \param p_scale Scale of the rectangle.
  \param p_point The point you want to check for inside-ness.
  \return True or false, whether or not the point is in the rectangle.
  *****************************************************************************************/
  bool isInsideRectangle(const vector3 & p_center, const vector3 & p_scale, const std::pair<double, double> & p_point);

  /*!***************************************************************************************
  \brief Searches the uiSystem list of buttons for the requested game object.
  \param p_buttonID Object ID of the button to find in the list of buttons.
  \return A pointer, if found, to the object requested.
  *****************************************************************************************/
  std::shared_ptr<gameObject> findButton(objID p_buttonID);

  /*!***************************************************************************************
  \brief Buttons can not be pressed faster than this cooldown. (To prevent double pressing)
  *****************************************************************************************/
  float m_coolDown;

  /*!***************************************************************************************
  \brief Whether or not we have created the UISystem space and game object
  *****************************************************************************************/
  bool m_init;

  /*!***************************************************************************************
  \brief Removes any dead UI component objects from our list.
  *****************************************************************************************/
  void cleanListOfButtons();

  /*!***************************************************************************************
  \brief Resets the current selected button to the first valid one it finds.
  *****************************************************************************************/
  void resetCurrentButton();

};

