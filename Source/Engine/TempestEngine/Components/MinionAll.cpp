#include "MinionAll.hpp"

typeRT minionAll::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionAll::updateFromTypeRT(typeRT& p_type)
{
}
