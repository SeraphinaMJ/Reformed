#pragma once
#include "Component.hpp"
#include "../EngineInterfaces/Public/IComponentRepository.hpp"

class minionGroup_3 : public componentCRTP<minionGroup_3>
{

public:

	/*!***************************************************************************************
	\brief  componentType getter
	\return The type of component this is
	*****************************************************************************************/
	componentType const type() const override { return minionGroup_3::getType(); }

	///*!*******************************************************************************
	//\brief  Converts this into a type
	//\return Type equivalent.
	//*********************************************************************************/
	typeRT toTypeRT() const override;

	///*!***************************************************************************************
	//\brief  Updates the values of this object to the values in the typeRT structure
	//\param p_type - a typeRT structure with all serializable values for this object
	//*****************************************************************************************/
	void updateFromTypeRT(typeRT& p_type) override;

	/*!***************************************************************************************
	\brief   Gets the type details of the component
	\return  The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Minion Group Three", true, true, false); }

	/*!***************************************************************************************
	\brief  static componentType getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<minionGroup_3>(); return s_type; }

private:
};