/*!***************************************************************************************
\file       UI_System.cpp
\author     Morgen Hyde
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/

#include <iostream>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include "UI_System.h"

#include "ObjectHierarchy.h"
#include "ObjectEditor.h"
#include "AudioPlayer.h"
#include "AudioMetrics.h"

void error_callback(int error, const char* description)
{
	std::cout << "ERROR " << error << ": " << description << std::endl;
}

UI_System::UI_System(Audio::BusInterface* busInterface_p) 
	: systemName("UISystem"), bInterface(busInterface_p), UI_EventManager(new Audio::EventManager())
{
}

UI_System::~UI_System()
{
	delete UI_EventManager;
}

bool UI_System::onInitialize()
{
	if (!glfwInit())
	{
		std::cout << "Failed to init glfw, press enter to close application" << std::endl;
		std::cin.get();
		return false;
	}

	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	AppWindow = glfwCreateWindow(1280, 720, "My Audio Authoring Tool", nullptr, nullptr);
	if (!AppWindow)
		return false;

	glfwMakeContextCurrent(AppWindow);

	if (!gladLoadGL())
	{
		std::cout << "failed to initialize glad loader" << std::endl;
		std::cin.get();
		return false;
	}

	ImGui::CreateContext();

	ImGuiIO& myIO = ImGui::GetIO();
	myIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	// Setup the authoring tool's style information..
	ImGuiStyle& myStyle = ImGui::GetStyle();
	myStyle.Colors[ImGuiCol_WindowBg] = ImColor(36, 40, 38, 255);
	myStyle.Colors[ImGuiCol_ChildBg]  = ImColor(36, 40, 38, 255);

	myStyle.Colors[ImGuiCol_Border] = ImColor(83, 142, 116, 255);

	myStyle.Colors[ImGuiCol_FrameBg]        = ImColor(31, 73, 44, 255);
	myStyle.Colors[ImGuiCol_FrameBgHovered] = ImColor(46, 89, 63, 255);
	myStyle.Colors[ImGuiCol_FrameBgActive]  = ImColor(58, 124, 115, 255);
	 
	myStyle.Colors[ImGuiCol_TitleBg]          = ImColor(58, 124, 115, 255);
	myStyle.Colors[ImGuiCol_TitleBgActive]    = ImColor(58, 124, 115, 255);
	myStyle.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(58, 124, 115, 255);

	myStyle.Colors[ImGuiCol_MenuBarBg] = ImColor(147, 168, 172, 100);

	myStyle.Colors[ImGuiCol_ScrollbarBg]          = ImColor(5, 5, 5, 135);
	myStyle.Colors[ImGuiCol_ScrollbarGrab]        = ImColor(79, 79, 79, 255);
	myStyle.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(105, 105, 105, 255);
	myStyle.Colors[ImGuiCol_ScrollbarGrabActive]  = ImColor(130, 130, 130, 255);

	myStyle.Colors[ImGuiCol_CheckMark]        = ImColor(147, 168, 172, 255);
	myStyle.Colors[ImGuiCol_SliderGrab]       = ImColor(147, 168, 172, 100);
	myStyle.Colors[ImGuiCol_SliderGrabActive] = ImColor(104, 119, 122, 255);
	
	myStyle.Colors[ImGuiCol_Button]        = ImColor(40, 80, 100, 255);
	myStyle.Colors[ImGuiCol_ButtonHovered] = ImColor(56, 100, 130, 255);
	myStyle.Colors[ImGuiCol_ButtonActive]  = ImColor(68, 122, 160, 255);

	myStyle.Colors[ImGuiCol_Header]        = ImColor(40, 80, 100, 255);
	myStyle.Colors[ImGuiCol_HeaderHovered] = ImColor(56, 100, 130, 255);
	myStyle.Colors[ImGuiCol_HeaderActive]  = ImColor(68, 122, 160, 255);

	myStyle.Colors[ImGuiCol_Separator]        = ImColor(83, 142, 116, 255);
	myStyle.Colors[ImGuiCol_SeparatorHovered] = ImColor(83, 142, 116, 255);
	myStyle.Colors[ImGuiCol_SeparatorActive]  = ImColor(83, 142, 116, 255);

	myStyle.Colors[ImGuiCol_ResizeGrip]        = ImColor(255, 255, 250, 255);
	myStyle.Colors[ImGuiCol_ResizeGripHovered] = ImColor(255, 255, 250, 200);
	myStyle.Colors[ImGuiCol_ResizeGripActive]  = ImColor(255, 255, 250, 100);

	myStyle.Colors[ImGuiCol_Tab]        = ImColor(40, 80, 100, 255);
	myStyle.Colors[ImGuiCol_TabHovered] = ImColor(56, 100, 130, 255);
	myStyle.Colors[ImGuiCol_TabActive]  = ImColor(68, 122, 160, 255);

	ImGui_ImplGlfw_InitForOpenGL(AppWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430"); // maybe add core afterwards

	// vv Create the various large modules of the UI here! vv
	ObjectHierarchy*  hierarchy = new ObjectHierarchy();
	ObjectEditor*     editor    = new ObjectEditor(bInterface);
	AudioPlayer*      player    = new AudioPlayer();
	AudioMetrics*     metrics   = new AudioMetrics();


	addComponents(hierarchy);
	addComponents(editor);
	addComponents(player);
	addComponents(metrics);

	UI_EventManager->RegisterEventCallback<AudioPlayer, UI::ActiveSwapEvent, &AudioPlayer::changeActiveCallback>(player);
	UI_EventManager->RegisterEventCallback<ObjectEditor, UI::ActiveSwapEvent, &ObjectEditor::setActiveObject>(editor);

	return true;
}

void UI_System::onUpdate()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::vector<UI_ComponentsBase*>::iterator iter, endIter;
	for(iter = UI_elements.begin(), endIter = UI_elements.end(); iter != endIter; ++iter)
		(*iter)->onUpdate();

	UI_EventManager->ParseAllEvents();

	ImGui::Render();
	glfwMakeContextCurrent(AppWindow);
	int displayWidth, displayHeight;
	glfwGetFramebufferSize(AppWindow, &displayWidth, &displayHeight);
	glViewport(0, 0, displayWidth, displayHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwMakeContextCurrent(AppWindow);
	glfwSwapBuffers(AppWindow);
}

void UI_System::onShutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	for (unsigned i = 0; i < UI_elements.size(); i++)
		delete UI_elements[i];

	glfwDestroyWindow(AppWindow);
	glfwTerminate();
}

void UI_System::addComponents(UI_ComponentsBase* newComponent)
{
	newComponent->setSystemPointer(this);
	UI_elements.push_back(newComponent);
}

bool UI_System::windowCloseCommand()
{
	if (glfwWindowShouldClose(AppWindow))
		return true;

	return false;
}
