/*!***************************************************************************************
\file       CSVParser.cpp
\author     Ryan Booth
\date       9/21/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		This is Cole's beautifully designed Telemetry Data Parser - Object Creator.
*****************************************************************************************/

//========Self Include==================================================================//
#include "CSVParser.hpp"
//========1st Party Includes============================================================//
//========3rd Party Includes============================================================//
#include <fstream>
//========Types=========================================================================//
//========Defines=======================================================================//
//========Static Deceleration===========================================================//

///////========================================================================///////
///////   Public                                                               ///////
///////========================================================================///////

//==============================================================================//
//        Constructor                                                           //
//==============================================================================//
CSVParser::CSVParser(std::string p_file_name)
{
	m_file_name = p_file_name;

	int l_place = static_cast<int>(m_file_name.find('_', 0));

	m_component_name = m_file_name.substr(l_place, m_file_name.size() - 4);
}


/*!***************************************************************************************
\brief 
*****************************************************************************************/
void CSVParser::Load()
{
	std::fstream fs;
	fs.open(m_file_name, std::fstream::in);

	if(fs.is_open())
	{
		//line format: '<object_id>, <x>, <y>, <z>, '
		if (m_file_name.find("transform"))
		{
			std::string l_current_line;

			while (!fs.eof())
			{
				if (std::getline(fs, l_current_line))
				{

					//vector3 l_data;
					std::vector<float> l_data;
					std::string l_object_id;

					////find the index of the first comma
					//int l_current_comma = l_current_line.find(',', 0);

					////get the object id as a string
					//l_object_id = l_current_line.substr(0, l_current_comma);

					////
					//l_current_comma = l_current_line.find(',', l_current_comma);

					int l_current_comma = 0;
					for (int i = 0; i < 4; ++i)
					{
						//find next comma
						l_current_comma = static_cast<int>(l_current_line.find(',', l_current_comma));

						//if first comma, store as the object id
						if (i == 0)
						{
							l_object_id = l_current_line.substr(0, l_current_comma);
						}
						else
						{
							//else, get the string up to the next comma, should represent data
							int l_temp2 = static_cast<int>((l_current_line.find(',', l_current_comma + 1))) - l_current_comma - 1;
							std::string l_temp = l_current_line.substr(l_current_comma + 1, l_temp2);
							l_current_comma = l_current_comma + l_temp2 + 1;
							l_data.emplace_back(std::stof(l_temp));
						}
					}

					m_transform_data.emplace_back(std::make_pair(l_object_id, l_data));
				}
			}
		}


		fs.close();
	}

}

std::vector<std::pair<std::string, std::vector<float>>>& CSVParser::getTransformData()
{
	return m_transform_data;
}

