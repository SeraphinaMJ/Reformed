/*!***************************************************************************************
\file       GLFWAdapter.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include <ExternalWindowInterface.hpp>
#include <RenderContextInterface.hpp>
#include <InputSourceInterface.hpp>

#include <vector>


struct glfwWindowData;

class glfwAdapter : public externalWindowInterface, public renderContextInterface,
	public inputSourceInterface
{
public:
	glfwAdapter(glfwWindowData * p_window);
	virtual ~glfwAdapter();

	// externalWindowInterface implementation
	void handleProcessEvents() override;
	void handleResizeRequest(int p_width, int p_height) override;
	void handleFullscreenToggleRequest() override;
	void handleQuitRequest() override;

	// renderContextInterface implementation
	void handleSwapBuffersRequest() override;
	bool queryContextChanged() override;
	renderContextParameters queryContextParameters() override;
	void updateAssetPath(const std::string& newPath) override;
	void draw(const renderable& r) override;
	void updateCameraMatrices(const cameraData& c) override;

	// inputSourceInterface implementation
	void enableMouseCapture() override;
	void disableMouseCapture() override;
	void setKeyboardCallback(keyboardCallbackFunction p_function_ptr) override;
	void setMousePosCallback(mousePosCallbackFunction p_function_ptr) override;
	void setMouseButtonCallback(mouseButtonCallbackFunction p_function_ptr) override;
	void setGamepadButtonCallback(gamepadButtonCallbackFunction p_function_ptr) override;
	void setGamepadAxisCallbackFunction(gamepadAxisCallbackFunction p_function_ptr) override;
	scancodeWrapper mapLetterToScancode(unsigned p_letter) override;

	std::pair<int, int> getWindowDimentions() override;
	virtual std::pair<int, int> getWindowPos();

	bool isClosing() const;

	bool isFullscreen() const;

protected:
	bool m_parameters_changed;
	renderContextParameters m_parameters;
	
	virtual void onGLFWKeyCallback(int p_key, int p_scancode, int p_action, int p_mods);
	virtual void onGLFWMousePosCallback(double p_xpos, double p_ypos);
	virtual void onGLFWMouseButtonCallback(int p_key, int p_action, int p_mods);


private:
	glfwWindowData * m_data;
	bool m_should_close = false;

	keyboardCallbackFunction m_keyboard_callback;
	mousePosCallbackFunction m_mouse_pos_callback;
	mouseButtonCallbackFunction m_mouse_button_callback;

	std::vector<actionType> m_glfw_action_map;

	// incredibly lazy window position. Only updates when toggling to fullscreen and used when toggling from fullscreen
	// getWindowPos() should be used instead
	std::pair<int, int> m_last_windowed_pos = { 0,0 };
};

