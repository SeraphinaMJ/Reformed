/*!***************************************************************************************
\file       RenderTarget.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once
#include <Component.hpp>

enum class play_state
{
    enum_feed = 0,
    enum_playback = 1,
    enum_texture = 2,
};
class renderTarget : public componentCRTP<renderTarget>
{
public:

		/*!***************************************************************************************
		\brief  Constructs this object and sets the initial values.
		*****************************************************************************************/
		renderTarget() = default;

    renderTarget(unsigned p_playBackChannel, unsigned p_feedChannel) : m_playBackChannel(p_playBackChannel), m_feedChannel(p_feedChannel){}

		/*!***************************************************************************************
		\brief  Constructs this object and sets the initial values from the typeRT data.
		\param p_type_data - the typeRT data structure with the initial values
		*****************************************************************************************/
		renderTarget(typeRT & p_type_data);

		/*!***************************************************************************************
		\brief  Converts this object into a typeRT structure
		\return typeRT - a typeRT structure with all serializable values for this object
		*****************************************************************************************/
		typeRT toTypeRT() const override;

		/*!***************************************************************************************
		\brief  Updates the values of this object to the values in the typeRT structure
		\param p_type - a typeRT structure with all serializable values for this object
		*****************************************************************************************/
		void updateFromTypeRT(typeRT& p_type) override;
		
    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    componentType const type() const override { return getType(); }

    /*!***************************************************************************************
    \brief  Gets component type
    \return The component type
    *****************************************************************************************/
    static componentType const getType() { static componentType const type = componentType::forConcrete<renderTarget>(); return type; }
	  static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Render Target", true, true, false); }

    unsigned getPlayBackChannel() { return m_playBackChannel; }
    unsigned getFeedChannel() { return m_feedChannel; }
    void changePlayState(play_state p_state) { m_playState = p_state; }
    play_state getPlayState() { return m_playState; }

private:

    unsigned  m_playBackChannel = 0;
    unsigned  m_feedChannel = 0;
    play_state m_playState = play_state::enum_feed;


};
