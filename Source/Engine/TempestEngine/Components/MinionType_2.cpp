#include "MinionType_2.hpp"

typeRT minionType_2::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionType_2::updateFromTypeRT(typeRT& p_type)
{
}
