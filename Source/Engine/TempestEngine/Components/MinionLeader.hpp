#pragma once
#include "Component.hpp"
#include "../EngineInterfaces/Public/IComponentRepository.hpp"

class minionLeader : public componentCRTP<minionLeader>
{

public:

	/*!***************************************************************************************
	\brief  componentType getter
	\return The type of component this is
	*****************************************************************************************/
	virtual componentType const type() const override { static componentType const s_type = componentType::forConcrete<minionLeader>(); return s_type; }

	///*!*******************************************************************************
	//\brief  Converts this into a type
	//\return Type equivalent.
	//*********************************************************************************/
	virtual typeRT toTypeRT() const override;

	///*!***************************************************************************************
	//\brief  Updates the values of this object to the values in the typeRT structure
	//\param p_type - a typeRT structure with all serializable values for this object
	//*****************************************************************************************/
	virtual void updateFromTypeRT(typeRT& p_type) override;

	/*!***************************************************************************************
	\brief   Gets the type details of the component
	\return  The component type details
	*****************************************************************************************/
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Minion Leader", true, true, false); }

	/*!***************************************************************************************
	\brief  static componentType getter
	\return The type of component this is
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forAbstract<minionLeader>(); return s_type; }

private:
};