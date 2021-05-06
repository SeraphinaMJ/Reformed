/*!**********************************************************************************************************
\file       JumbotronComponent.hpp
\author     Cody Cannell
\date       11/2/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
************************************************************************************************************/
#pragma once
#include <Component.hpp>

//========  1st Party Includes  ===========================================================================//
//========  3rd Party Includes  ===========================================================================//
//========  Types  ========================================================================================//
//========  Defines  ======================================================================================//
//========  Forward Declaration  ==========================================================================//
//========  Enums  ========================================================================================//
//========  Helper Structs  ===============================================================================//

class jumbotronComponent final : public componentCRTP<jumbotronComponent>
{
public:
	//====  Static  =======================================================================================//
	//====  Pure-Virtual  =================================================================================//
	jumbotronComponent(float p_initial_offset = 0);

	jumbotronComponent(typeRT & p_type);

	//virtual ~jumbotronComponent();

	//====  Constructor/Destructor  =======================================================================//
	//====  Accessor Methods  =============================================================================//

	void BeginInstantReplay(float p_replay_duration);

	void OffsetTimeStamp(float p_difference);

	bool IsLive();

	void SetIsLive(bool p_is_live);

	void SetNeedsRecording(bool p_needs_recording);

	int GetID();

	void UpdateReplayDuration(float p_time_difference);

	float GetReplayDuration();

	void SetCameraCapture();

	bool NeedsRecording();

	//====  Virtual/Overrides  ============================================================================//
	typeRT toTypeRT() const override;

	void updateFromTypeRT(typeRT& p_type) override;

	//====  Operators  ====================================================================================//
	//====  Interface Declarations (For each interface Implemented)  ======================================//
	//====  Everything Else  ==============================================================================//

	componentType const type() const override { return jumbotronComponent::getType(); }
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<jumbotronComponent>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "jumbotronComponent", true, true, false); }

	static int m_object_count;

protected:

private:
	//====  Camera ========================================================================================//

	//====  Data  =========================================================================================//

	float m_is_live = true;

	int m_id;

	bool m_needs_recording;

	//==== Timers =========================================================================================//
	float m_replay_duration;

};


// namespace <namespace name>

