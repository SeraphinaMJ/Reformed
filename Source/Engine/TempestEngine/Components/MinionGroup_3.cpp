#include "MinionGroup_3.hpp"

typeRT minionGroup_3::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_3::updateFromTypeRT(typeRT& p_type)
{
}
