#include "MinionGroup_5.hpp"

typeRT minionGroup_5::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_5::updateFromTypeRT(typeRT& p_type)
{
}
