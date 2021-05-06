#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <filesystem>
#include <set>

#include "TempestClassParser.h"

class tempestSerializationGenerator
{
public:
	void generateData(const std::map<std::string, tempestClassDetails*>& p_classData);
	void writeFile(std::filesystem::path p_directory);

private:
	void setupSerializationFile();
	void parseClasses(const std::map<std::string, tempestClassDetails*>& p_classData);
	void serializeComponent(const tempestClassDetails& p_component);
	void serializeScript(const tempestClassDetails& p_component);

	std::stringstream ComponentList;
	std::stringstream ScriptList;
	std::stringstream IncludeList;

	std::set<std::string> m_includes;
};
