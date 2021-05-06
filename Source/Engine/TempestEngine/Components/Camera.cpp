/*!***************************************************************************************
\file       Camera.cpp
\author     Cody Cannell
\date       5/17/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A component class for camera objects
*****************************************************************************************/

//========Self Include==================================================================//
#include "Camera.hpp"
//========1st Party Includes============================================================//
#include "../SceneManagement/Transform.hpp"
#include "../SceneManagement/GameObject.hpp"


//========3rd Party Includes============================================================//
#include <Matrix4x4.hpp>
#include <ColorSerialization.hpp>
#include <Vector3Serialization.hpp>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

	///////========================================================================///////
	///////   Public                                                               ///////
	///////========================================================================///////

		//==============================================================================//
		//        Constructor                                                           //
		//==============================================================================//
camera::camera(typeRT& p_type)
{
	updateFromTypeRT(p_type);
}
//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Operators                                                             //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////



//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////
typeRT camera::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());

	std::vector<typeRT> members;

	typeRT fieldOfView("fieldOfView", static_cast<float>(m_fieldOfView));
	members.push_back(fieldOfView);

	typeRT width("width", static_cast<float>(m_width));
	members.push_back(width);

	typeRT height("height", static_cast<float>(m_height));
	members.push_back(height);

	typeRT nearPlaneDist("nearPlaneDist", static_cast<float>(m_nearPlaneDist));
	members.push_back(nearPlaneDist);

	typeRT farPlaneDist("farPlaneDist", static_cast<float>(m_farPlaneDist));
	members.push_back(farPlaneDist);

	typeRT fogColor = ::toTypeRT(m_fogColor);
	fogColor.setVariableName("fogColor");
	members.push_back(fogColor);

	typeRT viewVec = ::toTypeRT(m_viewVec);
	viewVec.setVariableName("viewVec");
	members.push_back(viewVec);

	typeRT active("active", static_cast<bool>(m_active));
	members.push_back(active);

	typeRT isMainCamera("isMainCamera", static_cast<bool>(m_isMainCamera));
	members.push_back(isMainCamera);

	for (auto i_member : members)
	{
		result.insertMember(i_member);
	}

	return result;

}

void camera::updateFromTypeRT(typeRT& p_type)
{
	auto l_members = p_type.members();

	if (l_members.find("fieldOfView") != l_members.end())
	{
		m_fieldOfView = p_type.member("fieldOfView").getFloat();
	}
	if (l_members.find("width") != l_members.end())
	{
		m_width = p_type.member("width").getFloat();
	}
	if (l_members.find("height") != l_members.end())
	{
		m_height = p_type.member("height").getFloat();
	}
	if (l_members.find("nearPlaneDist") != l_members.end())
	{
		m_nearPlaneDist = p_type.member("nearPlaneDist").getFloat();
	}
	if (l_members.find("farPlaneDist") != l_members.end())
	{
		m_farPlaneDist = p_type.member("farPlaneDist").getFloat();
	}
	if (l_members.find("fogColor") != l_members.end())
	{
		m_fogColor = ::toColor(p_type.member("fogColor"));
	}
	if (l_members.find("viewVec") != l_members.end())
	{
		m_viewVec = ::toVector3(p_type.member("viewVec"));
	}
	if (l_members.find("active") != l_members.end())
	{
		m_active = p_type.member("active").getBool();
	}
	if (l_members.find("isMainCamera") != l_members.end())
	{
		m_isMainCamera = p_type.member("isMainCamera").getBool();
	}

}

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////
template<bool withPosition>
void camera::calcViewMatrix()
{
	auto parent = getGameObject();

	if (parent.expired()) 
	{
		return;
	}

	constexpr auto getInverse = [](auto const& parent)
	{
		auto trans = parent.lock()->template getComponent<transform>();

		if constexpr (withPosition)
		{
			return trans->getGlobalMatrix();
		}
		else
		{
			return trans->getGlobalMatrixNoTransform();
		}
	};

	m_transform = getInverse(parent);

	m_viewMatrix = m_transform.inverse();
}


void camera::process()
{
	calcViewMatrix</*withPosition = */true>();
	calcProjMatrix();
	calcViewProjMatrix();
	calcCamPosition();
}

void camera::processNoTransform()
{
	calcViewMatrix</*withPosition = */false>();
	calcProjMatrix();
	calcViewProjMatrix();
}

void camera::setIsMainCamera(const bool p_isMain)
{
	m_isMainCamera = p_isMain;
}

matrix4x4 camera::getInvViewMatrix()
{
	return m_transform;
}

void camera::calcViewMatrix()
{
	calcViewMatrix</*withPosition = */true>();
}

void camera::calcProjMatrix()
{
	m_projectionMatrix = matrix4x4::computeProjMatrix(m_fieldOfView, (m_width / m_height), m_nearPlaneDist, m_farPlaneDist, false);
}

void camera::calcViewProjMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void camera::calcCamPosition()
{
	m_cameraPosition = getGameObject().lock()->getComponent<transform>()->getGlobalPosition();
}
