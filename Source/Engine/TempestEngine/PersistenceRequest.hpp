/*!***************************************************************************************
\file       PersistenceRequest.hpp
\author     David Roberts
\date       9/28/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief		Pure virtual base class for persistence requests. Used by PersistenceSystem.
*****************************************************************************************/
#pragma once
#include "PersistenceSystem.hpp"

enum Direction
{
	Read,
	Write,
    Delete
};

/*!***************************************************************************************
\par class: persistenceRequest
\brief
*****************************************************************************************/
template <typename T>
class persistenceRequest
{

public:

	/*!***************************************************************************************
	\brief Constructor used for persistence requests.
	\param p_folder_name - File path to the folder.
	\param p_file_name - Name of the file to read/write.
	\param p_data - Data to write/Object to attach read data to.
	\param p_direction - Read or Write
	*****************************************************************************************/
	persistenceRequest(Direction p_direction, const std::string p_folder_name, const std::string p_file_name = "", std::shared_ptr<T> p_data = nullptr) :
		m_direction(p_direction), m_folder_name(p_folder_name), m_file_name(p_file_name), m_data(p_data)
	{
	}

	/*!***************************************************************************************
	\brief Executes a read or write request.
	*       Templated class must implement appropriate read/write function.
	*****************************************************************************************/
	void execute(std::string relative_path)
	{
		m_relative_path = relative_path;
		if (m_direction == Read)
		{
			m_data = T::read(this);
		}
		else
		{
			m_data->write(this);
		}
	}

	void writeDataToFile(const std::string p_data)
	{
		persistenceSystem::writeToFile(p_data, m_relative_path + m_folder_name, m_file_name);
	}

	typeRT readDataFromFile()
	{
		return persistenceSystem::readFromFile(m_relative_path + m_folder_name, m_file_name);
	}
	std::shared_ptr<T> getData(){ return m_data; }
	const std::string getFolderName() { return m_folder_name; }
	const std::string getFileName() { return m_file_name; }
	const std::string getRelativePath() { return m_relative_path; }
private:
	Direction m_direction;
	const std::string  m_folder_name;
	const std::string  m_file_name;
	std::shared_ptr<T> m_data;
	std::string m_relative_path = "";

};
