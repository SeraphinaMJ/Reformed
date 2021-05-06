#include "ParticleRenderer.hpp"
#include "ParticleMesh.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../Components/ParticleEmitter.hpp"
#include "ShaderProgram.hpp"
#include "SimpleTexture.hpp"
#include "CameraSystem.hpp"
#include "../../EngineInterfaces/Public/IComponentRepository.hpp"
#include "Camera.hpp"
#include "CameraSystem.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include <regex>

#include <string>
#include <Engine.hpp>

#include "Logger.hpp"

void particleRenderer::onInitialize()
{

	// collect configuration
	std::string l_assetPath = this->getSystemManager()->getConfigurationFunction()().asset_path.generic_string();

	// set up particle shaderPrograme
	m_shaderProgram = std::make_shared<shaderProgram>(assetManager::getAsset(L"shaders/vertex/Particle.vert"),
	assetManager::getAsset(L"shaders/fragment/Particle.frag"));
	m_shaderProgram->build();
	m_shaderProgram->bind();


	// set up textures
	m_particleTexture = std::make_shared<simpleTexture>(assetManager::getAsset(L"textures/Health.png"));
	m_particleTexture->build();
	m_particleTexture->bind(0);

	m_shaderProgram->bind();

	m_shaderProgram->setUniforms("Texture", m_particleTexture->getBoundSlot());//NOT working

	m_buffer = new particleBuffer(MAXPARTICLES, m_mesh->getParticleSize());

	// create particle container
	m_mesh = new particleMesh();


	m_buffer->build(m_mesh);
	m_buffer->bind();
}

void particleRenderer::onUpdate()
{
	m_shaderProgram->bind();

	componentHandle<cameraBase> l_camera = castHandle<cameraBase>(this->getSystemManager()->getSystem<cameraSystem>()->getMainCamera());

	GameObjectFiltering::componentTypeSet l_patternEmitter;
	l_patternEmitter.setType(particleEmitter::getType());
	auto l_listEmitters = getSystemManager()->getGameObjectGatherer()->getList(l_patternEmitter);

	

	for (auto& l_emitterObject : l_listEmitters)
	{
		auto l_emitter = l_emitterObject->getComponent<particleEmitter>();
		l_camera->setCameraUniforms(m_shaderProgram);
		l_emitter->setEmitterUniforms(m_shaderProgram);

		m_buffer->update();

		render();
	}	
	
	
}

void particleRenderer::onShutdown()
{
	
	delete m_buffer;
	delete m_mesh;
}

particleBuffer* particleRenderer::getBuffer()
{
	return m_buffer;
}

void particleRenderer::render()
{
	m_buffer->render();

}