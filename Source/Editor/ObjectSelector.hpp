/*!***************************************************************************************
\file       ObjectSelectorManipulator.hpp
\author     Seraphina Kim
\date       9/10/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//

//#include "../../Source/Physics.Bullet/BulletWrapper.hpp"
#include "CameraManipulatorInterface.hpp"
#include "../ThirdParty/imgui-docking/imgui.h"
//#include "../../Binaries/Bullet-src/src/BulletCollision/CollisionDispatch/btCollisionObject.h"
//#include "../../Binaries/Bullet-src/src/LinearMath/btVector3.h"
//#include "../../Source/Engine/TempestEngine/Components/RigidBody.hpp"
#include <memory>
#include "EditorLogger.hpp"
#include "BulletWrapper.hpp"
#include <Matrix4x4.hpp>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par class: engineMetaManipulatorInterface
\brief   
*****************************************************************************************/
class objectSelector
{
public:
	objectSelector();
	bool isSelected(ImVec2 winpos);
	bool isCameraAttached() { return !_camera.expired(); }
	void setRayCastCamera(std::weak_ptr<cameraManipulatorInterface> p_camera, Editor::EditorLogger& logger);
	bool mousePickingIntersectionTest(int layer, vector3 & intersectionPoint, vector3  & intersectionNormal);
	
private:
	std::weak_ptr<cameraManipulatorInterface> _camera;
	static Editor::EditorLogger* editorlogger;
	btCollisionObject* m_other;
	
	// 0 = no collision 1 = persistent collision
	bool collisionstate;
	int count;

};