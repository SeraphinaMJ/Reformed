#include "MinionType_3.hpp"

typeRT minionType_3::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionType_3::updateFromTypeRT(typeRT& p_type)
{
}
