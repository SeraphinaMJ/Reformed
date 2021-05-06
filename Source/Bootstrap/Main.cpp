/*!***************************************************************************************
\file       Main.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the main entry point for the project.
*****************************************************************************************/

//========Self Include==================================================================//
//========1st Party Includes============================================================//

//========3rd Party Includes============================================================//
#include <GlfwWindow.hpp>

#include <filesystem>

#define SQL_WCHART_CONVERT

#include <Configuration.hpp>
#include <GLFWAdapter.hpp>
#include <EngineRunner.hpp>
#include <CLI/CLI.hpp>
#include <windows.h>
#include <shlobj.h>
#include <winerror.h>

// TODO: How do I expose public utility headers to Editor and Bootstrap??? Patrick
#include "../Utility/Public/Exports.hpp"

extern "C"
{
	// Enable dedicated graphics for NVIDIA Optimius
	TEMPEST_EXPORT unsigned long NvOptimusEnablement = 0x00000001;
	// Enable dedicated graphics for AMD Radeon :
	TEMPEST_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}



//========Types=========================================================================//
//========Defines=======================================================================//
#ifndef _WIN32
# define __argc argc
# define __argv argv
#endif
//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

/*!***************************************************************************************
\brief  The main entry point for the bootstrapper.  Initializes graphics packages and the
        engine and links the two together.  Also holds the main loop for running the engine.
\param argc - the number of arguments to the program.
\param argv - list of the arguments as strings
\return int - success or failure code of the program.
*****************************************************************************************/
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char ** argv)
#endif
{
	// parse command line parameters
	CLI::App app;

	std::filesystem::path l_basePath("../../Samples/");
	app.add_option("--basePath", l_basePath, "Base Path", true);

	std::filesystem::path l_assetsPath("Assets/");
	app.add_option("--assetPath", l_assetsPath, "Assets Subpath", true);

	std::filesystem::path l_dataPath("Data/");
	app.add_option("--dataPath", l_dataPath, "Data Subpath", true);

	// -------------
	// Local Paths

	WCHAR _local_path[MAX_PATH] = { 0 };
	PWSTR local_path = _local_path;
	if(auto result = SHGetKnownFolderPath(FOLDERID_LocalAppDataLow, 0, NULL, &local_path); result != S_OK)
	{
		if(result = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &local_path); result != S_OK)
		{
			// TODO: We should probably find some other place to dump the log
			//   files
		}
	}

	std::filesystem::path l_localBasePath(local_path);
	l_localBasePath = l_localBasePath / "DigiPen" / "ProjectBoomerang";
	app.add_option("--localBasePath", l_localBasePath, "Base Path for local files", true);

	std::filesystem::path l_logPath = l_localBasePath / "log.txt";
	app.add_option("--logPath", l_logPath, "Path for log files", true);

	std::string l_initialScene = "title_screen";
	app.add_option("--initialScene", l_initialScene, "Initial Scene", true);

	bool l_hideWindow = false;
	app.add_flag("--hideWindow", l_hideWindow, "Hide the window on startup");

	bool l_verbose = false;
	app.add_flag("--verbose", l_verbose, "Enable verbose logging.");

	bool l_quiet = false;
	app.add_flag("--quiet", l_quiet, "Disable all non-error logging output.");

	CLI11_PARSE(app, __argc, __argv);

	// setup the window
	glfwWindow * l_window = new glfwWindow(4, 3, 1280, 720, "RumbleOfToys", /*fullscreen=*/true);

	// build the engine instance
	glfwAdapter * l_glfw_adapter = new glfwAdapter(l_window->getDataPointer());
	engineRunner l_engine(l_glfw_adapter, l_glfw_adapter, l_glfw_adapter);
	configuration l_config;
	l_config.asset_path = l_basePath / l_assetsPath;
	l_config.data_path = l_basePath / l_dataPath;
	l_config.initial_scene_name = l_initialScene;
	l_config.initial_window_show = !l_hideWindow;
	l_config.log_path = l_logPath;
	l_config.quiet = l_quiet;
	l_config.verbose = l_verbose;
	l_engine.initialize(l_config);

	if(!l_quiet)
		std::cout << "Logging to " << l_config.log_path << std::endl;

	// run the engine
	while(!l_glfw_adapter->isClosing())
	{
		l_engine.update();
		l_engine.render();

	}

	// shut everything down
	l_engine.shutdown();
	delete l_glfw_adapter;
	delete l_window;

	return 0;
}



////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>

