#pragma once
#include "DepthTexture.hpp"

class shadowAtlas final : public depthTexture
{
public:
	shadowAtlas(unsigned width, unsigned rows, unsigned cols);


	void setWidth(unsigned width);
	void build() override;

	void setCols(unsigned cols);
	void setRows(unsigned rows);

	unsigned size() const;
	// ill formed?
private:
	unsigned m_size, m_rows, m_cols;
};
