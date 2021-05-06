/*!***************************************************************************************
\file       CameraManipulator.hpp
\author     Aaron Damyen
\date       1/18/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the camera manipulator interface
*****************************************************************************************/
#pragma once
#include "../../EngineInterfaces/Public/IComponentRepository.hpp"
#include "CameraManipulatorInterface.hpp"
#include "../Rendering/CameraBase.hpp"


//========Self Include==================================================================//
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
class systemManagerInterface;

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Enum(s) / Struct / Class                                                ////////
////////==========================================================================////////
class editorCamera : public cameraBase
{
public:
	editorCamera();

	void process() override;
	void processNoTransform() override;
	void calcViewMatrix() override;
	void calcProjMatrix() override;
	void calcViewProjMatrix() override;
	matrix4x4 getInvViewMatrix() override;
	void calcCamPosition() override;
	void panCamera(float p_right_amount, float p_up_amount);
	void rotateCamera2D(float p_right_amount, float p_up_amount);
	void transCamera3D(float p_right_amount, float p_forward_amount);

    void setNewPosition(const vector3& newPos, const vector3& positionToLookAt);

	/*!***************************************************************************************
	\brief  Converts this into a type
	\return Type equivalent.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	editorCamera(typeRT& p_type);

	void updateFromTypeRT(typeRT & p_type) override;

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override { return editorCamera::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<editorCamera>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Transform", false, false, false); }
private:
	vector3 m_euler_angles;
};

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class cameraManipulator : public cameraManipulatorInterface
{
public:
	cameraManipulator(systemManagerInterface * p_system_manager);

    virtual ~cameraManipulator() = default;

	void readCameraMatrices(matrixArray1D& p_camera_view_matrix, matrixArray1D& p_camera_projection_matrix) override;

	float getWidth() override;
	float getHeight() override;
	void enableEditorCamera() override;
	void disableEditorCamera() override;

	componentHandle<cameraBase> getMainCamera();

	void panCameraPosition(float p_right_amount, float p_up_amount) override;
	void rotateCamera2DCoords(float p_right_amount, float p_up_amount) override;
	void transCameraXZcoords(float p_right_amount, float p_forward_amount) override;
    void set3DPos(const vector3& newPos, const vector3& positionToLookAt) override;

	/*float getCameraPosX() override;
	float getCameraPosY() override;
	float getCameraPosZ() override;*/

	virtual float getNearDistance() override;
	virtual float getFarDistance() override;


	//std::tuple<float, float, float> getCameraPosition() override;
	//void setCameraPosition(std::tuple<float, float, float> p_new_position) override;
private:
	systemManagerInterface * m_system_manager;

	componentHandle<editorCamera> m_editor_camera = 0;

	// Inherited via cameraManipulatorInterface


	// Inherited via cameraManipulatorInterface



	// Inherited via cameraManipulatorInterface
	virtual float getCameraPosX() override;

	virtual float getCameraPosY() override;

	virtual float getCameraPosZ() override;

	// Inherited via cameraManipulatorInterface
	virtual float getViewX() override;

	virtual float getViewY() override;

	virtual float getViewZ() override;

};
///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      Configurable / Non-Configurable                                             ////
////==================================================================================////

////==================================================================================////
////      Pure-Virtual/Overridden/Virtual/Non-Virtual                                 ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper /Destructor //
//======================================================================================//

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*=======================================================================================/
/         Other                                                                          /
/=======================================================================================*/

// namespace <namespace name>
