#include "MinionGroup_4.hpp"

typeRT minionGroup_4::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionGroup_4::updateFromTypeRT(typeRT& p_type)
{
}
