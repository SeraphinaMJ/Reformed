#include "MinionType_1.hpp"

typeRT minionType_1::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionType_1::updateFromTypeRT(typeRT& p_type)
{
}
