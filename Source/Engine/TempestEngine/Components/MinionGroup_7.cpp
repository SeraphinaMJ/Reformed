#include "MinionGroup_7.hpp"

typeRT minionGroup_7::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_7::updateFromTypeRT(typeRT& p_type)
{
}
