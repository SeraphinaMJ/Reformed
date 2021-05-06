#include "MinionGroup_1.hpp"

typeRT minionGroup_1::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_1::updateFromTypeRT(typeRT& p_type)
{
}
