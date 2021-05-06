#pragma once
#include <vector>
#include <string>
#include <filesystem>
class tempestPrebuildSetup
{
public:
	tempestPrebuildSetup(std::filesystem::path p_optionsFile);
	std::filesystem::path projectLocation;
	std::vector<std::string> excludeList;
};