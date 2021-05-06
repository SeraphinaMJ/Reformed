#include "MinionGroup_9.hpp"

typeRT minionGroup_9::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_9::updateFromTypeRT(typeRT& p_type)
{
}
