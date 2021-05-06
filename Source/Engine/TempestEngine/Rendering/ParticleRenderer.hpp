#pragma once

#include "../SystemBase.hpp"
#include "ParticleBuffer.hpp"


#define MAXPARTICLES 8000

class shaderProgram;
class simpleTexture;

class particleRenderer final : public systemBase
{
public:

	

	static const std::string& getName() { static const std::string n("particleRenderer"); return n; }
	const std::string& name() const override { return getName(); }

	particleBuffer * getBuffer();

	void render();

protected:

	
	void onInitialize() override;
	void onUpdate() override;
	void onShutdown() override;

	




private:

	particleBuffer* m_buffer = nullptr;

	particleMesh* m_mesh = nullptr;

	std::shared_ptr<shaderProgram> m_shaderProgram;

	std::shared_ptr<simpleTexture> m_particleTexture;


};



