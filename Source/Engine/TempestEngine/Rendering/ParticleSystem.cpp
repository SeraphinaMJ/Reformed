/*!***************************************************************************************
\file       ParticleSystem.cpp
\author     Seraphina Kim
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "../Precompiled.h"
#include "ParticleSystem.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../SystemManager.hpp"
#include "../Components/ParticleEmitter.hpp"
#include "TypeData.hpp"
#include "ShaderProgram.hpp"
#include "GraphicsSystem.hpp"
#include "../Components/Camera.hpp"
#include "MeshBuilder.hpp"
#include "ParticleMesh.hpp"
#include "ParticleRenderer.hpp"
#include "CameraSystem.hpp"
#include "../SceneManagement/Transform.hpp"


void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		// gray
		out_r = out_g = out_b = v;
		return;
	}

	h = fmod(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

float degTorad(float degree)
{
	float Pie = 3.14159265359f;
	Pie = Pie / 180.f;
	degree *= Pie;
	return degree;
}

vector4 getCartisianCoordinate(float theta, float pi, float r)
{
	vector4 pos;
	theta = degTorad(theta);
	pi = degTorad(pi);
	pos.x = cos(theta) * sin(pi) * r;
	pos.z = sin(theta) * sin(pi) * r;
	pos.y = r * cos(pi);
	return pos;
}


vector4 getXYfromThetaOnCircle(float theta, float radius)
{
	vector4 pos;
	theta = degTorad(theta);
	pos.x = cos(theta) * radius;
	pos.z = sin(theta) * radius;
	pos.y = 0;
	pos.w = 1;
	return pos;
}





void particleSystem::onInitialize()
{
  


}

void particleSystem::onUpdate()
{
    GameObjectFiltering::componentTypeSet l_patternEmitter;
    l_patternEmitter.setType(particleEmitter::getType());
    auto l_listEmitters = getSystemManager()->getGameObjectGatherer()->getList(l_patternEmitter);

    /*GLint programId;
    glGetIntegerv(GL_PROGRAM_BINDING_ARB, &programId);*/

    std::shared_ptr<gameObject> l_camera = this->getSystemManager()->getSystem<cameraSystem>()->getMainCameraObject();

    
	//m_shaderProgram->setUniforms("Texture", m_particleTexture->getBoundSlot());


    //set up a particle pool
    for(auto & l_emitterObject : l_listEmitters)
    {
        auto l_emitter = l_emitterObject->getComponent<particleEmitter>();
		auto l_eData = l_emitter->getEmitterData();
	
        auto l_emitterTransform = l_emitterObject->getComponent<transform>();
        auto l_cameraTransform = l_camera->getComponent<transform>();



        // get the buffer for this emitter
        const int bufferID = l_emitter->getBufferID();

		// updates the cpu particles and put fill the buffer object
        update(getDt(), l_emitter->getEmitterData(), l_emitterTransform, l_cameraTransform);

        // get the new data for this emitter and create new particles with it
		if (l_eData.m_on == false)
		{
			//make all particles dead here
			this->getSystemManager()->getSystem<particleRenderer>()->getBuffer()->resetParticleCount();
		}
		else {
			update(getDt(), l_emitter->getEmitterData(), l_emitterTransform, l_cameraTransform);

			// get the new data for this emitter and create new particles with it
			if (l_eData.m_on || (l_eData.m_batchCount == 0 && l_eData.m_currentBatchCount <= l_eData.m_batchCount))
			{
				spawnParticles(l_emitter->getEmitterData(), l_emitterTransform, l_cameraTransform);
			}
		}
        //update gpu buffers with new particle
       // updateBuffers();

    }
}

void particleSystem::onShutdown()
{
	
	for (size_t i = 0; i < m_particlePool.size(); ++i)
	{
		m_particlePool[i].m_life = -1;
	}
	m_liveParticle = 0;

}



void particleSystem::buildBuffers()
{


}

void particleSystem::updateBuffers()
{
	this->getSystemManager()->getSystem<particleRenderer>()->getBuffer()->update();
}

void particleSystem::spawnParticles(const emitterData & p_emitterData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{

    unsigned l_interval = p_emitterData.m_interval;



    if (p_emitterData.m_timeSinceLastInterval < l_interval)
    {
			p_emitterData.m_timeSinceLastInterval++;
    }
    else
    {
        unsigned l_batchSize = p_emitterData.m_batchSize;

		p_emitterData.m_currentBatchSize = 0;
        for (unsigned i = 0; i < l_batchSize; ++i)
        {
          int l_index = findDeadParticles();

		  if (l_index >= 0)
		  {
			  createNewParticle(l_index, p_emitterData, p_emiterTransform, p_cameraTransform);
			  p_emitterData.m_currentBatchSize++;
		  }

        }
		p_emitterData.m_currentBatchCount++;


        sortParticles();
		p_emitterData.m_timeSinceLastInterval = 0;
    }


}

void particleSystem::sortParticles()
{
    std::sort(&m_particlePool[0], &m_particlePool[MAXPARTICLES - 1]);
}



void particleSystem::update(float p_dt,  const emitterData & p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{
    //UNREFERENCED_PARAMETER(p_eData);
    UNREFERENCED_PARAMETER(p_emiterTransform);

	//p_eData.m_radious += p_dt;
   // auto inverseEmitterMatrix = p_emiterTransform->getGlobalMatrix().inverse();
	this->getSystemManager()->getSystem<particleRenderer>()->getBuffer()->resetParticleCount();
    int & l_particlesCount = this->getSystemManager()->getSystem<particleRenderer>()->getBuffer()->getParticleCount();

    for (int i = 0; i < MAXPARTICLES; i++) {

        cpu_particle& p = m_particlePool[i];

        if (p.m_life > 0.0f) {

            // Decrease life
            p.m_life -= p_dt;
            if (p.m_life > 0.0f) {

				

                // Simulate simple physics : gravity only, no collisions
                p.m_acceleration = vector4(0.0f, +9.81f, 0.0f, 0.0f) * p_dt * 0.1f;
                //p.m_position += p.m_acceleration * p_dt;


				// if the particles are local to the emitter
				if (p_eData.m_localParticles) 
				{

					// put the particle in the space of the emitter
					p.m_position = p_emiterTransform->getGlobalMatrix() * p.m_position;

					// update the particle
					p.m_position += p.m_velocity * p_dt;


					p.m_position = p_emiterTransform->getGlobalMatrix().inverse() * p.m_position;
				

				} 
				else
				{
					// eugler integration step
					//p.m_velocity += p.m_acceleration * p_dt;
					p.m_position += p.m_velocity * p_dt;
				}

            
				
			

				// interpolated value =(1-T) * V0 + T * V1;
				float T = p.m_t;
				float cotT = cos(degTorad((1 - T) * 90));
				
				p.m_color.x = (cotT)* p.m_startColor.x + (1.0f - (cotT))*p.m_endColor.x;
				p.m_color.y = (cotT)* p.m_startColor.y + (1.0f - (cotT))*p.m_endColor.y;
				p.m_color.z = (cotT)* p.m_startColor.z + (1.0f - (cotT))*p.m_endColor.z;


				/*p.m_color.x = (T) * p.m_startColor.x + (1.0f - (T))*p.m_endColor.x;
				p.m_color.y = (T) * p.m_startColor.y + (1.0f - (T))*p.m_endColor.y;
				p.m_color.z = (T) * p.m_startColor.z + (1.0f - (T))*p.m_endColor.z;
*/
                vector4 distanceVector = p.m_position - vector4(p_cameraTransform->getGlobalPosition(), 1);
                p.m_cameraDistance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y + distanceVector.z * distanceVector.z);



                //logger("particleSystem").debug() << "Particle: " << i << "Position: " << p.m_position.toStringPtr() << "\n";

                //p.m_position = inverseEmitterMatrix * p.m_position;

                // Fill the GPU buffer
				//color p_startColor = p.m_startColor;
				
				color p_color = p.m_color;
				//ColorConvertHSVtoRGB(p.m_color.x, p.m_color.y, p.m_color.z, p_color.x, p_color.y, p_color.z);

				//changes the color range from start to end 
				p.m_t -= p_dt * p.m_dColorT;
				if (p.m_t < 0.0f)
					p.m_t = 0.0f;
				this->getSystemManager()->getSystem<particleRenderer>()->getBuffer()->addParticle(l_particlesCount, particleMesh::gpu_particle({ p.m_position.x, p.m_position.y, p.m_position.z, p.m_size }, p_color));
            }
            else
			{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.m_cameraDistance = -1.0f;
                m_liveParticle--;
            }

			l_particlesCount++;

        }


    }

    // Debug
    /*for(size_t i = 0; i < m_particlePool.size(); ++i)
    {
        logger("particleSystem").debug() << "Particle: " << i << " Position: " << m_particlePool[i].m_position.toStringPtr() << " Life: " << m_particlePool[i].m_life;
    }*/
}

void particleSystem::setUniforms()
{

}



std::shared_ptr<gameObject> particleSystem::getMainCamera()
{
    GameObjectFiltering::componentTypeSet l_patternCamera;
    l_patternCamera.setType(camera::getType());
    auto l_listCameras = getSystemManager()->getGameObjectGatherer()->getList(l_patternCamera);

    for(auto p_camera : l_listCameras)
    {
        if(p_camera->getName() == "Main Camera")
        {
            return p_camera;
        }
    }

    return nullptr;
}



int particleSystem::findDeadParticles()
{
    for (int i = m_liveParticle; i < MAXPARTICLES; i++) {
        if (m_particlePool[i].m_life < 0) {
            return i;
        }
    }

    for (int i = 0; i < m_liveParticle; i++) {
        if (m_particlePool[i].m_life < 0) {
            return i;
        }
    }


    return -1; // All particles are taken, override the first one
}

void particleSystem::createNewParticle(unsigned p_index, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{
    // get particle
    cpu_particle& p = m_particlePool[p_index];

    auto l_emitter_matrix = p_emiterTransform->getGlobalMatrix();


    // set up random number gereator
    std::mt19937 rng;
    rng.seed(std::random_device()());


	vector4 l_randPos = calculateRandomPositionBasedOnShapeType(rng, p_eData, p.m_position, p.m_velocity);



    p.m_position = l_emitter_matrix * l_randPos;

		if(p_eData.m_sizeMin <= p_eData.m_sizeMax)
		{
			std::uniform_real_distribution<float> randomSize(p_eData.m_sizeMin, p_eData.m_sizeMax);
			p.m_size = randomSize(rng);
		}

		if (p_eData.m_minLife <= p_eData.m_maxLife)
		{
			std::uniform_real_distribution<float> randomLife(p_eData.m_minLife, p_eData.m_maxLife);
			p.m_life = randomLife(rng);
			p.m_dColorT = 1 / p.m_life;
		}

		// 1. pick a start color randomly from the startColorRange in emitterData
		// 2. pick a end color randomly from the endColorRange in emitterData
		// 3. set the t value as 0
		
		vector3 validMinMaxR = vector3(std::min(p_eData.m_startColRange.x, p_eData.m_startColRange2.x), std::max(p_eData.m_startColRange.x, p_eData.m_startColRange2.x), 0);
		vector3 validMinMaxG = vector3(std::min(p_eData.m_startColRange.y, p_eData.m_startColRange2.y), std::max(p_eData.m_startColRange.y, p_eData.m_startColRange2.y), 0);
		vector3 validMinMaxB = vector3(std::min(p_eData.m_startColRange.z, p_eData.m_startColRange2.z), std::max(p_eData.m_startColRange.z, p_eData.m_startColRange2.z), 0);

		std::uniform_real_distribution<float> randomR(validMinMaxR.x, validMinMaxR.y);
		std::uniform_real_distribution<float> randomG(validMinMaxG.x, validMinMaxG.y);
		std::uniform_real_distribution<float> randomB(validMinMaxB.x, validMinMaxB.y);


		p.m_startColor = color(randomR(rng), randomG(rng), randomB(rng), p_eData.m_startColRange.getW());

		vector3 _validMinMaxR = vector3(std::min(p_eData.m_endColRange.x, p_eData.m_endColRange2.x), std::max(p_eData.m_endColRange.x, p_eData.m_endColRange2.x), 0);
		vector3 _validMinMaxG = vector3(std::min(p_eData.m_endColRange.y, p_eData.m_endColRange2.y), std::max(p_eData.m_endColRange.y, p_eData.m_endColRange2.y), 0);
		vector3 _validMinMaxB = vector3(std::min(p_eData.m_endColRange.z, p_eData.m_endColRange2.z), std::max(p_eData.m_endColRange.z, p_eData.m_endColRange2.z), 0);

		std::uniform_real_distribution<float> randomEndR(_validMinMaxR.x, _validMinMaxR.y);
		std::uniform_real_distribution<float> randomEndG(_validMinMaxG.x, _validMinMaxG.y);
		std::uniform_real_distribution<float> randomEndB(_validMinMaxB.x, _validMinMaxB.y);



		p.m_t = 1.0f;

		p.m_endColor = color(randomEndR(rng), randomEndG(rng), randomEndB(rng), p_eData.m_endColRange.getW());



    vector4 distanceVector = p.m_position - vector4(p_cameraTransform->getGlobalPosition(), 1);
    p.m_cameraDistance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y + distanceVector.z * distanceVector.z);
    m_liveParticle++;
}

vector4 particleSystem::calculateRandomPositionBasedOnShapeType(std::mt19937 p_rng, const emitterData& e_Data, vector4 &	position, vector4 &vel)
{

	while(true)
	{
		// find a random location in the Emitter cube
		std::uniform_real_distribution<float> randomX(-1.0f, 1.0f);
		std::uniform_real_distribution<float> randomY(-1.0f, 1.0f);
		std::uniform_real_distribution<float> randomZ(-1.0f, 1.0f);

		float l_x = randomX(p_rng);
		float l_y = randomY(p_rng);
		float l_z = randomZ(p_rng);

		// prune by shape type
		switch (e_Data.m_shape)
		{
			case emitterShape::enm_torus:
			{
				
				float a = 1.0f;
				float angle = 0.f;
				float u = 0.f;

				float speed = 10.f;
				float phi = 45.f;
				float height = (e_Data.m_radious / tan(phi));

		
				if (e_Data.m_isRandom)
				{
					std::uniform_real_distribution<float> randomU(0.f, 360.0f);
					std::uniform_real_distribution<float> randomV(0.f, 360.0f);

					angle = randomV(p_rng);
				    u = randomU(p_rng);

				}
				else
				{
					if (e_Data.m_arc == 0 || e_Data.m_batchSize == 0)
					{
						assert(e_Data.m_arc != 0);
						assert(e_Data.m_batchSize != 0);
						return position;
					}

					float dArc = 360 / e_Data.m_arc;
					u = (e_Data.m_currentBatchCount) * dArc;
					angle = (360.f / e_Data.m_batchSize) * e_Data.m_currentBatchSize;

				}
				
				angle = degTorad(angle);
				u = degTorad(u);
				
				float l_x = (e_Data.m_radious + (a * cos(angle))) * cos(u);
				float l_y = (e_Data.m_radious + (a * cos(angle))) * sin(u);
				float l_z = a*sin(angle);
				
				if (e_Data.m_isRandom)
				{
					float x = e_Data.m_radious * cos(u);
					float y = e_Data.m_radious * sin(u);
					vel.x = x;
					vel.y = height;
					vel.z = y;
					vel.normalized();
					vel *= speed;

				}
				else
				{
					vel.x = 0;
					vel.y = 0;
					vel.z = 0;
				}

				position.x = l_x;
				position.z = l_y;
				position.y = l_z;


				return position;
					//return vector4(l_x, l_y, l_z, 1);
			}break;

			case emitterShape::enm_firework:
			{
				std::uniform_real_distribution<float> randomX(0.f, 360.0f);
				std::uniform_real_distribution<float> randomY(0.f, 180.0f);
				

				float l_x = randomX(p_rng);
				float l_y = randomY(p_rng);
				
				position = { 0,0,0,1 };
				vel = getCartisianCoordinate(l_x, l_y, 3.f);
				vel.w = 0.f;

				vel.normalized();

				float speend = 5.f;
				vel *= speend;

				return position;
				/*if(isInsideCircle(l_x,l_y,l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;*/
			}break;

			case emitterShape::enm_cone:
			{
				float theta = static_cast<float>(rand()%1000)/1000.f; // [0, 0.999]
				theta *= 360;

				float length = static_cast<float>(rand() % 1000) / 1000.f; // [0, 1]
				length *= 10.f;

				float angle = 90- 30.f;
				
				float EPSILON = 0.00001f;

				if (length <= EPSILON)
					break;

				vel = getCartisianCoordinate(theta+180, angle, length);
				vel.normalized();
				float speed = 2;
				vel *= 1.f * speed;

				position = getXYfromThetaOnCircle(theta, length);

				return position;
			}break;

				

			case emitterShape::enm_heart:
			{
				float a = 1.0f;
				float angle = 0.f;
				float u = 0.f;

				float speed = 10.f;
				float phi = 45.f;
				float height = (e_Data.m_radious / tan(phi));


				if (e_Data.m_isRandom)
				{
					std::uniform_real_distribution<float> randomU(0.f, 360.0f);
					std::uniform_real_distribution<float> randomV(0.f, 360.0f);

					angle = randomV(p_rng);
					u = randomU(p_rng);

				}
				else
				{
					if (e_Data.m_arc == 0 || e_Data.m_batchSize == 0)
					{
						assert(e_Data.m_arc != 0);
						assert(e_Data.m_batchSize != 0);
						return position;
					}

					float dArc = 360 / e_Data.m_arc;
					u = (e_Data.m_currentBatchCount) * dArc;
					angle = (360.f / e_Data.m_batchSize) * e_Data.m_currentBatchSize;

				}

				angle = degTorad(angle);
				u = degTorad(u);
				float save = (sin(u)*sin(u)*sin(u));
				float l_x = 16 * save;
				float l_y = 13 * cos(u) - 5 * cos(2 * u) - 2 * (cos(3 * u)) - cos(4 * u);
				float l_z = a * sin(angle);

				if (e_Data.m_isRandom)
				{
					float x = e_Data.m_radious * cos(u);
					float y = e_Data.m_radious * sin(u);
					

					vel.x = 0;
					vel.y = 1;
					vel.z = 0;
					vel.normalized();
					vel *= speed;

				}
				else
				{
					vel.x = 0;
					vel.y = 0;
					vel.z = 0;
				}

				position.x = l_x;
				position.z = l_y;
				position.y = l_z;


				return position;
				/*if (isInsideTorus(l_x, l_y, l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;*/
			}
			case emitterShape::enm_sphere:
			{
				std::uniform_real_distribution<float> randomX(0.f, 360.0f);
				std::uniform_real_distribution<float> randomY(0.f, 360.0f);


				float l_x = randomX(p_rng);
				float l_z = randomY(p_rng);

				position = { 0,0,0,1 };
				vel = getCartisianCoordinate(l_x, l_x, l_z);
				vel.w = 0.f;

				vel.normalized();

				/*float speend = 5.f;
				vel *= speend;*/

				return position;
				/*if(isInsideCircle(l_x,l_y,l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;*/
			}break;

			case emitterShape::enm_angleRing:
			{
				float a = 0.1f;
				float angle = 0.f;
				float u = 0.f;

				float speed = 0.f;
				float phi = 45.f;
				float height = 0.f;//(e_Data.m_radious / tan(phi));


				if (e_Data.m_isRandom)
				{
					std::uniform_real_distribution<float> randomU(0.f, 360.0f);
					std::uniform_real_distribution<float> randomV(0.f, 360.0f);

					angle = randomV(p_rng);
					u = randomU(p_rng);

				}
				else
				{
					if (e_Data.m_arc == 0 || e_Data.m_batchSize == 0)
					{
						assert(e_Data.m_arc != 0);
						assert(e_Data.m_batchSize != 0);
						return position;
					}

					float dArc = 360 / e_Data.m_arc;
					u = (e_Data.m_currentBatchCount) * dArc;
					angle = (360.f / e_Data.m_batchSize) * e_Data.m_currentBatchSize;

				}

				angle = degTorad(angle);
				u = degTorad(u);

				float l_x = (e_Data.m_radious + (a * cos(angle))) * cos(u);
				float l_y = (e_Data.m_radious + (a * cos(angle))) * sin(u);
				float l_z = 0;// a * sin(angle);

				if (e_Data.m_isRandom)
				{
					float x = e_Data.m_radious * cos(u);
					float y = e_Data.m_radious * sin(u);


					vel.x = 0;
					vel.y = 0.1f;
					vel.z = 0;
					vel.normalized();
					//vel *= speed;

				}
				else
				{
					vel.x = 0;
					vel.y = 0;
					vel.z = 0;
				}

				position.x = l_x;
				position.z = l_y;
				position.y = l_z;


				return position;
			}break;
			case emitterShape::enm_circle:
			{
				float theta = static_cast<float>(rand() % 1000) / 1000.f; // [0, 0.999]
				theta *= 360;

				float length = static_cast<float>(rand() % 1000) / 1000.f; // [0, 1]
				length *= 10.f;

				float angle = 90 - 30.f;

				float EPSILON = 0.00001f;

				if (length <= EPSILON)
					break;

				vel.x = 0.f;
				vel.z = 0.f;
				vel.y = 1.f;
				//vel.normalized();
				float speed = 2;
				//vel *= 1.f * speed;

				position = getXYfromThetaOnCircle(theta, length);

				return position;
			}break;
				// TODO: Log a warning, or deal with this properly
			default:
				break;
		}
	}

	return position;
}

bool particleSystem::isInsideCircle(float p_x, float p_y, float p_z)
{
	return (p_x * p_x + p_y * p_y + p_z * p_z) <= 1;
}

bool particleSystem::isInsideCone(float p_x, float p_y, float p_z)
{
		// put y into 0 to 1 range which becomes radius at height y
		float l_r = 1.0f - ((p_y + 1.0f) / 2.0f);



		// is inside radius
		return (p_x* p_x + p_z * p_z) <= l_r;

		
}

bool particleSystem::isInsideTorus(float p_x, float p_y, float p_z)
{
	//(x ^ 2 + y ^ 2 + z ^ 2 - (a ^ 2 + b ^ 2)) ^ 2 - 4 * a*b*(b ^ 2 - z ^ 2)

	// minor radius
	float l_r1 = 1;

	// major radius
	float l_r2 = 2;

	return ((p_x * p_x + p_y + p_y + p_z + p_z) - (l_r1 * l_r1 + l_r2 * l_r2)) *
		((p_x * p_x + p_y + p_y + p_z + p_z) - (l_r1 * l_r1 + l_r2 * l_r2)) - 4.0f * l_r1 * l_r2 * (l_r2 * l_r2 - p_z * p_z) < 0;


}

