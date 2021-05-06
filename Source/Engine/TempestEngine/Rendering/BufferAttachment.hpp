#pragma once

#include <string>

class simpleTexture;
class depthTexture;

struct attachment
{
public:
	attachment(std::shared_ptr<simpleTexture> p_texture, std::string const& p_samplerUniformName)
		: m_texture(p_texture)
		, m_samplerUniformName(p_samplerUniformName) 
	{}

	//! color texture handle
	std::shared_ptr<simpleTexture> m_texture;
	//! name of the sampler in GLSL
	std::string m_samplerUniformName;
};

struct depthAttachment
{
public:
	depthAttachment(std::shared_ptr<depthTexture> m_texture, std::string const& p_samplerUniformName)
		: m_texture{m_texture}
		, m_samplerUniformName{p_samplerUniformName}
	{}

	//! depth texture handle
	std::shared_ptr<depthTexture> m_texture;
	//! name of the sampler in GLSL
	std::string m_samplerUniformName;
};
