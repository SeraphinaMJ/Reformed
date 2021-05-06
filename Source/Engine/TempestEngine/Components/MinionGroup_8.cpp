#include "MinionGroup_8.hpp"

typeRT minionGroup_8::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_8::updateFromTypeRT(typeRT& p_type)
{
}
