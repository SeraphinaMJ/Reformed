/*!***************************************************************************************
\file       RenderContextInterface.hpp
\author     Aaron Damyen
\date       8/10/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the definition of the interface from the engine to the external render context.
*****************************************************************************************/
#pragma once

class transform;

//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <string>

////////==========================================================================////////
////////  Structs                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par struct: renderContextParameters
\brief  A structure holding all of the parameters that the render context can report to
        the engine.
*****************************************************************************************/
struct renderContextParameters
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:
	int width; //!< The current width of the render context
	int height; //!< The current height of the render context

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

/*!***************************************************************************************
\brief this is the structure containing everything the engine needs to pass to the graphicsSystem
		to have it render a gameobject
*****************************************************************************************/
struct renderable
{
	std::string meshID;
	std::string materialID;
	transform* t;
};

struct cameraData
{
	float nearClip;
	float farClip;
	float fovWidth;
	float aspectRatio;
	transform* t;
};

////////==========================================================================////////
////////  Interfaces                                                              ////////
////////==========================================================================////////

/*!***************************************************************************************
\par interface: renderContextInterface
\brief  This is the interface the engine uses to communicate with the external render context
*****************************************************************************************/
class renderContextInterface
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	/*!***************************************************************************************
	\brief  Allows the engine to notify the external render context with a buffer presentFrame is needed.
	*****************************************************************************************/
	virtual void handleSwapBuffersRequest() = 0;
	/*!***************************************************************************************
	\brief  Queries the external render context to identify if any of the parameters have changed.
	\return bool - Returns true if any context parameter has changed, false otherwise.
	*****************************************************************************************/
	virtual bool queryContextChanged() = 0;
	/*!***************************************************************************************
	\brief  Queries the external render context for all context parameters so the engine can
	        adjust itself accordingly.
	\return renderContextParameters - Returns a structure containing all current render 
	        context parameters.
	*****************************************************************************************/
	virtual renderContextParameters queryContextParameters() = 0;
	/*!***************************************************************************************
	\brief the engine may presentFrame out games, and needs to give the new path to the graphics asset 
			managers, so they have a relative path to burrow into their respective asset directories
	\param newPath the path to the new directory that the asset managers will look for their assets
	*****************************************************************************************/
	virtual void updateAssetPath(const std::string& newPath) = 0;
	/*!***************************************************************************************
	\brief tells the graphics system that a gameobject needs to be drawn to the screen, and gives 
			it the data necessary to do so
	\param r contains all the data the combined graphics system and asset managers need to render something
	*****************************************************************************************/
	virtual void draw(const renderable& r) = 0;
	/*!***************************************************************************************
	\brief update the camera and projection matrices used in the MVP transformation
	\param c contains all the pertinent information needed to render objects from the camera's perspective
	*****************************************************************************************/
	virtual void updateCameraMatrices(const cameraData& c) = 0;


	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

};

