/*!***************************************************************************************
\file       GraphicsSystem.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Holds all the managers and handles rendering for all objects
*****************************************************************************************/

#pragma once

//========1st Party Includes============================================================//
#include "../SceneManagement/GameObject.hpp"
#include "../SystemBase.hpp"
#include "FrameBuffer.hpp"
#include "FrameBufferManager.hpp"
#include "../Platform/PlatformWindowing.hpp"
#include "../Components/RenderTarget.hpp"
#include "CameraBase.hpp"
#include "Vector3.hpp"
#include "LineSegment.hpp"

//========3rd Party Includes============================================================//
#include <vector>
#include <cassert>
#include <queue>
#include <Aspect.hpp>
#include <ComponentTypeSet.hpp>

//========Forward Deceleration==========================================================//
class meshManager;
class lightManager;
class shaderManager;
class TextureManager;
class vertexArrayManager;
class materialManager;
class textureManager;

class camera;
class renderContextInterface;
class externalWindowInterface;

enum class layer
{
	enm_useDepth = 0, //! lines may be occluded by objects and other lines
	enm_layer1,       //! 1st layer (renders on top of only useDepth lines)
	enm_layer2,       //! 2nd layer
	enm_layer3,       //! 3rd layer
	enm_layer4,       //! 4th layer (renders on top of everything except UI)
	enm_LAYER_COUNT   //! bookkeeping
};

enum class renderOutput
{
	enm_forwardPlus = -2,
	enm_forwardOld  = -1,
	enm_normals     = +0,
	enm_depth,
	enm_lightAcc,
	enm_opaque,
	enm_COUNT,
	enm_SSAO,
	enm_shadows,
	enm_bloomHighPass,
	enm_highlight,
	enm_fxaaSobel,
	enm_final,
};


////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: graphicsSystem
\brief holds all the managers and  and handles rendering for all objects
*****************************************************************************************/
class graphicsSystem final : public systemBase
{
	///////=======================================================================////////
	///////   Public                                                              ////////
	///////=======================================================================////////
public:
	friend class graphicsManipulator;
	/*!***************************************************************************************
	\brief  retrieves the name of this system. There are two versions for the same functionality
			so that we can get the name from the class as well as an instance
	\return the name of the s
	*****************************************************************************************/

	static const std::string& getName() { static const std::string n("graphicsSystem"); return n; }
	const std::string& name() const override { return getName(); }

	graphicsSystem(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr)
	{
		assert(p_window_ptr != nullptr && "pointer to externalWindowInterface cannot be nullptr");
		assert(p_render_context_ptr != nullptr && "pointer to renderContextInterface cannot be nullptr");

		m_window_ptr = p_window_ptr;
		m_render_context_ptr = p_render_context_ptr;

	}

	/*!***************************************************************************************
	\brief draws an immediate mode line in world space
	\param begin the first point of the line IN WORLD SPACE
	\param end the last point of the line IN WORLD SPACE
	\param layer ordering in which the line should be drawn. Defaults to red
	\param color color of the line. Defaults to only rendering on top of opaque objects
	*****************************************************************************************/
	void drawLine(
		vector3 const& begin,
		vector3 const& end,
		vector3 const& color = { 1,0,0 },
		float thickness = 0.02f,
		layer layer = layer::enm_layer1
	);

	////==========================================================================////
	////      Pure Virtual                                                         ////
	////==========================================================================////

	/*!*******************************************************************************
	\brief Engine system call to initalize graphics
	*********************************************************************************/
	void onInitialize() override;

	void onStartFrame() override;

	/*!*******************************************************************************
	\brief Engine system call to update graphics
	*********************************************************************************/
	void onUpdate() override;

	void onEndFrame() override;

	/*!*******************************************************************************
	\brief Engine system call to shutdown graphics
	*********************************************************************************/
	void onShutdown() override;

	///============================================================================///
	///       Helper                                                               ///
	///============================================================================///


	//==============================================================================//
	//         Getters & Setters                                                    //
	//==============================================================================//

	/*!*******************************************************************************
	\brief   Gets the shader manager
	\return raw pointer to the shader manager
	*********************************************************************************/
	shaderManager *  getShaderManager() { return m_shaderManager; }

	/*!***************************************************************************************
	\brief  Gets the vertex array manager
	\return raw pointer to the vertex array manager
	*****************************************************************************************/
	vertexArrayManager * getVertexArrayManager() { return m_vertexArrayManager; }

	/*!***************************************************************************************
	\brief  Gets the texture manager
	\return raw pointer to the texture system
	*****************************************************************************************/
	textureManager * getTextureManager() { return m_textureManager; }

	/*!***************************************************************************************
	\brief  Gets the framebuffer manager
	\return raw pointer to the framebuffer system
	*****************************************************************************************/
	framebufferManager * getFramebufferManager() { return m_framebufferManager; }

	/*!***************************************************************************************
	\brief  Written by Alex. Toggles debug line display
	\param  p_show - If debug lines should be shown
	*****************************************************************************************/
	void setDebugLineDraw(bool p_show);

	void reloadShaders();

	static constexpr size_t s_texCount = 13;

	viewportGeometry const& getViewport() const;

	////////======================================================================////////
	////////   Private                                                            ////////
	////////======================================================================////////
private:


	/////============================================================================/////
	/////     Functions / Data                                                       /////
	/////============================================================================/////
	std::vector<std::shared_ptr<gameObject>> getObjectList(GameObjectFiltering::componentTypeSet p_pattern) const;

	std::array<std::queue<lineSegment>,
		static_cast<size_t>(layer::enm_LAYER_COUNT)>
		m_linesToDraw;

	void initGLState();
	void initManagers();
	void initFramebuffers();
	void initShaders();
	void initAssets();

	void shutdownShaders();

	static const std::array<const char *, s_texCount> textureNames;

	/*!***************************************************************************************
	\brief  This is basic rendering sending data through pipline to the screne
	*****************************************************************************************/
	void renderButtons();

	void recordToRenderPool(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera);

	using objectList = std::vector<std::shared_ptr<gameObject>>;

	void renderSkybox(const objectList& p_obj, componentHandle<cameraBase> p_cam);
	void renderOpaque(const objectList& p_obj, const objectList& p_lights, const objectList& p_skybox, componentHandle<cameraBase> p_cam);
		void renderNormals(const objectList& p_obj, componentHandle<cameraBase> p_camera);
		size_t renderShadows(const objectList& p_obj, const objectList& p_lights, componentHandle<cameraBase> p_cam);
		void renderLighting(const objectList& p_lights, componentHandle<cameraBase> p_camera);
		void renderForwardPlus(const objectList& p_obj, componentHandle<cameraBase> p_camera, const objectList& p_lights);
	void renderForward(const objectList& p_obj, componentHandle<cameraBase> p_cam);
	void renderDebug(componentHandle<cameraBase> p_cam);

	bool m_showWireframes{ false };
	void renderDebugWireframes(const objectList& p_physObj, componentHandle<cameraBase> p_camera);
	bool m_showDebugLines{ false };
	void renderDebugLines(const objectList& p_lines, componentHandle<cameraBase> p_camera);
	void renderOld(const objectList & p_listObjects, const objectList & p_lights, componentHandle<cameraBase> p_camera);


	struct renderTextureParams
	{
		vector4 premultiply;
		vector4 postadd;
		std::array<unsigned, 4> channelMapper;
		bool compareTexture;
		viewportGeometry where;
	};

	void renderTextureToScreen(std::shared_ptr<simpleTexture> p_texture, renderTextureParams const& params);

	componentHandle<camera> getCameraByChannel(unsigned p_channel);

	void captureForAllRecordingCameras();


	void renderToScreen(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera);

	std::shared_ptr<framebuffer> renderToBuffer(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera);

	/*!***************************************************************************************
	\brief  The main render loop
	*****************************************************************************************/
	void render(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera);



	void initializeRenderTargets();

	void updateRenderTargets();

	void fpsPrint();

	void checkAndSetWindowDimensionVariables();
	void setCurrentMainCameraMemberVariable();

	GLint getLastAttachedFBO();


	vertexArrayManager *  m_vertexArrayManager = nullptr; //!< pointer to vertex manager
	shaderManager * m_shaderManager = nullptr;            //!< pointer to shader manager
	//materialManager * m_materialManager = nullptr;        //!< pointer to material manager
	textureManager * m_textureManager = nullptr;          //!< pointer to texture manager
	framebufferManager * m_framebufferManager = nullptr;  //!< pointer tp framebuffer manager

	externalWindowInterface * m_window_ptr;
	renderContextInterface * m_render_context_ptr;

	viewportGeometry m_viewport;

	componentHandle<cameraBase> m_mainCamera;

	int m_textureIndex{ -1 };

};










