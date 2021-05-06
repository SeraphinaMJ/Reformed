/*!***************************************************************************************
\file       GraphicsSystem.cpp
\author     Cannell, Cody & Cook, Patrick
\date       9/27/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Holds all the managers and handles rendering for all objects
*****************************************************************************************/
#include "Precompiled.h"
//========Self Include==================================================================//
#include "GraphicsSystem.hpp"
//========1st Party Includes============================================================//
#include "SystemManager.hpp"

#include "GameObjectFilter.hpp"
#include "GameObject.hpp"

#include "MaterialManager.hpp"
#include "ShaderManager.hpp"

#include "TextureManager.hpp"
#include "Texture.hpp"

#include "VertexArrayManager.hpp"
#include "VertexArrayObject.hpp"

#include "CameraSystem.hpp"
#include "Camera.hpp"

#include "DebugLines.hpp"
#include "Debug.hpp"
#include "GLDebug.hpp"
#include "MeshDebug.hpp"

#include "RigidBody.hpp"
#include "Transform.hpp"

#include "UISprite.hpp"
#include "Light.hpp"
#include "MeshBuilder.hpp"
#include "RenderTarget.hpp"
#include "Renderer.hpp"
#include "Skybox.hpp"
#include "TextRenderer.hpp"
#include "ShadowAtlas.hpp"

#include "ModularFrameBuffer.hpp"
#include "MaterialLoader.hpp"
#include "AssetManager.hpp"

//========3rd Party Includes============================================================//
#include <RenderContextInterface.hpp>
#include <iomanip>
#include <Enumerate.hpp>
#include <BitCast.hpp>
#include <Aspect.hpp>

#include <GL/glew.h>
#include <Shapes.hpp>

struct [[nodiscard]] restoreViewport {
	std::array<int, 4> m_dim;

	static auto getViewport()
	{
		std::array<int, 4> l_dim;
		glGetIntegerv(GL_VIEWPORT, l_dim.data());
		return l_dim;
	}

	restoreViewport() : m_dim{ getViewport() } { }

	restoreViewport(const viewportGeometry v) : m_dim{ getViewport() } {
		glViewport(v.x, v.y, v.width, v.height);
	}

	~restoreViewport() {
		const auto& [x, y, width, height] = m_dim;
		glViewport(x, y, width, height);
	}
};


const std::array<const char *,graphicsSystem::s_texCount>
graphicsSystem::textureNames =
{
	"gWorldNormal+Shininess",
	"depthStencil",
	"diffuseSSAO",
	"specular",
	"ambient",
	"shadowAtlas",
	"shadowMap",
	"forwardColorOutput",
	"bloom1","bloom2","bloom3","bloom4",
	"maskTexture"
};

const std::array<vector4, graphicsSystem::s_texCount> premultiplies
{
	vector4{vector3{0.5f}, 1.f}, // normals, shininess
	vector4{1.f},  // depth stencil
	vector4{1.f},  // diffuse, ssao
	vector4{1.f},  // specular
	vector4{1.f},  // ambient
	vector4{1.f, 0.f,0.f,0.f}, // shadow atlas
	vector4{1.f},  // shadow map
	vector4{1.f},  // color output
	vector4{1.f}, vector4{1.f}, vector4{1.f}, vector4{1.f}, // bloom
	vector4{1.f},  // mask texture
};

const std::array<vector4, graphicsSystem::s_texCount> postadds
{
	vector4{vector3{0.5f}, 0.f}, // normals, shininess
	vector4{0.f},  // depth stencil
	vector4{0.f},  // diffuse, ssao
	vector4{0.f},  // specular
	vector4{0.f},  // ambient
	vector4{0.f}, // shadow atlas
	vector4{0.f},  // shadow map
	vector4{0.f},  // color output
	vector4{0.f}, vector4{0.f}, vector4{0.f}, vector4{0.f}, // bloom
	vector4{0.f},  // mask texture
};

const std::array<std::array<unsigned, 4>, graphicsSystem::s_texCount> channelmappings
{ {
	{0,1,2,3}, // normals, shininess
	{0,0,0,3}, // depth, stencil
	{0,1,2,3}, // diffuse, ssao
	{0,1,2,3}, // specular
	{0,1,2,3}, // ambient
	{0,0,0,0}, // shadow atlas
	{0,1,2,3}, // shadow map
	{0,1,2,3}, // color output
	{0,1,2,3}, {0,1,2,3}, {0,1,2,3}, {0,1,2,3}, // bloom
	{0,0,0,0}, // mask texture
} };

enum textureNames
{
	gWorldNormals=0,
	depthStencil,
	diffuseSSAO,
	specular,
	ambient,
	ShadowAtlas,
	shadowMap,
	forwardColor,
	bloom1,bloom2,bloom3,bloom4,
	maskTexture,
};

void graphicsSystem::onInitialize()
{
	initGLState();

	textRenderer::initialize();

	initManagers();
	initShaders();
	initFramebuffers();
	initAssets();
}

void graphicsSystem::setDebugLineDraw(bool p_show)
{
	m_showDebugLines = p_show;
}


void graphicsSystem::reloadShaders()
{
	initShaders();
}

std::vector<std::shared_ptr<gameObject>> 
graphicsSystem::getObjectList(const GameObjectFiltering::componentTypeSet p_pattern) const
{
	return getSystemManager()->getGameObjectGatherer()->getList(p_pattern);
}



void graphicsSystem::initGLState()
{
	#ifdef _DEBUG
		#define DEBUGGIN_GRAPHICS
	#endif
	#ifdef DEBUGGIN_GRAPHICS
		GLDebug::InitializeDebugCallback();
	#endif

	//  renderContextParameters renderParams = m_render_context_ptr->queryContextParameters();

	const auto [width, height] = m_render_context_ptr->queryContextParameters();

	m_viewport = maintainAspect({ width, height }, { 16, 9 });

	glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);

	// assetManager = std::make_shared<AssetManager>();

	auto l_configuration = this->getSystemManager()->getConfigurationFunction();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_BLEND);
}

void graphicsSystem::initManagers()
{
	m_vertexArrayManager = new vertexArrayManager();
	m_shaderManager = new shaderManager();
	m_textureManager = new textureManager();
	m_framebufferManager = new framebufferManager();
}

void graphicsSystem::initFramebuffers()
{
	const auto l_lastFBO = getLastAttachedFBO();
	auto fbMan = getFramebufferManager();
	//std::shared_ptr<framebuffer> l_buffer = fbMan->createFramebuffer(framebufferType::CaptureBuffer, 1024, 768);

	// TODO: maintain internal resolution
	constexpr unsigned l_width = 1920;
	constexpr unsigned l_height = 1080;

	constexpr unsigned l_shadowDim = 1024;
	constexpr unsigned l_shadowRows = 1;
	constexpr unsigned l_shadowCols = 4;

	auto tMan = getTextureManager();

	std::array<std::shared_ptr<texture>, s_texCount> l_textures;
	auto&&[l_gNormals, l_depthBuffer,
		l_lightAcc, l_specularAcc, l_ambientAcc,
		l_shadowAtlas, l_shadowMap, l_colorOutput,
		l_bloom0, l_bloom1, l_bloom2, l_bloom3,
		l_maskTexture] = l_textures;

	l_gNormals = tMan->createTexture<simpleTexture>(textureNames[gWorldNormals],
		l_width, l_height, format::enm_rgba_float);

	l_depthBuffer = tMan->createTexture<depthTexture>(textureNames[depthStencil],
		l_width, l_height);

	l_lightAcc = tMan->createTexture<simpleTexture>(textureNames[diffuseSSAO],
		l_width, l_height, format::enm_rgba);

	l_specularAcc = tMan->createTexture<simpleTexture>(textureNames[specular],
		l_width, l_height, format::enm_rgb);

	l_ambientAcc = tMan->createTexture<simpleTexture>(textureNames[ambient],
		l_width, l_height, format::enm_rgb);

	l_shadowAtlas = tMan->createTexture<shadowAtlas>(textureNames[ShadowAtlas], l_shadowDim, l_shadowRows, l_shadowCols);

	l_shadowMap = tMan->createTexture<simpleTexture>(textureNames[shadowMap],
		l_width, l_height, format::enm_rgba);

	l_colorOutput = tMan->createTexture<simpleTexture>(textureNames[forwardColor],
		l_width, l_height, format::enm_rgba);

	for (size_t i = 0; i != 4; ++i)
	{
		l_textures[i + bloom1] = tMan->createTexture<simpleTexture>(textureNames[i + bloom1],
			l_width / (1 << i), l_height / (1 << i), format::enm_rgb);
	}

	l_maskTexture = tMan->createTexture<simpleTexture>(textureNames[maskTexture],
		l_width, l_height, format::enm_r);

	for (auto&& l_tex : l_textures)
	{
		l_tex->build();
	}

	auto l_gBuffer = fbMan->createModFramebuffer(framebufferType::DeferredGBuffer, l_width, l_height);
	l_gBuffer->build();
	l_gBuffer->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_gNormals),
		"gNormal"),
		0
	);

	l_gBuffer->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);
	l_gBuffer->reconfigureDrawBuffers();

	l_gBuffer->setClearColor(color{ 0,0,0,0 });
	l_gBuffer->clear();

	auto l_lightAccBuf = fbMan->createModFramebuffer(framebufferType::LightAccBuf, l_width, l_height);
	l_lightAccBuf->build();
	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_lightAcc),
		"lightAcc"),
		0
	);

	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_specularAcc),
		"specAcc"),
		1
	);

	l_lightAccBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_ambientAcc),
		"ambAcc"),
		2
	);

	l_lightAccBuf->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);
	l_lightAccBuf->reconfigureDrawBuffers();

	l_lightAccBuf->setClearColor(color{ 0, 0, 0, 1 });
	l_lightAccBuf->clear();

	auto l_forwardOutput = fbMan->createModFramebuffer(framebufferType::DeferredOutput, l_width, l_height);
	l_forwardOutput->build();
	l_forwardOutput->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_colorOutput),
		"colorOutput"),
		0
	);

	l_forwardOutput->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);

	auto l_shadowBuffer = fbMan->createModFramebuffer(framebufferType::ShadowBuffer, 1024, 1024);
	l_shadowBuffer->build();
	l_shadowBuffer->setDepthAttachment(depthAttachment{
		std::static_pointer_cast<depthTexture>(l_shadowAtlas),
		"shadowAtlas"
	});

	auto l_shadowMapBuf = fbMan->createModFramebuffer(framebufferType::ShadowMap, l_width, l_height);
	l_shadowMapBuf->build();
	l_shadowMapBuf->addColorAttachment(attachment(
		std::static_pointer_cast<simpleTexture>(l_shadowMap),
		"colorOutput"),
		0
	);
	l_shadowMapBuf->setDepthAttachment(depthAttachment(
		std::static_pointer_cast<depthTexture>(l_depthBuffer),
		"depth")
	);

	l_shadowMapBuf->setClearColor(color(1, 1, 1, 1));

	glBindFramebuffer(GL_FRAMEBUFFER, l_lastFBO);
	if(!l_lastFBO) glDrawBuffer(GL_BACK_LEFT);
	else {
		constexpr GLenum att = GL_COLOR_ATTACHMENT0;
		glDrawBuffers(1, &att);
	}
	m_textureManager->UnbindAll();
}

void graphicsSystem::initShaders()
{
	using fsPath = std::filesystem::path;
	auto createShader =
		[man = getShaderManager()](
			programType type, fsPath const& vert, fsPath const& frag)
		->std::shared_ptr<shaderProgram>
	{
		return man->createShaderProgram(
			type, assetManager::getAsset(vert), assetManager::getAsset(frag));
	};
	const auto l_shadersDir = fsPath{ "shaders" };
	const auto l_vertexDir = l_shadersDir / "vertex";
	const auto l_fragmentDir = l_shadersDir / "fragment";

	std::array<std::shared_ptr<shaderProgram>, 13> l_shaders;
	auto&&[l_program, l_deferredProgram,
		l_screenSpace,l_screenSpaceHighlight,
		l_debugSolid, l_debugLine,
		l_skyboxShader, l_fsq,
		l_renderNormals, l_accLights, l_forwardPlus,
		l_shadowDepth, l_shadowMap] = l_shaders;

	l_program = createShader(programType::enm_forward,
		l_vertexDir / "shader.vert",
		l_fragmentDir / "shader.frag");

	l_deferredProgram = createShader(programType::enm_passthrough,
		l_vertexDir / "PassThrough.vert",
		l_fragmentDir / "PassThrough.frag");

	l_screenSpace = createShader(programType::enm_screenSpace,
		l_vertexDir / "ScreenSpace.vert",
		l_fragmentDir / "ScreenSpace.frag");

	l_screenSpaceHighlight = createShader(programType::enm_screenSpaceHighlight,
		l_vertexDir / "ScreenSpaceHighlight.vert",
		l_fragmentDir / "ScreenSpaceHighlight.frag");

	l_debugSolid = createShader(programType::enm_debugSolid,
		l_vertexDir / "SolidColor.vert",
		l_fragmentDir / "SolidColor.frag");

	l_debugLine = createShader(programType::enm_debugLine,
		l_vertexDir / "InstancedThickLines.vert",
		l_fragmentDir / "InstancedSolidColor.frag");

	l_skyboxShader = createShader(programType::enm_skybox,
		l_vertexDir / "Skybox.vert",
		l_fragmentDir / "Skybox.frag");

	l_fsq = createShader(programType::enm_fsq,
		l_vertexDir / "FSQ.vert",
		l_fragmentDir / "FSQ.frag");

	l_renderNormals = createShader(programType::enm_writeNormals,
		l_vertexDir / "DeferredGBufferPass.vert",
		l_fragmentDir / "DeferredGBufferPass.frag");

	l_accLights = createShader(programType::enm_accumulateLights,
		l_vertexDir / "DeferredLightingPass.vert",
		l_fragmentDir / "DeferredLightingPass.frag");

	l_forwardPlus = createShader(programType::enm_forwardPlus,
		l_vertexDir / "ForwardPlus.vert",
		l_fragmentDir / "ForwardPlus.frag");

	l_shadowDepth = createShader(programType::enm_shadowDepth,
		l_vertexDir / "DepthWrite.vert",
		l_fragmentDir / "NoOp.frag");

	l_shadowMap = createShader(programType::enm_mapShadows,
		l_vertexDir / "ShadowMap.vert",
		l_fragmentDir / "ShadowMap.frag");

	for (auto&& shader : l_shaders)
	{
		shader->build();
		shader->unbind();
	}
}

void graphicsSystem::initAssets()
{
	const auto l_quadMesh = getVertexArrayManager()->buildFullScreenQuad("fullScreenQuad");

	/******************ORDER OF TEXTURES****************/
	//GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front
	/***************************************************/

	// jpg dont currently work
	/* std::vector<std::string> textureNames{ "wood_create_512x512.jpg", "wood_create_512x512.jpg",
	 *                                        "wood_create_512x512.jpg", "wood_create_512x512.jpg",
	 *                                        "wood_create_512x512.jpg", "wood_create_512x512.jpg" };
	 *
	 */

	//https://opengameart.org/content/interstellar-skybox-png

	const auto getSkyboxTextures = []() -> std::vector<assetHandle>
	{
		const auto textures = std::filesystem::path{ "textures" };

		return {assetManager::getAsset(textures / "skybox_xpos.png"), assetManager::getAsset(textures / "skybox_xneg.png"),
				assetManager::getAsset(textures / "skybox_ypos.png"), assetManager::getAsset(textures / "skybox_yneg.png"),
				assetManager::getAsset(textures / "skybox_zpos.png"), assetManager::getAsset(textures / "skybox_zneg.png")};
	};

	auto l_skybox = getTextureManager()->createCubeTexture("sky_box_texture", getSkyboxTextures());
	l_skybox->build();

	m_textureManager->UnbindAll();
}

void graphicsSystem::shutdownShaders()
{
	auto unloadShader = [](auto& shader, const shaderType type)
	{
		const auto handle = assetManager::getAsset(shader.second->getShader(type)->getFileName());
		if (handle) assetManager::unloadAsset(*handle);
	};

	for (auto&& shader : m_shaderManager->m_shaderPrograms)
	{
		unloadShader(shader, shaderType::enm_vert);
		unloadShader(shader, shaderType::enm_frag);
	}

	for (auto&& program : m_shaderManager->m_programPipeline)
	{
		for (size_t i = 0; i != (size_t)shaderType::enm_count; ++i)
		{
			unloadShader(program, (shaderType)i);
		}
	}

	m_shaderManager->clearShaders();
}



void graphicsSystem::drawLine(
	vector3 const& begin,
	vector3 const& end,
	vector3 const& color,
	float thickness,
	layer layer
)
{
	m_linesToDraw[static_cast<size_t>(layer)].emplace(
		begin, end, color, thickness);
}

void graphicsSystem::onStartFrame()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void graphicsSystem::onUpdate()
{
	//fpsPrint();

	setCurrentMainCameraMemberVariable();

	if (!m_mainCamera)
	{
		return;
	}

	checkAndSetWindowDimensionVariables();

	std::shared_ptr<shaderProgram> l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);
	renderToScreen(l_shaderProgram, m_mainCamera);
	textRenderer::update();
}

void graphicsSystem::onEndFrame()
{
	getSystemManager()->getSystem<platformWindowing>()->presentFrame();
}

void graphicsSystem::onShutdown()
{
	textRenderer::shutdown();
	shutdownShaders();
	delete m_shaderManager;
	delete m_vertexArrayManager;
	delete m_textureManager;
}

float oldfps = 0, newfps = 0;
void graphicsSystem::fpsPrint()
{
	newfps = 1.0f / getDt();

	float smoothing = .99f; // larger=more smoothing
	auto measurement = (oldfps * smoothing) + (newfps * (1.0f - smoothing));

	oldfps = measurement;


	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << measurement;
	std::string mystring = ss.str();
	debug::displayText("fps is (graphicsSystem::onUpdate()): " + mystring, "arial", -500, 350, .5f, color::red);
}

void graphicsSystem::checkAndSetWindowDimensionVariables()
{

	if (m_render_context_ptr->queryContextChanged())
	{
		const auto [width, height] = m_render_context_ptr->queryContextParameters();
		m_viewport = maintainAspect({ width, height }, { 16,9 });

		const auto& [x, y, newWidth, newHeight] = m_viewport;

		glViewport(x, y, newWidth, newHeight);


		



		//for (auto&& name : textureNames)
		//{
		//	m_textureManager->deleteSimpleTexture(name);
		//}
		//
		//m_framebufferManager->resetModBuffers();

		//initFramebuffers();

	}
}

void graphicsSystem::setCurrentMainCameraMemberVariable()
{
	auto l_cameraSystem = this->getSystemManager()->getSystem<cameraSystem>();

	m_mainCamera = l_cameraSystem->getMainCamera();
}

GLint graphicsSystem::getLastAttachedFBO()
{
	GLint editorFboId;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &editorFboId);
	return editorFboId;
}


void graphicsSystem::render(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera)
{
	using GameObjectFiltering::make_componentTypeSet;
	static const auto s_objectType = make_componentTypeSet( renderer::getType(), transform::getType() );
	static const auto s_cameraType = make_componentTypeSet( camera::getType(), transform::getType() );
	static const auto s_lightType = make_componentTypeSet( light::getType(), transform::getType() );
	static const auto s_skyboxType = make_componentTypeSet(skybox::getType());


	auto l_listObjects = getObjectList(s_objectType);
	auto l_listCameras = getObjectList(s_cameraType);
	auto l_listLights =  getObjectList(s_lightType);
	auto l_listSkybox = getObjectList(s_skyboxType);


	glCullFace(GL_BACK);

	p_camera->process(); //TODO(cody): happening every frame.  add dirty flag

	renderOpaque(l_listObjects, l_listLights, l_listSkybox, p_camera);

	if (m_textureIndex != -1)
	{
		const auto [x, y, width, height] = restoreViewport::getViewport();
		renderTextureToScreen(std::static_pointer_cast<simpleTexture>(
			getTextureManager()->getSimpleTexture(textureNames[m_textureIndex])),
			renderTextureParams{ premultiplies[m_textureIndex],
			postadds[m_textureIndex], channelmappings[m_textureIndex],
			m_textureIndex == textureNames::ShadowAtlas,
			{ width / 2, height / 2, width / 3, height / 3 } });
	}


	renderButtons();
}

void graphicsSystem::recordToRenderPool(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera)
{

	// save each frame to the render pool

	p_shaderProgram->bind();

	// get render pool
	unsigned l_index = 0;
	if (!p_camera->hasTape())
	{
		l_index = m_framebufferManager->attachRenderPool();
		p_camera->setTapIndex(l_index);

		p_camera->setHasTap(true);
	}
	else
	{
		l_index = p_camera->getTapeIndex();
	}


	if (p_camera->getCaptureFlag())
	{
		if (!p_camera->getHasCapture())
		{
			p_camera->setCaptureIndex(m_framebufferManager->createCapturePool(p_camera->getTapeIndex()));
			p_camera->setHasCapture(true);
		}
		else
		{
			m_framebufferManager->saveRenderToCapturePool(p_camera->getCaptureIndex(), p_camera->getTapeIndex());
		}

		p_camera->resetCaptureFlag();

	}

	//if (p_camera->isNewRecording())
	//{
	//    //m_framebufferManager->clearBuffer(l_index);

	//    p_camera->isDirty(false);
	//}


   // m_framebufferManager->addFrameToBufferPool(l_index);

	auto & l_bufferPool = m_framebufferManager->getBufferPool(l_index);

	std::shared_ptr<framebuffer> l_temp_buffer = l_bufferPool.front();


	l_temp_buffer->bind();

	l_temp_buffer->clear();

	render(p_shaderProgram, castHandle<cameraBase>(p_camera));

	l_temp_buffer->unbind();

	l_bufferPool.push_back(l_temp_buffer);

	l_bufferPool.pop_front();
}

void graphicsSystem::renderSkybox(const objectList & p_listSkybox, componentHandle<cameraBase> p_camera)
{
	{//========================SkyBox========================//
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(false);
		

		for (const auto & renderObj : p_listSkybox)
		{
			auto l_skybox = renderObj->getComponent<skybox>();
			std::shared_ptr<shaderProgram> l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_skybox);

			l_shaderProgram->bind();
			//GLDebug::getLastError();

			// send camea uniforms

			p_camera->setCameraUniforms(l_shaderProgram);

			//materials

			l_shaderProgram->setUniforms("ModelMatrix", matrix4x4::translation(p_camera->getEyePosition()));
			auto&& mat = assetManager::getAsset(std::filesystem::path{"materials"} / "skybox.mtl")->getData<materialData>().getData();
			l_skybox->onRender(l_shaderProgram, mat, m_textureManager->getCubeTexture(l_skybox->getTextureName()));
			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);

			vao->bind();

			vao->render();

			vao->unbind();

			l_shaderProgram->unbind();
			//GLDebug::getLastError();

			m_textureManager->UnbindAll();
			//GLDebug::getLastError();
		}

		glDepthMask(true);
		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);

	}//=======================================================//
}

void graphicsSystem::renderOpaque(const objectList& p_listObjects, const objectList& p_lights, const objectList& p_skybox, componentHandle<cameraBase> p_camera)
{
	restoreViewport l_oldDim({ 0,0,1920,1080 }); // TODO: maintain internal resolution

	auto&& l_gbuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);

	const auto lastBound = getLastAttachedFBO();
	l_gbuffer->bind();
	l_gbuffer->clear();

	renderNormals(p_listObjects, p_camera);

	objectList l_shadowCasters; l_shadowCasters.reserve(4);
	std::copy_if(p_lights.cbegin(), p_lights.cend(), std::back_inserter(l_shadowCasters),
		[](std::shared_ptr<gameObject> gameObject) -> bool
		{
			const auto* l_light = gameObject->getComponent<light>();
			return l_light->m_castShadow &
				(l_light->m_lightType == lighttype::enm_directional) &
				l_light->m_isActive;
		});

	const auto shadowCount = renderShadows(p_listObjects, l_shadowCasters, p_camera);

	auto&& l_lightBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::LightAccBuf);
	l_lightBuffer->bind();
	l_lightBuffer->clear(bufferClearbits::color);

	renderLighting(p_lights, p_camera);

	auto&& l_forwardBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredOutput);
	l_forwardBuffer->bind();
	l_forwardBuffer->clear(bufferClearbits::color);

	renderForwardPlus(p_listObjects, p_camera, l_shadowCasters);

	renderDebug(p_camera);
	renderSkybox(p_skybox, p_camera);

	glBindFramebuffer(GL_FRAMEBUFFER, lastBound);
	if (lastBound == 0)
	{
		glDrawBuffer(GL_BACK_LEFT);
	}

	renderTextureToScreen(
		std::static_pointer_cast<simpleTexture>(getTextureManager()->getSimpleTexture(textureNames[textureNames::forwardColor])),
		{ /*premultiply=*/{1.f}, /*preadd=*/{0.f}, /*channelmapping=*/{0,1,2,3}, /*compareTexture=*/false, m_viewport }
	);

}

void graphicsSystem::renderNormals(const objectList & p_obj, componentHandle<cameraBase> p_camera)
{
	//glPolygonOffset(1.1f, 4.0f);
	if (auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_writeNormals); l_shader)
	{
		const auto bound = l_shader->objectBind();
		p_camera->setCameraUniforms(l_shader);
		for (auto&& i_obj : p_obj)
		{
			auto&& l_renderer = i_obj->getComponent<renderer>();

			if (!l_renderer->hasMaterial()) continue;

			auto&& l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());

			if (!l_vao) continue;

			auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
			l_shader->setUniforms("material.specularExponent", l_materialPtr->getData<materialData>().getData().getSpecularExponent());

			l_shader->setUniforms("ModelMatrix", i_obj->getComponent<transform>()->getGlobalMatrix());

			l_vao->bind();
			l_vao->render();
			l_vao->unbind();
		}
		getTextureManager()->UnbindAll();
	}
	
	//glPolygonOffset(0, 0);
}



constexpr std::ptrdiff_t s_maxShadows = 4;

size_t graphicsSystem::renderShadows(
	const objectList & p_obj, const objectList & p_lights,
	componentHandle<cameraBase> p_cam)
{
	if (p_lights.empty())
		return 0;

	

	auto&& l_shadowBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::ShadowBuffer);
	l_shadowBuffer->bind();
	l_shadowBuffer->clear(bufferClearbits::depth);

	// reduce peter panning
	glCullFace(GL_FRONT);
	// slope depth bias

	glPolygonOffset(0.6f, 6.0f);

	const auto shadows =
	[this, &p_lights, &p_obj]()
	{

		// render to atlas
		std::array<matrix4x4, s_maxShadows> shadows;

		const auto l_shader = getShaderManager()->getShaderProgram(programType::enm_shadowDepth);
		if (!l_shader)
			return shadows;

		const auto oldDim = restoreViewport{};

		const auto bound = l_shader->objectBind();

		const auto l_gNormals = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);
		l_gNormals->bindDepthAttachment(l_shader);

		for (auto const& [idx, i_light] : enumerate(p_lights))
		{
			// shadow map only has 4 slots
			if (idx == s_maxShadows)
			{
				if (static bool didWarn = false; !didWarn)
				{
					didWarn = true;
					logger("Graphics").warn() << "Shadowing can only be performed with four lights!";
				}
				break;
			}

			const auto width = std::static_pointer_cast<shadowAtlas>
				(getTextureManager()->getSimpleTexture(
					graphicsSystem::textureNames[textureNames::ShadowAtlas]))->size();

			[idx = idx, width = width]
			{
				const unsigned x = (idx % s_maxShadows) * width;
				const unsigned y = (idx / s_maxShadows) * width;

				glViewport(x, y, width, width);
			}();


			const auto texelSize = 1.0f / width;

			auto l_light = i_light->getComponent<light>();
			auto l_lightTrans = i_light->getComponent<transform>();
			const auto l_lightView = [&l_lightTrans,  &texelSize]()->matrix4x4
			{
				const auto start = bit_cast<glm::vec3>(floor(l_lightTrans->getGlobalPosition() / texelSize) * texelSize);
				const auto dir   = bit_cast<glm::vec3>(l_lightTrans->getGlobalRotation().getForward());
				
				return bit_cast<matrix4x4>(glm::transpose(glm::lookAt(start, start + dir, glm::vec3{ 0,1,0 })));
			}();

			

			const auto l_lightOrtho = [&texelSize, &l_lightTrans]() -> auto
			{
				{
					const auto l_scale = bit_cast<glm::vec3>(floor(l_lightTrans->getGlobalScale() * 0.5f / texelSize) * texelSize);

					return bit_cast<matrix4x4>(glm::transpose(glm::ortho(-l_scale.x, l_scale.x, -l_scale.y, l_scale.y, -l_scale.z, l_scale.z)));
				}
			}();

			auto& shadow = shadows[idx];
			shadow = l_lightOrtho * l_lightView;
			l_shader->setUniforms("LightMatrix", shadow);


			// TODO: frustrum culling
			for (auto&& i_obj : p_obj)
			{
				l_shader->setUniforms("ModelMatrix", i_obj->getComponent<transform>()->getGlobalMatrix());

				const auto* l_r = i_obj->getComponent<renderer>();
				const auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_r->GetRenderType());
				
				if (!l_vao) continue;

				l_vao->bind();
				l_vao->render();
				l_vao->unbind();
			}


		}

		return shadows;
	}();

	glCullFace(GL_BACK);
	
	auto l_shadowMap = getFramebufferManager()->getModFramebuffer(framebufferType::ShadowMap);
	l_shadowMap->bind();

	if (auto l_shader = getShaderManager()->getShaderProgram(programType::enm_mapShadows); l_shader)
	{
#ifdef DEBUGGIN_GRAPHICS
		constexpr auto depthFunc = GL_ALWAYS;
#else
		constexpr auto depthFunc = GL_GREATER;
#endif



		glDepthFunc(depthFunc);
		glDepthMask(false);
		
		l_shader->bind();

		auto l_gbuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);
		l_shadowBuffer->bindDepthAttachment(l_shader, 0);
		l_gbuffer->bindDepthAttachment(l_shader, 1);
		l_gbuffer->bindColorAttachments(l_shader, 2);

		p_cam->setCameraUniforms(l_shader);
		l_shader->setUniforms("invView", p_cam->getInvViewMatrix());

		vector2 nearFar;
		glGetFloatv(GL_DEPTH_RANGE, nearFar.toFloatPtr());
		l_shader->setUniforms("depthrange", nearFar);

		auto l_vao = getVertexArrayManager()->getVertexArrayBuffer("fullScreenQuad");

		constexpr std::array<std::string_view, s_maxShadows> uniformNames{
			"LightMatrix[0]", "LightMatrix[1]", "LightMatrix[2]", "LightMatrix[3]"
		};
		
		constexpr std::array<std::string_view, s_maxShadows> uniformNames2{
			"LightDir[0]", "LightDir[1]", "LightDir[2]", "LightDir[3]"
		};

		const size_t shadowCount = std::min(size_t(s_maxShadows), p_lights.size());
		for (size_t i = 0; i != shadowCount; ++i)
		{
			auto l_light = p_lights[i]->getComponent<transform>();
			l_shader->setUniforms(uniformNames[i], shadows[i]);
			l_shader->setUniforms(uniformNames2[i], l_light->getGlobalRotation().getForward());
		}
		l_shader->setUniforms("lightCount", (const int)shadowCount);

		l_vao->bind();
		l_vao->render();
		l_vao->unbind();

		glDepthMask(true);
		glDepthFunc(GL_LESS);
	}

	
	return std::min(size_t(s_maxShadows), p_lights.size());

}

void graphicsSystem::renderLighting(const objectList & p_lights, componentHandle<cameraBase> p_camera)
{
	// TODO: Light Volume stencil hack

	auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_accumulateLights);
	const auto bound = l_shader->objectBind();
	p_camera->setCameraUniforms(l_shader);
	l_shader->setUniforms("invView", p_camera->getInvViewMatrix());

	vector2 nearFar;
	glGetFloatv(GL_DEPTH_RANGE, nearFar.toFloatPtr());
	l_shader->setUniforms("depthrange", nearFar);


	auto&& l_gBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);
	l_gBuffer->bindColorAttachments(l_shader);   // 0
	l_gBuffer->bindDepthAttachment(l_shader, 1); // 1

	// necessary.
	// inputs: normal+shininess, depth, lighting information
	// outputs: intermediate lighting information


	auto l_vao = getVertexArrayManager()->getVertexArrayBuffer("fullScreenQuad");
	l_vao->bind();

	glDepthMask(GL_FALSE);
	glDepthFunc(GL_GREATER);
	glBlendFunc(GL_ONE, GL_ONE); // additive blending
	for (auto&& i_light : p_lights)
	{
		auto&& l_lightC = i_light->getComponent<light>();
		if (!l_lightC->m_isActive || l_lightC->m_castShadow) continue;
		l_lightC->SetLightUniform(0,l_shader, p_camera);
		l_vao->render();
	}
	glBlendFunc(GL_ONE, GL_ZERO);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	l_vao->unbind();

	getTextureManager()->UnbindAll();
}

void graphicsSystem::renderForwardPlus(const objectList & p_objects, componentHandle<cameraBase> p_camera, const objectList & p_lights)
{
	glDepthFunc(GL_EQUAL);
	glDepthMask(GL_FALSE);

	auto&& l_shader = getShaderManager()->getShaderProgram(programType::enm_forwardPlus);
	const auto bound = l_shader->objectBind();
	p_camera->setCameraUniforms(l_shader);
	l_shader->setUniforms("invView", p_camera->getInvViewMatrix());

	vector2 nearFar;
	glGetFloatv(GL_DEPTH_RANGE, nearFar.toFloatPtr());
	l_shader->setUniforms("depthrange", nearFar);

	const auto  l_lightBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::LightAccBuf);
	l_lightBuffer->bindDepthAttachment(l_shader, 0);  // 0
	l_lightBuffer->bindColorAttachments(l_shader, 3); // 3,4,5

	const auto  l_gBuffer = getFramebufferManager()->getModFramebuffer(framebufferType::DeferredGBuffer);
	l_gBuffer->bindColorAttachments(l_shader, 6);

	const auto l_shadowMap = getFramebufferManager()->getModFramebuffer(framebufferType::ShadowMap);
	l_shadowMap->bindColorAttachments(l_shader, 7);

	l_shader->setUniforms("lightCount", (int)std::min(size_t(4), p_lights.size()));

	for (auto&& [i,i_light] : enumerate(p_lights))
	{
		if (i == 4) break;
		i_light->getComponent<light>()->SetLightUniform(i, l_shader, p_camera);
	}

	light::SetGlobalAmbient(l_shader);

	for (auto&& i_obj : p_objects)
	{
		auto const* l_renderer = i_obj->getComponent<renderer>();
		if (!l_renderer->hasMaterial()) continue;

		l_shader->setUniforms("ModelMatrix", i_obj->getComponent<transform>()->getGlobalMatrix());

		const auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
		l_renderer->setMaterialUniforms(l_shader, l_materialPtr->getData<materialData>().getData(), m_textureManager);

		auto&& l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());
		l_vao->bind();
		l_vao->render();
		l_vao->unbind();

		l_renderer->unbindTextures(l_materialPtr->getData<materialData>().getData(), m_textureManager);
	}

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	getTextureManager()->UnbindAll();
}

void graphicsSystem::renderForward(const objectList &, componentHandle<cameraBase>)
{
	// TODO: when applicable, render transparent surfaces here
}

void graphicsSystem::renderOld(const objectList & p_listObjects, const objectList & p_lights, componentHandle<cameraBase> p_camera)
{
	for (const auto & renderObj : p_listObjects)
	{
		auto l_renderer = renderObj->getComponent<renderer>();

		// get the program object and bind
		auto l_shaderProgram = m_shaderManager->getShaderProgram(l_renderer->GetProgramType());

		// GLDebug::getLastFrameBufferError();
		const auto bound = l_shaderProgram->objectBind();

		// set camera uniforms
		p_camera->setCameraUniforms(l_shaderProgram);
		//cam->printMatrices();

		l_shaderProgram->setUniforms("lightCount", (int)(p_lights.size()));
		/*if(l_renderer->GetProgramType() == programType::enm_screenSpace)
			l_shaderProgram->setUniforms("OrthoMatrix", matrix4x4::computeOrthographicMatrix(m_screenWidth, m_screenHeight, m_mainCamera->getNearPlaneDistance(), m_mainCamera->getFarPlaneDistance()));*/

		for (auto&&[i, i_light] : enumerate(p_lights))
		{
			i_light->getComponent<light>()->SetLightUniform(i, l_shaderProgram, p_camera);
		}

		if (!l_renderer->hasMaterial()) continue;

		const auto l_materialPtr = assetManager::getAsset(l_renderer->getMaterialPath());
		l_renderer->setMaterialUniforms(l_shaderProgram, l_materialPtr->getData<materialData>().getData(), m_textureManager);

		l_renderer->setRenderUniforms(l_shaderProgram);
		std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(l_renderer->GetRenderType());

		if (vao == nullptr)
			continue;

		vao->bind();
		vao->render();
		vao->unbind();

		// unbinding step
		m_textureManager->UnbindAll();
	}
}

void graphicsSystem::renderDebug(componentHandle<cameraBase> p_camera)
{
	using GameObjectFiltering::make_componentTypeSet;
	if (m_showWireframes)
	{
		static const auto wireType = make_componentTypeSet( rigidBody::getType(), transform::getType() );
		renderDebugWireframes(getObjectList(wireType), p_camera);
	}

	if (m_showDebugLines)
	{
		static const auto lineType = make_componentTypeSet(debugLines::getType());
		renderDebugLines(getObjectList(lineType), p_camera);
	}
}

void graphicsSystem::renderDebugWireframes(const objectList & p_physObj, componentHandle<cameraBase> p_camera)
{
	auto l_shader = m_shaderManager->getShaderProgram(programType::enm_debugSolid);
	if (!l_shader) return;

	const auto bound = l_shader->objectBind();

	p_camera->setCameraUniforms(l_shader);
	l_shader->setUniforms("color", vector4{ 1,0,0,1 });

	auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);

	if (!l_vao) return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	l_vao->bind();
	for (auto && obj : p_physObj)
	{
		l_shader->setUniforms("ModelMatrix", obj->getComponent<transform>()->getGlobalMatrix());
		l_vao->render();
	}
	l_vao->unbind();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void graphicsSystem::renderDebugLines(const objectList & p_lines, componentHandle<cameraBase> p_camera)
{
	{// DEBUG LINES
		glDepthFunc(GL_ALWAYS);
		// batched lines
		if (auto l_debugLineShader = getShaderManager()->getShaderProgram(programType::enm_debugLine);
			l_debugLineShader)
		{
			const auto bound = l_debugLineShader->objectBind();
			p_camera->setCameraUniforms(l_debugLineShader);

			for (auto&& obj : p_lines)
			{
				for (auto&& component : obj->getComponents<debugLines>())
				{
					auto&& l_buffer = (component)->m_buffer;
					auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(l_buffer.getAssetHandle());

					l_buffer.update();
					l_buffer.bind();
					l_buffer.render(l_vao);
					l_buffer.unbind();
				}
			}
		}

		// immediate lines
		if(auto l_shader = m_shaderManager->getShaderProgram(programType::enm_debugSolid);
			l_shader)
		{
			auto l_vao = m_vertexArrayManager->getVertexArrayBuffer(CUBE_FILE);
			l_vao->bind();

			const auto bound = l_shader->objectBind();
			p_camera->setCameraUniforms(l_shader);


			auto renderLine = [&l_shader, &l_vao](lineSegment const& toRender)
			{
				auto buildModel = [](vector3 const& begin, vector3 const& end, float weight) -> matrix4x4
				{
					vector3 l_vec = end - begin;
					vector3 l_trans = (begin + end) * 0.5f;
					matrix4x4 l_rot{};
					l_rot.computeModelMatrixFromLookAtVector2(l_trans, l_vec + l_trans);
					vector3 l_scale{ weight, weight, l_vec.distance() * 0.5f + weight };

					return l_rot * matrix4x4::scale(l_scale);
				};

				l_shader->setUniforms("ModelMatrix", buildModel(toRender.begin, toRender.end, toRender.weight));
				l_shader->setUniforms("color", vector4(toRender.color, 1));

				l_vao->render();
			};

			auto renderLineQueue = [&renderLine](auto && lineQueue)
			{
				while (!lineQueue.empty())
				{
					renderLine(lineQueue.front());
					lineQueue.pop();
				}
			};

			glDepthFunc(GL_LESS);
			renderLineQueue(m_linesToDraw.front());

			glDepthFunc(GL_ALWAYS); // so skybox doesn't overwrite lines

			std::for_each(m_linesToDraw.begin() + 1, m_linesToDraw.end(), renderLineQueue);

			glDepthFunc(GL_LESS);
			l_vao->unbind();
		}
	}

	glDepthMask(true);

	m_textureManager->UnbindAll();
}

void graphicsSystem::renderTextureToScreen(std::shared_ptr<simpleTexture> p_texture, renderTextureParams const& params)
{
	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);
	auto l_shader = getShaderManager()->getShaderProgram(programType::enm_fsq);

	const auto bound = l_shader->objectBind();

	l_shader->setUniforms("channelmapper", params.channelMapper);
	l_shader->setUniforms("premultiply", params.premultiply);
	l_shader->setUniforms("postadd", params.postadd);

	p_texture->bind(0);

	if (params.compareTexture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}
	
	const auto oldDim = restoreViewport{ params.where };

	auto l_vao = getVertexArrayManager()->getVertexArrayBuffer("fullScreenQuad");

	l_vao->bind();
	l_vao->render();
	l_vao->unbind();

	if (params.compareTexture)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	}

	p_texture->unbind();

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

viewportGeometry const& graphicsSystem::getViewport() const
{
	return m_viewport;
}

componentHandle<camera> graphicsSystem::getCameraByChannel(unsigned p_channel)
{
	using GameObjectFiltering::make_componentTypeSet;
	static const auto cameraType = make_componentTypeSet(camera::getType());
	auto&& l_cameras = getObjectList(cameraType);

	auto l_found = std::find_if(l_cameras.begin(), l_cameras.end(),
		[p_channel](std::shared_ptr<gameObject> const& obj) -> bool
		{
			return obj->getComponent<camera>()->getBroadcastCannel() == p_channel;
		}
	);

	if (l_found != l_cameras.end())
		return (*l_found)->getComponent<camera>();

	return componentHandle<camera>();
}

void graphicsSystem::captureForAllRecordingCameras()
{
	using GameObjectFiltering::make_componentTypeSet;
	static const auto cameraType = make_componentTypeSet(camera::getType());
	for (auto&& l_cameraObject : getObjectList(cameraType))
	{
		auto l_camera = l_cameraObject->getComponent<camera>();

		/*if (l_camera->getIsRecording())
		{*/
		auto l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);
		recordToRenderPool(l_shaderProgram, l_camera);
		//}
	}
}

void graphicsSystem::renderToScreen(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<cameraBase> p_camera)
{
	const auto bound = p_shaderProgram->objectBind();

	//m_framebufferManager->renderToScreen();

	render(p_shaderProgram, p_camera);
}

std::shared_ptr<framebuffer> graphicsSystem::renderToBuffer(std::shared_ptr<shaderProgram> p_shaderProgram, componentHandle<camera> p_camera)
{
	std::shared_ptr<framebuffer> l_capture_buffer = getFramebufferManager()->getFramebuffer(framebufferType::CaptureBuffer);

	l_capture_buffer->bind();
	l_capture_buffer->clear();

	const auto bound = p_shaderProgram->objectBind();

	render(p_shaderProgram, castHandle<cameraBase>(p_camera));

	return l_capture_buffer;
}

void graphicsSystem::renderButtons()
{
	const auto l_oldDim = restoreViewport{ m_viewport };
	// Render stuff to screen
	using GameObjectFiltering::make_componentTypeSet;
	static const auto buttonType = make_componentTypeSet( uiSprite::getType(), transform::getType() );
	auto l_listButton = getObjectList(buttonType);
	std::sort(l_listButton.begin(), l_listButton.end(), [](std::shared_ptr<gameObject> & a, std::shared_ptr<gameObject> & b) -> bool
	{ return a->getComponent<transform>()->getPosition().getZ() < b->getComponent<transform>()->getPosition().getZ(); });

	glDepthFunc(GL_ALWAYS);
	glDepthMask(GL_FALSE);
	for (auto & buttonObj : l_listButton)
	{
		auto l_transform = buttonObj->getComponent<transform>();
		auto l_uiObject = buttonObj->getComponent<uiSprite>();

		if (l_uiObject && l_transform)
		{
			if (l_uiObject->isVisible())
			{
				//l_shaderProgram->setUniforms("ModelMatrix", l_transform->getLocalMatrix());

				//l_shaderProgram->setUniforms("ViewMatrix", m_mainCamera->getViewMatrix());
				//			//1024, 768
				//	auto ortho = matrix4x4::computeOrthographicMatrix(0, 1024/*m_mainCamera->getWidth()*/,0, 768/*m_mainCamera->getHeight()*/, m_mainCamera->getNearPlaneDistance(), m_mainCamera->getFarPlaneDistance());
				//l_shaderProgram->setUniforms("OrthoMatrix", ortho);

				const auto l_shaderType = l_uiObject->getShaderType(); // l_uiObject->isSelected() ? programType::enm_screenSpaceHighlight : programType::enm_screenSpace;
				auto l_shaderProgram{ m_shaderManager->getShaderProgram(l_shaderType)};

				if (!l_shaderProgram)
					continue;

				const auto bound = l_shaderProgram->objectBind();

				l_shaderProgram->setUniforms("Scale", l_transform->getGlobalScale());

				l_shaderProgram->setUniforms("Transform", l_transform->getGlobalPosition());

				l_shaderProgram->setUniforms("CameraWidth", m_mainCamera->getWidth());

				l_shaderProgram->setUniforms("CameraHeight", m_mainCamera->getHeight());

				std::shared_ptr<texture> l_texture;

				std::string l_textureName = l_uiObject->getTextureName();

				l_texture = m_textureManager->getSimpleTexture(l_textureName);

				/*if(l_uiObject->isSelected())
					l_texture = m_textureManager->getSimpleTexture("button2.png");
				else
					l_texture = m_textureManager->getSimpleTexture("button1.png");*/


					//Bad texture
				if (l_texture == nullptr)
				{
					continue;
				}

				l_texture->bind(0);

				l_shaderProgram->setUniforms("ScreenSampler", l_texture->getBoundSlot());

				auto vao = m_vertexArrayManager->getVertexArrayBuffer("fullScreenQuad");

				vao->bind();
				vao->render();
				vao->unbind();

				// unbinding step
			   // shaderProgram->unbind();
				m_textureManager->UnbindAll();
			}

		}
	}

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

void graphicsSystem::initializeRenderTargets()
{
	std::shared_ptr<framebuffer> l_buffer = getFramebufferManager()->createFramebuffer(framebufferType::CaptureBuffer, 1024, 768);

	l_buffer->addColorAttachment("screenSampler");

#if CAPTURE
	for (int i = 0; i < 2; i++)
	{
		m_framebufferManager->createBufferPool(MAXFRAMES);
	}
#endif

	l_buffer->build();
}

void graphicsSystem::updateRenderTargets()
{
	using GameObjectFiltering::make_componentTypeSet;
	static const auto renderTargetType = make_componentTypeSet( renderTarget::getType(), transform::getType() );
	auto l_renderTargetObject = getObjectList(renderTargetType);

	std::shared_ptr<shaderProgram> l_shaderProgram;


#if CAPTURE
	captureForAllRecordingCameras();
#endif

	for (auto & l_renderObj : l_renderTargetObject)
	{
		auto l_target = l_renderObj->getComponent<renderTarget>();
		auto l_transform = l_renderObj->getComponent<transform>();
#if CAPTURE
		if (l_target->getPlayState() == play_state::enum_feed)
		{
#endif
			if (auto l_camera2 = getCameraByChannel(l_target->getFeedChannel()); l_camera2)
			{
				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);

				auto l_capture_buffer = renderToBuffer(l_shaderProgram, l_camera2);

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
				l_shaderProgram->bind();


				l_capture_buffer->bind();

				l_capture_buffer->bindColorAttachments(l_shaderProgram);

				//l_capture_buffer->unbind();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, getLastAttachedFBO());



			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			auto vao = getVertexArrayManager()->getVertexArrayBuffer("fullScreenQuad");

			vao->bind();
			vao->render();

			vao->unbind();
#if CAPTURE
		}
		else if (l_target->getPlayState() == play_state::enum_playback)
		{

			auto l_camera2 = getCameraByChannel(l_target->getPlayBackChannel());

			if (l_camera2 != nullptr)
			{
				if (l_camera2->getHasCapture())
				{
					l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
					l_shaderProgram->bind();

					unsigned l_index = l_camera2->getTapeIndex();
					auto & l_bufferPool = m_framebufferManager->getCapturePool(l_index);
					std::shared_ptr<framebuffer> l_playback_buffer = l_bufferPool.front();

					l_playback_buffer->bind();

					l_playback_buffer->bindColorAttachments(l_shaderProgram);

					l_playback_buffer->unbind();

					l_bufferPool.push_back(l_playback_buffer);

					l_bufferPool.pop_front();

				}
			}


			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(primitiveType::enm_quad);


			vao->bind();
			vao->render();
			vao->unbind();
		}
		else
		{
			auto l_camera2 = getCameraByChannel(l_target->getFeedChannel());

			if (l_camera2 != nullptr)
			{

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_forward);

				auto l_capture_buffer = renderToBuffer(l_shaderProgram, l_camera2);

				l_shaderProgram = m_shaderManager->getShaderProgram(programType::enm_passthrough);
				l_shaderProgram->bind();


				l_capture_buffer->bind();

				l_capture_buffer->bindColorAttachments(l_shaderProgram);

				l_capture_buffer->unbind();
			}




			m_mainCamera->setCameraUniforms(l_shaderProgram);

			l_shaderProgram->setUniforms("ModelMatrix", l_transform->getGlobalMatrix());

			std::shared_ptr<vertexArrayObject> vao = m_vertexArrayManager->getVertexArrayBuffer(primitiveType::enm_quad);

			vao->bind();
			vao->render();
			vao->unbind();
		}

#endif

	}
}
