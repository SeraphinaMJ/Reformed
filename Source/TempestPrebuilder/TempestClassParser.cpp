#include "stdafx.h"
#include "TempestClassParser.h"
#include <direct.h>
#include "ClangToolFrontEnd.h"
#include <sstream>
#include <fstream>

tempestClassParser::tempestClassParser(const tempestPrebuildSetup& p_SetupDetails) :m_ProjectPath(p_SetupDetails.projectLocation), m_excludeList(p_SetupDetails.excludeList)
{
}

void tempestClassParser::generateClassData()
{
	recurseDirectory(std::filesystem::directory_iterator(m_ProjectPath));
	resolveRelativePaths();
}

const std::map<std::string, tempestClassDetails*>& tempestClassParser::getClassData()
{
	return m_FoundClasses;
}

std::filesystem::path tempestClassParser::getProjectLocation()
{
	return m_ProjectPath;
}

void tempestClassParser::resolveRelativePaths()
{
	for(auto i_classes : m_FoundClasses)
	{
		if(i_classes.second->m_RelativePath == "")
		{
			i_classes.second->m_RelativePath = i_classes.second->m_AbsolutePath.string().erase(0, m_ProjectPath.string().size() + 1);
		}
	}
}

void tempestClassParser::recurseDirectory(std::filesystem::directory_iterator p_Directory)
{
	for (auto itter : p_Directory)
	{
		if (itter.is_directory() && !isExcluded(itter.path()))
		{
			recurseDirectory(std::filesystem::directory_iterator(itter.path()));
		}
		else if (itter.path().extension() == ".cpp")
		{
			runToolOnCode(itter.path());
		}
	}
}

bool tempestClassParser::isExcluded(const std::filesystem::path& p_path)
{
	for (auto i_excluded : m_excludeList)
	{
		if (i_excluded == p_path.string())
		{
			return true;
		}
	}
	return false;
}

void tempestClassParser::runToolOnCode(std::filesystem::path p_file)
{
	_chdir(p_file.parent_path().string().c_str());
	std::stringstream l_stream;
	l_stream << std::ifstream(p_file).rdbuf();
	clang::tooling::runToolOnCodeWithArgs(new FindNamedClassAction(&m_FoundClasses), l_stream.str(), {"-DSYNTAX_ONLY_REFLECTION_PASS"}, p_file.filename().string());
}
