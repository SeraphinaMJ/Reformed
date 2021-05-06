#include "stdafx.h"
#include <fstream>
#include "TempestPrebuildSetup.h"
#include <regex>
#include <direct.h>

tempestPrebuildSetup::tempestPrebuildSetup(std::filesystem::path p_optionsFile)
{
	_chdir(p_optionsFile.parent_path().string().c_str());
	std::ifstream optionsFile(p_optionsFile);
	std::string currentLine;
	while (!optionsFile.eof())
	{
		std::getline(optionsFile, currentLine);

		std::smatch matches;
		if (std::regex_match(currentLine, matches, std::regex("Project_Location=(.*)")))
		{
			projectLocation = std::filesystem::absolute(matches[1].str());
		}
		else if(std::regex_match(currentLine,std::regex("Excluded_Directories=.*")))
		{
			std::regex_search(currentLine, matches, std::regex("Excluded_Directories=([^;]+)(?:;([^;]+))*"));
			for(unsigned int i = 0; i < matches.size(); ++i)
			{
				if(i == 0)
				{
					//Skip the first result because it's always the whole string.
					//We don't start i at 1 because we will crash if there are no exclude directories.
					continue;
				}
				excludeList.push_back(matches[i]);
			}
		}
	}
}
