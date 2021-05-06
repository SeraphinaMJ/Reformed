#include "Enemy.hpp"


enemy::enemy(typeRT p_data)
{
}

typeRT enemy::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void enemy::updateFromTypeRT(typeRT& p_type)
{
}
