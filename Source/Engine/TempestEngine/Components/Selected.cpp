#include "Selected.hpp"

typeRT selected::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void selected::updateFromTypeRT(typeRT& p_type)
{
}
