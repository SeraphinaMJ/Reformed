/*!***************************************************************************************
\file       LineMesh.cpp
\author     Patrick Cook
\date       9/16/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      method impl. for LineMesh.hpp
*****************************************************************************************/
#include "LineBuffer.hpp"

#include "LineSegment.hpp"
#include "VertexArrayObject.hpp"
#include "Mesh.hpp"

#include <GL/glew.h>

lineBuffer::lineBuffer(std::vector<lineSegment>&& p_segments)
	: m_segments{std::move(p_segments)}
	, m_dirty{true}
{
}

lineBuffer::lineBuffer(std::vector<lineSegment> const & p_segments)
	: m_segments{p_segments}
	, m_dirty{true}
{
}

void lineBuffer::reserve(size_t p_count)
{
	m_segments.reserve(p_count);
}

void lineBuffer::resize(size_t p_count, lineSegment const & val)
{
	m_segments.resize(p_count, val);
	m_dirty = true;
}

lineSegment & lineBuffer::operator[](size_t idx)
{
	m_dirty = true;
	return m_segments[idx];
}

lineSegment const & lineBuffer::operator[](size_t idx) const
{
	return m_segments[idx];
}

void lineBuffer::add_segment(lineSegment const & p_segment)
{
	m_segments.push_back(p_segment);
	m_dirty = true;
}

void lineBuffer::add_segment(lineSegment && p_segment)
{
	m_segments.push_back(std::move(p_segment));
	m_dirty = true;
}

void lineBuffer::add_segments(std::vector<lineSegment> const & p_segments)
{
	add_segments(p_segments, p_segments.size());
	m_dirty = true;
}

void lineBuffer::clear() noexcept
{
	m_segments.clear();
	m_dirty = true;
}

size_t lineBuffer::size() const noexcept
{
	return count();
}

size_t lineBuffer::count() const noexcept
{
	return m_segments.size();
}

void lineBuffer::unsetDirty() const noexcept
{
	m_dirty = false;
}

std::string const & lineBuffer::getAssetHandle() const
{
	return m_vaoAssetHandle;
}

void lineBuffer::setAssetHandle(std::string const & p_handle)
{
	if (p_handle != m_vaoAssetHandle)
	{
		m_vaoAssetHandle = p_handle;
		m_assetChanged = true;
	}
}

bool lineBuffer::isDirty() const noexcept
{
	return m_dirty;
}

lineSegment const * lineBuffer::data() const noexcept
{
	return m_segments.data();
}

std::vector<lineSegment> const & lineBuffer::peekBuffer() const noexcept
{
	return m_segments;
}

std::vector<lineSegment>& lineBuffer::getBuffer() noexcept
{
	m_dirty = true;
	return m_segments;
}

size_t lineBuffer::getBufferSize() const
{
	return m_segments.capacity() * sizeof(lineSegment);
}

void lineBuffer::build()
{
	glGenVertexArrays(1, &m_vaoHandle);
	glBindVertexArray(m_vaoHandle);

	// line data starting from 2 (vPosition = 0, vNormal = 1, rest = following attribs)
	for (unsigned i = 0; i != 6; ++i)
		glEnableVertexAttribArray(i);
	// instancing setup
	for (unsigned i = 0; i != 4; ++i)
		glVertexAttribDivisor(i + 2, 1);

	// interleaved buffer
	glGenBuffers(1, &m_vboHandle);
	bind();

	// preallocate 16 segments
	// this buffer may be updated every frame, so mark it as such
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineSegment) * 16, nullptr, GL_STREAM_DRAW);

	char * offset = 0;

	glVertexAttribPointer(
		2, // segment begin
		3, // X, Y, Z positions
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(lineSegment), // stride
		offset // array buffer offset
	);

	offset += sizeof(vector3);

	glVertexAttribPointer(
		3, // segment end
		3, // X, Y, Z positions
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(lineSegment), // stride
		offset // array buffer offset
	);

	offset += sizeof(vector3);

	glVertexAttribPointer(
		4, // segment color
		3, // R, G, B color
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(lineSegment), // stride
		offset // array buffer offset
	);

	offset += sizeof(vector3);

	glVertexAttribPointer(
		5, // segment width
		1, // scalar
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(lineSegment), // stride
		offset // array buffer offset
	);

	offset += sizeof(float);

	glBindVertexArray(0);
}

void lineBuffer::update()
{
	if (!m_built)
	{
		build();
		m_built = true;
	}

	if (m_dirty)
	{
		bind();
		glBufferData(GL_ARRAY_BUFFER, getBufferSize(), nullptr, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lineSegment) * count(), data());
		m_dirty = false;
		unbind();
	}
}

void lineBuffer::render(const std::shared_ptr<vertexArrayObject> p_modelVAO)
{
	auto topology = [&p_modelVAO]()-> int
	{
		switch (p_modelVAO->getIndexBufferObject().getTopology())
		{
		case topology::enm_lines:
			return GL_LINES;
		case topology::enm_triangle:
			return GL_TRIANGLES;
		default: return -1;
		}
	}();

	if (topology == -1) return;

	mesh const* l_mesh = p_modelVAO->lastBuilt();

	if (!l_mesh) return;

	glBindVertexArray(m_vaoHandle); 

	// TODO: check if this was the last VBO/IBO bound and skip
	p_modelVAO->getVertexBufferObject().bind();
	p_modelVAO->getIndexBufferObject().bind();

	size_t l_attributeCount = std::min(size_t(2), l_mesh->getAttributeCount());
	size_t const l_vertexSize = l_mesh->getVertexSize();

	for (size_t l_offset = 0, i = 0; i < l_attributeCount; l_offset += l_mesh->getAttributeElementSize()[i], ++i)
	{
		glVertexAttribPointer(i,
			static_cast<GLint>(l_mesh->getAttributeElementCounts()[i]), GL_FLOAT, GL_FALSE,
			static_cast<GLuint>(l_vertexSize), reinterpret_cast<GLvoid *>(l_offset));
	}

	glDrawElementsInstanced(topology, p_modelVAO->getIndexBufferObject().getIndexCount(), GL_UNSIGNED_INT, nullptr, count());

	glBindVertexArray(0);
}

void lineBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboHandle);
}

void lineBuffer::unbind() const
{
}

void lineBuffer::shutdown()
{
	glDeleteBuffers(1, &m_vboHandle);
	glDeleteVertexArrays(1, &m_vaoHandle);
}
