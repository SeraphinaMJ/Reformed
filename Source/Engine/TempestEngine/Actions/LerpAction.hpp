/*!***************************************************************************************
\file       LerpAction.hpp
\author     Aaron Damyen
\date       6/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Defines a class which can linearly interpolate a value using the action system.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "Action.hpp"

#include <functional>

//========3rd Party Includes============================================================//

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: lerpAction
\brief   Linearly interpolates a field between two values using the action system.
*****************************************************************************************/
template< typename T >
class lerpAction : public action
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Constructs a LERP action and initializes it.
	\param p_handle - This actions handle (assigned by the actionSystem).
	\param p_start_value - The starting value of the action.
	\param p_end_value - The ending value of the action.
	\param p_duration - The total duration the action should be in effect for.
	\param p_update_function - The function to call with the new value every update event.
	*****************************************************************************************/
	lerpAction(actionHandle p_handle,
	    T const & p_start_value,
	    T const & p_end_value,
	    float p_duration,
	    std::function<void(T const & value)> p_update_function);

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Protected                                                            ////////
	////////========================================================================////////
protected:

	/*!***************************************************************************************
	\brief  Notifies the action of an update event.
	\param p_dt - The amount of time since the last update event.
	*****************************************************************************************/
	void onUpdate(float p_dt) override;

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	T m_start_value; //!< The initial value for the interpolation
	T m_end_value; //!< The final value for the interpolation
	float m_duration; //!< The duration for the interpolation
	float m_dt_remaining; //!< The amount of time remaining for the interpolation
	std::function<void(T const & value)> m_updateFunction; //!< The function to call each update frame.

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};


template <typename T>
lerpAction<T>::lerpAction(actionHandle p_handle, T const& p_start_value, T const& p_end_value, float p_duration,
    std::function<void(T const& value)> p_update_function)
    : action(p_handle), m_start_value{p_start_value}, m_end_value{p_end_value}, m_duration{p_duration},
        m_dt_remaining{p_duration}, m_updateFunction{p_update_function}
{ }

template <typename T>
void lerpAction<T>::onUpdate(float p_dt)
{
    m_dt_remaining -= p_dt;
    if (m_dt_remaining <= 0.0f)
        complete();
    else
    {
        if(m_updateFunction != nullptr)
        {
            // since m_dt_remaining starts at m_duration and decreases to 0,
            //   have to calculate a 'reverse' t value then perform subtraction to get t.
            float l_revt = m_dt_remaining / m_duration;
            float l_t = 1.0f - l_revt;
            // call the supplied update function with the new value
            m_updateFunction(m_start_value * l_t + m_end_value * l_revt);
        }
    }
}


