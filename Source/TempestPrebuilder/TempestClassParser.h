#pragma once
#include <filesystem>
#include <map>
#include "TempestPrebuildSetup.h"

class tempestMember
{
public:
	tempestMember(std::string p_type, std::string p_name) : m_type(p_type), m_name(p_name) {}
	std::string m_type;
	std::string m_name;
};

class tempestClassDetails
{
public:
	tempestClassDetails(std::string p_name, bool p_isComponent, bool p_isScript, bool p_isSystem, bool p_isAbstract, std::filesystem::path p_AbsolutePath, std::filesystem::path p_RelativePath = "") :
		m_name(p_name), m_isComponent(p_isComponent), m_isScript(p_isScript), m_isSystem(p_isSystem), m_isAbstract(p_isAbstract), m_AbsolutePath(p_AbsolutePath), m_RelativePath(p_RelativePath) {}
	std::string m_name;
	bool m_isComponent;
	bool m_isScript;
	bool m_isSystem;
	bool m_isAbstract;
	std::filesystem::path m_AbsolutePath;
	std::filesystem::path m_RelativePath;
	std::vector<tempestMember> m_members;
};
class tempestClassParser
{
public:
	tempestClassParser(const tempestPrebuildSetup& p_SetupDetails);
	void generateClassData();
	const std::map<std::string, tempestClassDetails*>& getClassData();
	std::filesystem::path getProjectLocation();

private:
	void resolveRelativePaths();
	void recurseDirectory(std::filesystem::directory_iterator p_Directory);
	bool isExcluded(const std::filesystem::path& p_path);
	void runToolOnCode(std::filesystem::path p_file);

	std::filesystem::path m_ProjectPath;
	std::map<std::string, tempestClassDetails*> m_FoundClasses;
	std::vector<std::string> m_excludeList;
};