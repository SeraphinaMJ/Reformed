/*!***************************************************************************************
\file       SystemMonitor.hpp
\author     Aaron Damyen
\date       3/15/2019
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a monitoring system for the system manager
*****************************************************************************************/
#pragma once

//======== 1st Party Includes ==========================================================//
#include "../SystemBase.hpp"
//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//

/*!***************************************************************************************
\par class: classExample
\brief   This is a stub class to be used as a template / example for class formating.
*****************************************************************************************/
class systemMonitor : public systemBase
{
public:
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
		so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/
	static const std::string& getName() { static const std::string n("__SystemMonitoring"); return n; }
	const std::string& name() const override { return getName(); }

	/*!***************************************************************************************
	\brief  Sets the visibility status of the Frames Per Second metric
	\param p_visible - whether or not to show the frames per second metric
	*****************************************************************************************/
	void showFPS(bool p_visible);

	/*!***************************************************************************************
	\brief  Sets the visibility status of the object counts metric
	\param p_visible - whether or not to show the object counts metric
	*****************************************************************************************/
	void showCounts(bool p_visible);
	
protected:

#if _DEBUG
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
		so that we can get the name from the class as well as an instance.  Only available in DEBUG mode.
	\return the name of the s
	*****************************************************************************************/
	void onEndFrame() override;
#endif 

private:
	bool m_show_fps = false;
	bool m_show_counts = false;
};

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

// namespace <namespace name>

