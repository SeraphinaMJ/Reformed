#include "MinionGroup_6.hpp"

typeRT minionGroup_6::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_6::updateFromTypeRT(typeRT& p_type)
{
}
