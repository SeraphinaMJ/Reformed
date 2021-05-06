/*!***************************************************************************************
\file       CameraManipulatorInterface.hpp
\author     Aaron Damyen
\date       1/17/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the interface for a camera manipulator in the engine
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include "ManipulatorTypedefs.hpp"
#include <Vector3.hpp>
//========3rd Party Includes============================================================//
#include <tuple>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class cameraManipulatorInterface
{
public:

	virtual void readCameraMatrices(matrixArray1D& p_camera_view_matrix, matrixArray1D& p_camera_projection_matrix) = 0;

	virtual float getWidth() = 0;
	virtual float getHeight() = 0;

	/*!***************************************************************************************
	\brief  This just for giving an example of a function format.
	\param exampleIn - dat old boring int
	\return What is this returning?
	*****************************************************************************************/
	virtual void enableEditorCamera() = 0;
	virtual void disableEditorCamera() = 0;

	virtual void panCameraPosition(float p_right_amount, float p_up_amount) = 0;
	virtual void rotateCamera2DCoords(float p_right_amount, float p_up_amount) = 0;
	virtual void transCameraXZcoords(float p_right_amount, float p_forward_amount) = 0;
    virtual void set3DPos(const vector3& newPos, const vector3& positionToLookAt) = 0;

	virtual float getCameraPosX() = 0;
	virtual float getCameraPosY() = 0;
	virtual float getCameraPosZ() = 0;


	virtual float getNearDistance() = 0;
	virtual float getFarDistance() = 0;

	virtual float getViewX() = 0;
	virtual float getViewY() = 0;
	virtual float getViewZ() = 0;

	
	

	//virtual std::tuple<float /*x*/, float /*y*/, float /*z*/> getCameraPosition() = 0;
	//virtual std::tuple<float /*x_rot*/, float /*y_rot*/, float /*z_rot*/> getCameraRotation() = 0;

	//virtual void setCameraPosition(std::tuple<float /*x*/, float /*y*/, float /*z*/> p_new_position) = 0;
	//virtual void setCameraRotation(std::tuple<float /*x_rot*/, float /*y_rot*/, float /*z_rot*/> p_new_rotation) = 0;
};


