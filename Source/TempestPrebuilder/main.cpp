#include <filesystem>
#include "stdafx.h"
#include "TempestClassParser.h"
#include "TempestSerializationGenerator.h"
#include <direct.h>
#include "TempestPrebuildSetup.h"
#include <iostream>


#ifdef _DEBUG
int main()
#else
int WinMain()
#endif
{
	//std::filesystem::path mypath("C:/Users/david.r/Desktop/SourceTreeRepo/Project/ProjectBoomerang/TempestEngine/Prebuild/ParseSettings.tps");
	//system("exit");
	if(__argc < 2)
	{
		std::cout << "No settings file was provided" << std::endl;
		return 0;
	}
	if(!std::filesystem::exists(__argv[1]))
	{
		std::cout << "The settings file provided does not exist:" << __argv[1] << std::endl;
		return 0;
	}
	try
	{
		tempestPrebuildSetup Setup(__argv[1]);
		//tempestPrebuildSetup Setup(mypath);
		tempestClassParser parser(Setup);
		parser.generateClassData();

		tempestSerializationGenerator serializer;
		serializer.generateData(parser.getClassData());
		serializer.writeFile(Setup.projectLocation);
	}
	catch (std::filesystem::filesystem_error&)
	{
		std::cout << "The project location given in the settings file does not exist" << std::endl;
		return 0;
	}
	return 0;
}