#include "Player.hpp"

typeRT player::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void player::updateFromTypeRT(typeRT& p_type)
{
}
