#if 0
#include "LineMesh.hpp"

lineBuffer & lineMesh::getBuffer() noexcept
{
	return m_cpuSegmentData;
}

lineBuffer const & lineMesh::getBuffer() const noexcept
{
	return m_cpuSegmentData;
}

typeRT lineMesh::toTypeRT() const
{
	typeRT result;

	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	return result;
}

lineMesh::lineMesh()
	: m_cpuSegmentData{ "Buffer McBufferFace" }
{
}

lineMesh::lineMesh(typeRT & p_type)
	: m_cpuSegmentData {"Buffer McBufferFace"}
{
}
#endif