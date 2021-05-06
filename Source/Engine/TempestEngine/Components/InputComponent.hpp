/*!***************************************************************************************
\file       InputComponent.hpp
\author     Aaron Damyen
\date       6/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the input component.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "InputConstants.hpp"

//========3rd Party Includes============================================================//
#include <string>
#include <unordered_map>
#include <Component.hpp>


/*!***************************************************************************************
\par class: inputComponent
\brief   Defines a component for translating input commands into actions.
*****************************************************************************************/
class inputComponent final : public componentCRTP<inputComponent>
{
public:
	using name = std::string;

	
	/*!***************************************************************************************
	\brief  Creates the input component object and initializes it.
	*****************************************************************************************/
	inputComponent() = default;

	/*!***************************************************************************************
	\brief  Constructs this object and sets the initial values from the typeRT data.
	\param p_type_data - the typeRT data structure with the initial values
	*****************************************************************************************/
	inputComponent(typeRT & p_type_data);

	/*!***************************************************************************************
	\brief  Destroys the input component, releasing all allocated resources.
	*****************************************************************************************/
	virtual ~inputComponent();
	
	/*!***************************************************************************************
	\brief  Converts this object into a typeRT structure
	\return typeRT - a typeRT structure with all serializable values for this object
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Updates the values of this object to the values in the typeRT structure
	\param p_type - a typeRT structure with all serializable values for this object
	*****************************************************************************************/
	void updateFromTypeRT(typeRT& p_type) override;

	/*!***************************************************************************************
	\brief  Retrieves the state of a button mapped to a name.
	\param p_name - The name of the button to retrieve.
	\return bool - Returns true if the button is pressed, false otherwise.
	*****************************************************************************************/
	bool getButtonState(name const & p_name);

	/*!***************************************************************************************
	\brief  Retrieves the state of an axis mapped to a name.
	\param p_name - The name of the axis to retrieve.
	\return double - Returns the value of the specified axis.
	*****************************************************************************************/
	float getAxisState(name const & p_name);

	/*!***************************************************************************************
	\brief Get the connection status of a given named controller
	\param p_name name of connection state 
	\return true if controller connected, false otherwise
	*****************************************************************************************/
	bool getConnectionState(name const& p_name);

	/*!***************************************************************************************
	\brief  Retrieves the state of a cursor mapped to a name.
	\param p_name - The name of the cursor to retrieve.
	\return std::pair<double, double> - Returns the (x,y) coordinates of the cursor.
	*****************************************************************************************/
	cursorPosition getCursorState(name const & p_name);

	/*!***************************************************************************************
	\brief  Assigns this instance to a specific context.
	\param p_context - the context to which this instance is assigned.
	*****************************************************************************************/
	void setContext(std::string p_context);

	/*!***************************************************************************************
	\brief  Retrieves the currently assign context.
	\return std::string - The context currently assigned.
	*****************************************************************************************/
	std::string getContext() const;

	/*!***************************************************************************************
	\brief  Associates a name with a lettered button on the keyboard.
	\param p_name - The name that is the range for the association.
	\param p_letter - The letter for the button that is the domain for the association.
	*****************************************************************************************/
	void addButtonMapping(name const & p_name, letter p_letter);

	void addButtonMapping(name const & p_name, Key p_key);

	void addButtonMapping(name const & p_name, MouseButton p_button);

	/*!***************************************************************************************
	\brief  Removes all button associations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeButtonMapping(std::string p_name);

	/*!***************************************************************************************
	\brief  Associates a name with an axis.
	\param p_name - The name that is the range for the association.
	\param p_axiscode - The axis that is the domain for the association.
	*****************************************************************************************/
	void addAxisMapping(name const & p_name, axis p_axiscode);

	/*!***************************************************************************************
	\brief  Removes all axis associations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeAxisMapping(name const & p_name);

	/*!***************************************************************************************
	\brief  Associates a name with a cursor.
	\param p_name - The name that is the range for the assocation.
	\param p_cursorcode - The cursor that is the domain for the association.
	*****************************************************************************************/
	void addCursorMapping(name const & p_name, Cursor p_cursorcode);

	/*!***************************************************************************************
	\brief  Removes all cursor assocations for a name.
	\param p_name - The name of the range of the associations to remove.
	*****************************************************************************************/
	void removeCursorMapping(name const & p_name);

	void addGamepadButtonMapping(name const & p_name, input_button p_button, gamepad_id p_controllerid);

	void removeGamepadButtonMapping(name const & p_name);

	void addGamepadAxisMapping(name const & p_name, axis p_axis, gamepad_id p_controllerid);

	void removeGamepadAxisMapping(name const & p_name);

	void addControllerStateMapping(name const & p_name, gamepad_id p_id);

	void removeControllerStateMapping(name const & p_name);

	void clearAllMaps();

	void setMouseCapture(bool p_enable);
	
	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	componentType const type() const override { return inputComponent::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<inputComponent>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Input", true, true, false); }

private:
	using gamepad_button = std::pair<input_button, gamepad_id>;
	using gamepad_axis = std::pair<axis, gamepad_id>;

	std::string context; //!< the context filter this component belongs to.

	
	std::unordered_map<name, letter> nameToLetterMap;
	std::unordered_map<name, MouseButton> nameToMouseButtonMap;
	std::unordered_map<name, gamepad_button> nameToGamepadButtonMap;
	std::unordered_map<name, axis> nameToAxisMap;
	std::unordered_map<name, gamepad_axis> nameToGamepadAxisMap;
	std::unordered_map<name, Cursor> nameToCursorMap;
	std::unordered_map<name, gamepad_id> nameToControllerMap;

};

