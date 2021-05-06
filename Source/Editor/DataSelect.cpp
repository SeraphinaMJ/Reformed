/*!***************************************************************************************
\file       DataSelect.cpp
\author     Ryan Booth
\date       9/18/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		This is Cole's beautifully designed Telemetry Data Parser.
*****************************************************************************************/

//========Self Include==================================================================//
#include "DataSelect.hpp"
//========1st Party Includes============================================================//
#include "EditorWindow.hpp"
#include "CSVParser.hpp"

//========3rd Party Includes============================================================//
#include <filesystem>
#include <fstream>
#include <imgui.h>
#include <EngineRunner.hpp>
#include <Engine.hpp>

//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//
bool getFileNames(void *p_data, int p_index, const char** p_file_name);
std::string splitFileName(std::string & str);
bool sortNewest(std::pair<int, std::string> p_i, std::pair<int, std::string> p_j);

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//

Editor::dataSelect::dataSelect(editorWindow * p_parent_editor) 
	: windowBase(p_parent_editor), 
	 m_current_selected_file(-2), m_selected_file(-1), m_visible(true)
{

}

//==============================================================================//
//        Destructor                                                            //
//==============================================================================//

//==============================================================================//
//        Getters & Setters                                                     //
//==============================================================================//

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

void Editor::dataSelect::onRender()
{
	if (!m_visible)
		return;

	//Create ImGUI window
	ImGui::PushItemWidth(-1);
	ImGui::Text("Select a data from the list.");
	ImGui::ListBox("", &m_selected_file, &getFileNames, &m_file_list, static_cast<int>(m_file_list.size()), 10);

	//Load the selected level
	if (ImGui::Button("Load Data from selected file"))
	{
		if (m_selected_file != -1)
		{
			//Load Objects
			if(m_selected_file != m_current_selected_file)
			{
				CSVParser l_parser(m_file_list[m_selected_file].second);
				l_parser.Load();

				m_current_selected_file = m_selected_file;

				//Engine
				auto l_engine = getEngineController().getEngineRunner();
				//SceneSystem/Scene Manipulator
				auto l_scene_manip = l_engine->getEngine()->getSceneManipulator().lock();

				//Instantiate gameObject
				auto l_transform_data = l_parser.getTransformData();
				int l_i = 0;
				for (auto l_start = l_transform_data.begin(); l_start != l_transform_data.end(); ++l_start)
				{
					if(l_i < 500)
					{
						l_i++;
						continue;
					}
					//Create a new gameObject
					l_scene_manip->addRenderedGameObject(l_start->second[0], l_start->second[1], l_start->second[2], l_start->first, "basic.mtl");
				}
			}
		}
	}

	//refresh button
	if(ImGui::Button("Refresh Data List"))
	{
		Refresh();
	}

	//end ImGui loop
}

void Editor::dataSelect::Reset()
{
	//reset all flags
	m_visible = true;
	m_selected_file = -1;
	Refresh();
}

void Editor::dataSelect::Refresh()
{
	m_file_list.clear();

	const std::filesystem::directory_iterator l_end{};

	//Loop through the data/logs folder and validate files and add them to the list of files
	for(auto & p: std::filesystem::recursive_directory_iterator(m_digipen_team_shared_path))
	{
		//if the current file is not a directory
		if(std::filesystem::is_regular_file(p))
		{
			std::filesystem::path l_file_path = p.path();

			if(l_file_path.extension() == ".csv")
			{
				std::string l_thin = l_file_path.generic_string();

				if(ValidateFile(l_thin))
				{
					int l_date;

					l_date = GetFileDate(l_thin);

					m_file_list.emplace_back(std::make_pair(l_date, l_thin));
				}
			}
		}
	}

	//sort list
	std::sort(m_file_list.begin(), m_file_list.end(), sortNewest);
}


///////========================================================================///////
///////   Private                                                              ///////
///////========================================================================///////

//////======================================================================//////
//////    Non-Static                                                        //////
//////======================================================================//////

/////========================================================================/////
/////     Functions                                                          /////
/////========================================================================/////

////==========================================================================////
////      Non-Virtual                                                         ////
////==========================================================================////

//==============================================================================//
//        Helper                                                                //
//==============================================================================//

bool Editor::dataSelect::ValidateFile(std::string p_file_name)
{
	//Maybe will have functionality later
	return true;
}

int Editor::dataSelect::GetFileDate(std::string p_file_path)
{
	int l_date;

	//get the file name
	std::string l_file_name = splitFileName(p_file_path);

	//extract the date by looking from the front of the string to the first '_'
	std::string l_file_date = l_file_name.substr(0, l_file_name.find_first_of('_'));

	//convert
	l_date = std::stoi(l_file_date);
	return l_date;
}

/*!***************************************************************************************
\brief helper function used to std::sort the file list from newest to oldest
\param p_i left
\param p_j right
\return bool - used by std::sort
*****************************************************************************************/
bool sortNewest(std::pair<int, std::string> p_i, std::pair<int, std::string> p_j) { return (p_i.first > p_j.first); }

/*!***************************************************************************************
\brief get all file names from the data/logs folder. used in update for the ImGUI
\param p_data Unknown
\param p_index Unknown
\param p_file_name Unknown
\return bool - always return true, used for ImGui update loop.
*****************************************************************************************/
bool getFileNames(void *p_data, int p_index, const char** p_file_name)
{
	auto *l_file_names = static_cast<std::vector<std::pair<int, std::string>>*>(p_data);
	*p_file_name = ((*l_file_names)[p_index]).second.c_str();
	return true;
}

/*!***************************************************************************************
\brief helper function to split a string by path and file name
\param p_file_path the entire file path
\return string the name of the file
*****************************************************************************************/
std::string splitFileName(std::string & p_file_path)
{
	std::size_t l_found = p_file_path.find_last_of("/\\");
	return p_file_path.substr(l_found + 1);
}
