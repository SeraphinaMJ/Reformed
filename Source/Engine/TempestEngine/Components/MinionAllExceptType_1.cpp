#include "MinionAllExceptType_1.hpp"

typeRT minionAllExceptType_1::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionAllExceptType_1::updateFromTypeRT(typeRT& p_type)
{
}
