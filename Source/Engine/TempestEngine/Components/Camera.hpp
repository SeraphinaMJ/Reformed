/*!***************************************************************************************
\file       Camera.hpp
\author     Cody Cannell
\date       5/17/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      A component class for camera objects
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "../Rendering/FrameBufferManager.hpp"
#include "../Rendering/CameraBase.hpp"

#define IS_DEFFERED true
#define MAIN_CAMERA true
#define MAXRECORDLENGTH 100;


/////////========================================================================/////////
/////////  Class                                                                 /////////
/////////========================================================================/////////


/*!*************************************************************************************
\par class: camera
\brief   Class for managing 3 float values
***************************************************************************************/
class camera : public cameraBase
{

	///////========================================================================///////
	///////   public                                                               ///////
	///////========================================================================///////
public:

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	camera(typeRT& p_type);

	/*!***************************************************************************************
	\brief  default constructor.
	*****************************************************************************************/
	camera() : m_broadcastChannel(numCameras++) { }

	/*!***************************************************************************************
	\brief  Converts this into a type
	\return Type equivalent.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief   Gets the type of the component
	\return  The component type
	*****************************************************************************************/
	componentType const type() const override { return getType(); }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Camera", true, true, false); }

	void updateFromTypeRT(typeRT& p_type) override;

	/*!***************************************************************************************
	\brief  Gets the type of the component
	\return The component type
	*****************************************************************************************/
	static componentType const getType() { static componentType const type = componentType::forConcrete<camera>(); return type; }

	matrix4x4 getInvViewMatrix() override;
	void calcViewMatrix() override;
	void calcProjMatrix() override;
	void calcViewProjMatrix() override;
	void calcCamPosition() override;


	//======================================================================================//
	//        Getters & Setters                                                             //
	//======================================================================================//


	/////========================================================================/////
	/////     Functions                                                          /////
	/////========================================================================/////

	/*!***************************************************************************************
	\brief  Calculates view perjection matrix
	*****************************************************************************************/
	void process() override;

	void processNoTransform() override;

	void setIsMainCamera(bool p_isMain) /*override*/;

	/*!***************************************************************************************
	\brief Sets the active status of this camera
	\param p_active - The new active status.
	*****************************************************************************************/

	void record() { m_isRecording = true; }

	void stopRecording() { m_isRecording = false; }

	void stopPlaying() { m_isPlaying = false; }

	void play() { m_isPlaying = true; }

	bool getIsPlaying() { return m_isPlaying; }

	unsigned getBroadcastCannel() { return m_broadcastChannel; }

	bool isMainCamera() { return m_isMainCamera; }

	bool isDeffered() { return m_isDeffered; }

	bool hasTape() { return m_hasTape; }

	void setHasTap(bool p_hasTap) { m_hasTape = p_hasTap; }

	void setTapIndex(unsigned p_index) { m_tapeIndex = p_index; }

	unsigned getTapeIndex() { return m_tapeIndex; }


	void saveCapture() { m_saveCurruentCapture = true; };

	void resetCaptureFlag() { m_saveCurruentCapture = false; }

	bool getCaptureFlag() { return m_saveCurruentCapture; }

	void setCaptureIndex(unsigned p_index) { m_captureIndex = p_index; }

	unsigned getCaptureIndex() { return m_captureIndex; }

	bool getHasCapture() { return m_hasCapturePool; }

	void setHasCapture(bool m_flag) { m_hasCapturePool = m_flag; }



	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////



private:
	/*!***************************************************************************************
	\brief helper function to calculate view matrix with the correct construction
	\tparam withPosition determines whether to factor in the global position of the camera
	*****************************************************************************************/
	template<bool withPosition>
	void calcViewMatrix();
	/////================================================================================/////
	/////     Data                                                                       /////
	/////================================================================================/////

	bool m_isMainCamera = false;

	bool m_isRecording = false;
	bool m_isPlaying = false;
	bool m_isDeffered = false;

	unsigned m_broadcastChannel;
	unsigned m_tapeIndex = 0;

	bool m_hasTape = false;
	bool m_hasCapturePool = false;
	bool m_saveCurruentCapture = false;

	unsigned m_captureIndex = 0;

};

