/*!***************************************************************************************
\file       ParticleEmitter.cpp
\author     Seraphina Kim
\date       7/31/18
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "ParticleEmitter.hpp"
#include <Vector4Serialization.hpp>
#include <Color4Serialization.hpp>
#include "ShaderProgram.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../SceneManagement/Transform.hpp"

const char* emitterShapeStrs[] = {
	"Torus",
	"Cone",
	"FireWork",
	"Heart",
	"Sphere",
	"Angle_Ring",
	"Circle",
};

const char*const* particleEmitter::getEmitterShapeStr()
{
	return emitterShapeStrs;
}
void particleEmitter::setEmitterUniforms(std::shared_ptr<shaderProgram> p_program)
{

	matrix4x4 emitterModelMatrix = getGameObject().lock()->getComponent<transform>()->getGlobalMatrix();

	p_program->setUniforms("ModelMatrix", emitterModelMatrix);
}

typeRT particleEmitter::toTypeRT() const
{

	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;



	std::vector<typeRT> startColorRange;
	startColorRange.push_back(::toTypeRT(m_data.m_startColRange));
	startColorRange.push_back(::toTypeRT(m_data.m_startColRange2));
	typeRT startColorRanges = typeRT("colorRange", "startColorRange", startColorRange);
	members.push_back(startColorRanges);

	std::vector<typeRT> endColorRange;
	endColorRange.push_back(::toTypeRT(m_data.m_endColRange));
	endColorRange.push_back(::toTypeRT(m_data.m_endColRange2));
	typeRT endColorRanges = typeRT("colorRange", "endColorRange", endColorRange);
	members.push_back(endColorRanges);

	typeRT sizeMin("sizeMin", m_data.m_sizeMin);
	members.push_back(sizeMin);

	typeRT sizeMax("sizeMax", m_data.m_sizeMax);
	members.push_back(sizeMax);

	typeRT lifeMin("lifeMin", m_data.m_minLife);
	members.push_back(lifeMin);

	typeRT lifeMax("lifeMax", m_data.m_maxLife);
	members.push_back(lifeMax);

	
	
	typeRT maxParticles("maxParticles", static_cast<int>(m_data.m_maxParticles));
	members.push_back(maxParticles);

	typeRT batchSize("batchSize", static_cast<int>(m_data.m_batchSize));
	members.push_back(batchSize);

	typeRT batchCount("batchCount", static_cast<int>(m_data.m_batchCount));
	members.push_back(batchCount);

	typeRT interval("interval", static_cast<int>(m_data.m_interval));
	members.push_back(interval);

	typeRT radious("radious", static_cast<int>(m_data.m_radious));
	members.push_back(radious);

	typeRT angle("angle", static_cast<int>(m_data.m_angle));
	members.push_back(angle);

	typeRT arc("arc", static_cast<int>(m_data.m_arc));
	members.push_back(arc);

	typeRT on("on", static_cast<bool>(m_data.m_on));
	members.push_back(on);

	typeRT isTorusRan("isTorusRan", static_cast<bool>(m_data.m_isRandom));
	members.push_back(isTorusRan);

	std::vector<typeRT> shape;
	shape.push_back(typeRT("shapeType", static_cast<int>(m_data.m_shape)));
	typeRT shapeAsType = typeRT("aa_emitterShape", shape);
	members.push_back(shapeAsType);



	for (auto i_member : members)
	{
		result.insertMember(i_member);
	}

  return result;
}

bool particleEmitter::getActiveFlag()
{
	return m_data.m_on;
}

void particleEmitter::setActiveFlag(bool flag)
{
	m_data.m_on = flag;
}

void particleEmitter::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();


	if (l_members.find("startColorRange") != l_members.end())
	{
		typeRT startColorRange = p_type.member("startColorRange");
		std::vector<typeRT> members = startColorRange.array();
		m_data.m_startColRange = ::toColor4(members[0]);
		m_data.m_startColRange2 = ::toColor4(members[1]);
	}
	if (l_members.find("endColorRange") != l_members.end())
	{
		typeRT endColorRange = p_type.member("endColorRange");
		std::vector<typeRT> members = endColorRange.array();
		m_data.m_endColRange = ::toColor4(members[0]);
		m_data.m_endColRange2 = ::toColor4(members[1]);
	}
	if (l_members.find("sizeMin") != l_members.end())
	{
		m_data.m_sizeMin = p_type.member("sizeMin").getFloat();
	}
	
	if (l_members.find("sizeMax") != l_members.end())
	{
		m_data.m_sizeMax = p_type.member("sizeMax").getFloat();
	}
	if (l_members.find("lifeMin") != l_members.end())
	{
		m_data.m_minLife = p_type.member("lifeMin").getFloat();
	}
	if (l_members.find("lifeMax") != l_members.end())
	{
		m_data.m_maxLife = p_type.member("lifeMax").getFloat();
	}
	if (l_members.find("maxParticles") != l_members.end())
	{
		m_data.m_maxParticles = p_type.member("maxParticles").getInt();
	}
	if (l_members.find("batchSize") != l_members.end())
	{
		m_data.m_batchSize = p_type.member("batchSize").getInt();
	}
	if (l_members.find("batchCount") != l_members.end())
	{
		m_data.m_batchCount = p_type.member("batchCount").getInt();
	}
	if (l_members.find("interval") != l_members.end())
	{
		m_data.m_interval = p_type.member("interval").getInt();
	}
	if (l_members.find("radious") != l_members.end())
	{
		m_data.m_radious = p_type.member("radious").getInt();
	}
	if (l_members.find("angle") != l_members.end())
	{
		m_data.m_angle = p_type.member("angle").getInt();
	}
	if (l_members.find("arc") != l_members.end())
	{
		m_data.m_arc = p_type.member("arc").getInt();
	}
	if (l_members.find("on") != l_members.end())
	{
		m_data.m_on = p_type.member("on").getBool();
	}
	if (l_members.find("isTorusRan") != l_members.end())
	{
		m_data.m_isRandom = p_type.member("isTorusRan").getBool();
	}
	if (l_members.find("aa_emitterShape") != l_members.end())
	{
		typeRT shapeAsType = p_type.member("aa_emitterShape");
		std::vector<typeRT> members = shapeAsType.array();
		if (members.size() == 0)
			m_data.m_shape = emitterShape::enm_angleRing;
		else 
			m_data.m_shape = static_cast<emitterShape>(members[0].getInt());
	}
}

particleEmitter::particleEmitter(typeRT& p_type)
{
	typeRT startColorRange = p_type.member("startColorRange");
	std::vector<typeRT> startmembers = startColorRange.array();
	m_data.m_startColRange = ::toColor4(startmembers[0]);
	m_data.m_startColRange2 = ::toColor4(startmembers[1]);

	typeRT endColorRange = p_type.member("endColorRange");
	std::vector<typeRT> endmembers = endColorRange.array();
	m_data.m_endColRange = ::toColor4(endmembers[0]);
	m_data.m_endColRange2 = ::toColor4(endmembers[1]);


	m_data.m_sizeMin = p_type.member("sizeMin").getFloat();
	m_data.m_sizeMax = p_type.member("sizeMax").getFloat();
	m_data.m_minLife = p_type.member("lifeMin").getFloat();
	m_data.m_maxLife = p_type.member("lifeMax").getFloat();

	m_data.m_maxParticles = p_type.member("maxParticles").getInt();
	m_data.m_batchSize = p_type.member("batchSize").getInt();
	m_data.m_batchCount = p_type.member("batchCount").getInt();
	m_data.m_interval = p_type.member("interval").getInt();
	m_data.m_radious = p_type.member("radious").getFloat();
	m_data.m_angle = p_type.member("angle").getFloat();
	m_data.m_arc = p_type.member("arc").getFloat();
	m_data.m_on = p_type.member("on").getBool();
	m_data.m_isRandom = p_type.member("isTorusRan").getBool();


	typeRT shapeAsType = p_type.member("aa_emitterShape");
	std::vector<typeRT> members = shapeAsType.array();
	m_data.m_shape = static_cast<emitterShape>(members[0].getInt());
}
