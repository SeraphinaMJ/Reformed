#include "MinionGroup_10.hpp"

typeRT minionGroup_10::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_10::updateFromTypeRT(typeRT& p_type)
{
}
