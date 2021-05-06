/*!***************************************************************************************
\file       ParticleSystem.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"
#include "Color.hpp"
#include "ShaderProgram.hpp"
#include "SimpleTexture.hpp"
#include "ParticleMesh.hpp"
#include "ParticleBuffer.hpp"
#include "../Components/Camera.hpp"
#include "../../EngineInterfaces/Public/IComponentRepository.hpp"

#include <random> // std::mt19937

#include "GLTypes.hpp"

enum class emitterShape;
class transform;
struct emitterData;

#define MAXPARTICLES 8000


struct cpu_particle
{
    cpu_particle() = default;
    cpu_particle(vector4 p_position, float p_size, color p_color) : m_position(p_position), m_color(p_color), m_size(p_size) {}
    
    vector4 m_position { 0,0,0,1 };
    vector4 m_velocity{ 0,0,0,0 };
    vector4 m_acceleration = { 0,0,0,0 };

    color m_color{ 1,0,0 };
	color m_startColor{ 0,0,0 };
	color m_endColor{ 0,0,0 };
    float m_size{ 0 };
	float m_t{ 0 };
	float m_dColorT{ 0 };

    
    float m_lifeSpan { 10 };
    float m_life { -0.1f };

    float m_cameraDistance { -1.0f };

    bool operator<(cpu_particle& rhs) {
        // Sort in reverse order : far particles drawn first.
        return this->m_cameraDistance > rhs.m_cameraDistance;
    }
};


class particleSystem final : public systemBase
{
public:

    particleSystem() : m_particlePool(MAXPARTICLES) {}
    
    static const std::string& getName() { static const std::string n("particleSystem"); return n; }
    const std::string& name() const override { return getName(); }

protected:
        
    void onInitialize() override;
    void onUpdate() override;
	void onShutdown() override;
    

private:

    void buildBuffers();
    void updateBuffers();
    void spawnParticles(const emitterData & l_emitterData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);
    void sortParticles();
    void update(float p_dt, const emitterData & p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);
    void setUniforms();

    


    std::shared_ptr<gameObject> getMainCamera();


    int findDeadParticles();

    void createNewParticle(unsigned p_index, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);

	vector4 calculateRandomPositionBasedOnShapeType(std::mt19937 p_rng, const emitterData& e_Data, vector4 &position, vector4 &vel);


	bool isInsideCircle(float p_x, float p_y, float p_z);
	bool isInsideCone(float p_x, float p_y, float p_z);
	bool isInsideTorus(float p_x, float p_y, float p_z);


    

    std::vector<cpu_particle> m_particlePool;

    int m_liveParticle = 0;

  
   
    
    GLuint m_TextureID = 0;
};
