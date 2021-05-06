/*!**********************************************************************************************************
\file       ParticleEmitter.hpp
\author     Seraphina Kim
\date       11/10/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			The origin point of the particles 
************************************************************************************************************/
#pragma once

#include <Component.hpp>
#include <Vector4.hpp>
#include <Color4.hpp>


enum class emitterShape
{
	enm_torus = 0,
	enm_cone = 1,
	enm_firework = 2,
	enm_heart = 3,
	enm_sphere = 4,
	enm_angleRing = 5,
	enm_circle = 6,
	enm_max,
};


struct emitterData
{
	emitterShape m_shape = emitterShape::enm_angleRing;

    //std::string m_textureName;
	color4 m_startColRange = { 1,0,0 };
	color4 m_startColRange2 = { 1,1,0 };
	color4 m_endColRange = { 0,0,1 };
	color4 m_endColRange2 = { 0,1,1 };

    unsigned m_maxParticles = 1000;
    unsigned m_batchSize = 3;
    unsigned m_batchCount = 0; // 0 means looping forever
    unsigned m_interval = 6;
    float m_sizeMin = 0.5f;
    float m_sizeMax = 5.0f;
    float m_minLife = 1.f;
    float m_maxLife = 3.f;
	float m_radious = 4.0f;

	float m_angle = 50.0f;
	float m_arc = 180.f;

	bool m_isRandom = true;
	bool m_on = true;
	bool m_localParticles = true;


	mutable unsigned m_timeSinceLastInterval = 0;
	mutable unsigned m_currentBatchCount = 0;
	mutable unsigned m_currentBatchSize = 0;

	vector4 m_lastGlobalPos = { 0,0,0,1 };
	
};

class shaderProgram;

class particleEmitter : public componentCRTP<particleEmitter>
{

public:

	   particleEmitter() = default;


	   static const char* const* getEmitterShapeStr();

    //====  Virtual/Overrides  ============================================================================//

    /*!***************************************************************************************
    \brief  Converts this into a type
    \return Type equivalent.
    *****************************************************************************************/
    typeRT toTypeRT() const override;

    componentType const type() const override { return getType(); }

    void updateFromTypeRT(typeRT& p_type) override;

		/*!***************************************************************************************
		\brief  Converts type to transform.
		\param p_type - type to convert
		*****************************************************************************************/
		particleEmitter(typeRT& p_type);

    static componentType const getType() { static componentType const type = componentType::forConcrete<particleEmitter>(); return type; }
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Particle Emitter", true, true, true); }

		void setEmitterUniforms(std::shared_ptr<shaderProgram> p_program);

    //setters for particle data
    void setMaxParticles(unsigned p_maxParticles) { m_data.m_maxParticles = p_maxParticles;  m_isDirty = true; }

    const emitterData & getEmitterData() const { return m_data; }


    int getBufferID() { return m_bufferID; };
    void setBefferID(int p_bufferID) { m_bufferID = p_bufferID; }

    void setDirtyFlag(bool p_flag) { m_isDirty = p_flag; }
    bool getDirtyFlag() { return m_isDirty; }

	bool getActiveFlag();
	void setActiveFlag(bool flag);

private:

	
   
    emitterData m_data; 
    bool m_isDirty = true;

    int m_bufferID = 0;
};
