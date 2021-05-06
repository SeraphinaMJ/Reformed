#include "ShadowAtlas.hpp"

#include <GL/glew.h>

shadowAtlas::shadowAtlas(const unsigned width, const unsigned rows, const unsigned cols)
	: depthTexture{width * cols, width * rows}
	, m_size{width}
	, m_rows{rows}
	, m_cols{cols}
{
}

void shadowAtlas::setWidth(const unsigned width)
{
	m_size = width;

	m_width  = m_cols * m_size;
	m_height = m_rows * m_size;
}

void shadowAtlas::setCols(const unsigned cols)
{
	m_cols = cols;
	m_width = m_cols * m_size;
}

void shadowAtlas::setRows(const unsigned rows)
{
	m_rows = rows;
	m_height = m_rows * m_size;
}

unsigned shadowAtlas::size() const
{
	return m_size;
}

void shadowAtlas::build()
{
	glGenTextures(1, &m_depthHandle);
	bind(0);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 
		m_width, m_height, 0,
		GL_DEPTH_COMPONENT,
		GL_UNSIGNED_INT, nullptr
	);
	
	// for PCF
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// max sure that sampling outside of the texture returns the maximum possible depth value
	constexpr std::array border{ 1.f, 1.f, 1.f, 1.f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	// This is to allow usage of textureProj function in the shader
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	unbind();
}
