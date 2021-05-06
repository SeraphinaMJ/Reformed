#include "MinionLeader.hpp"

typeRT minionLeader::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	return result;
}

void minionLeader::updateFromTypeRT(typeRT& p_type)
{
}
