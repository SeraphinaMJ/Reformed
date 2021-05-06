#include "stdafx.h"
#include "TempestSerializationGenerator.h"
#include <direct.h>

void tempestSerializationGenerator::writeFile(std::filesystem::path p_directory)
{
	_chdir(p_directory.string().c_str());
	std::ofstream File;
	File.open("ComponentLists.h");
	File << IncludeList.rdbuf() << std::endl << ComponentList.rdbuf() << std::endl << ScriptList.rdbuf() << std::endl;
	File.close();
}

void tempestSerializationGenerator::generateData(const std::map<std::string, tempestClassDetails*>& p_classData)
{
	setupSerializationFile();
	parseClasses(p_classData);
}

void tempestSerializationGenerator::setupSerializationFile()
{
	IncludeList << "#pragma once" << std::endl;
	ComponentList << "#define COMPONENT_LIST \\" << std::endl;
	ScriptList << "#define SCRIPT_LIST \\" << std::endl;
}

void tempestSerializationGenerator::parseClasses(const std::map<std::string, tempestClassDetails*>& p_classData)
{
	for(auto i_class : p_classData)
	{
		if(i_class.second->m_isComponent && !i_class.second->m_isAbstract && i_class.second->m_name != "component" && i_class.second->m_name != "scriptCPP")
		{
			serializeComponent(*i_class.second);
		}
		if (i_class.second->m_isScript && !i_class.second->m_isAbstract && i_class.second->m_name != "scriptCPP")
		{
			serializeScript(*i_class.second);
		}
	}
}

void tempestSerializationGenerator::serializeComponent(const tempestClassDetails& p_component)
{
	if (m_includes.find(p_component.m_RelativePath.string()) == m_includes.end())
	{
		m_includes.insert(p_component.m_RelativePath.string());
		IncludeList << "#include \"" << p_component.m_RelativePath.string() << "\"" << std::endl;
	}
	ComponentList << "X(" << p_component.m_name << ")\\" << std::endl;
}

void tempestSerializationGenerator::serializeScript(const tempestClassDetails& p_component)
{
	ScriptList << "X(" << p_component.m_name << ")\\" << std::endl;
}
