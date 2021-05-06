#include "MinionGroup_2.hpp"

typeRT minionGroup_2::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_2::updateFromTypeRT(typeRT& p_type)
{
}
