/*!***************************************************************************************
\file       Reflection.cpp
\author     Michael-Paul Moore
\date       6/29/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is the implementation of reflection, excluding templated functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Reflection.hpp"

//========3rd Party Includes============================================================//
#include <sstream>
#include <string>
#include <Logger.hpp>
#include <cassert>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: basicTypeGuides
\brief   Templated for basic types functionality, helps reflection functionality
*****************************************************************************************/
template <typename T>
class basicTypeHelper final : public basicTypeHelperBase
{
	///////============================================================================///////
	///////   Public                                                                  ///////
	///////============================================================================///////
	public:

		//////==============================================================================//////
		//////    Static                                                                    //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Get's, or creates if not available, an instance of this typeRT.
		\return The instance?
		*****************************************************************************************/
		static basicTypeHelperBase* instance()
		{
			if (m_instance)
				return m_instance;
			return m_instance = new basicTypeHelper();
		}

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
	private:

		//////==============================================================================//////
		//////    Static                                                                    //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Data                                                                       /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Configurable                                                            ////
		////==================================================================================////

		static basicTypeHelper* m_instance; //!< singleton instance of this typeRT

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Constructor                                                                 ///
		//======================================================================================//

		/*!***************************************************************************************
		\brief  default constrictor.
		*****************************************************************************************/
		basicTypeHelper() = default;

		////==================================================================================////
		////      Overridden                                                                  ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		/*!***************************************************************************************
		\brief  Copies a basic typeRT
		\param to_coppy - data to copy
		\return copied data
		*****************************************************************************************/
		char* copy(char* to_coppy) override
		{
			return reinterpret_cast<char*>(new T(*reinterpret_cast<T*>(to_coppy)));
		}

		/*!***************************************************************************************
		\brief  Creats a coppy of data to the corect typeRT from a string.
		\param string_to_coppy - string equivalent of data to copy
		\return copied data
		*****************************************************************************************/
		char* copy_from_string(const std::string& string_to_coppy) override
		{
			T l_data;
			std::istringstream(string_to_coppy) >> l_data;
			return reinterpret_cast<char*>(new T(l_data));
		}

		/*!***************************************************************************************
		\brief  Does == operator for basic types
		\param lh_side - data on the left hand side
		\param rh_side - data on the right hand side
		\return Result of '=='
		*****************************************************************************************/
		bool op_eq(const char* lh_side, const char* rh_side) override
		{
			return *reinterpret_cast <const T*>(lh_side) == *reinterpret_cast <const T*>(rh_side);
		}

		/*!***************************************************************************************
		\brief  Does output stream for basic types.
		\param os - output stream
		\param data - data to output
		\return What is this returning?
		*****************************************************************************************/
		void output_stream(std::ostream& os, char* data) override
		{
			os << *reinterpret_cast<T*>(data);
		}
};

template<typename T>
basicTypeHelper<T>* basicTypeHelper<T>::m_instance = nullptr;

////////==========================================================================////////
////////  Class (basicTypeHelperBase)                                             ////////
////////==========================================================================////////

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Static                                                                    //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		std::map<std::string, basicTypeHelperBase*> basicTypeHelperBase::basicTypeHelpers()
		{
			basicTypeHelperBase::initialize();
			return m_base_types;
		}

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Static                                                                    //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Pure-Virtual                                                                ////
		////==================================================================================////

		///====================================================================================///
		///       Helper                                                                       ///
		///====================================================================================///

		void basicTypeHelperBase::initialize()
		{
			// Used Basic types, https://www.tutorialspoint.com/cplusplus/cpp_data_types.htm

			m_base_types["bool"] = basicTypeHelper<bool>::instance();
			m_base_types["char"] = basicTypeHelper<char>::instance();
			m_base_types["int"] = basicTypeHelper<int>::instance();
			m_base_types["float"] = basicTypeHelper<float>::instance();
			m_base_types["double"] = basicTypeHelper<double>::instance();
			m_base_types["string"] = basicTypeHelper<std::string>::instance();
			m_base_types["unsigned long"] = basicTypeHelper<unsigned long>::instance();
		}

float typeRT::getFloat() const
{
	return *reinterpret_cast<float*>(m_data);
}

double typeRT::getDouble() const
{
	return *reinterpret_cast<double*>(m_data);
}

bool typeRT::getBool() const
{
	return *reinterpret_cast<bool*>(m_data);
}

std::string typeRT::getString() const
{
	return std::string{ reinterpret_cast<char*>(m_data) };
}

char typeRT::getChar() const
{
	return *reinterpret_cast<char*>(m_data);
}


int typeRT::getInt() const
{
	return *reinterpret_cast<int*>(m_data);
}

unsigned typeRT::getUInt() const
{
	return *reinterpret_cast<unsigned int*>(m_data);
}

objID typeRT::getObjID() const
{
	// std::cout << "Internal type_name == " << m_type_name << std::endl;
	// std::cout << "\"" << m_variable_name << "\".getObjID() is at " << (void*)m_data << std::endl;
	return *reinterpret_cast<objID*>(m_data);
}

unsigned long typeRT::getULong() const
{
	return *reinterpret_cast<unsigned long*>(m_data);
}

void* typeRT::getRawData()
{
	return m_data;
}

const void* typeRT::getRawData() const
{
	return m_data;
}

void typeRT::setFloat(float p_data)
{
	if(m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(float);
	m_data = new char[m_size];
	*reinterpret_cast<float*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new float(p_data));
	m_type_name = "float";
}

void typeRT::setDouble(double p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(double);
	m_data = new char[m_size];
	*reinterpret_cast<double*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new double(p_data));
	m_type_name = "double";
}

void typeRT::setBool(bool p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(bool);
	m_data = new char[m_size];
	*reinterpret_cast<bool*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new bool(p_data));
	m_type_name = "bool";
}

void typeRT::setString(std::string p_data)
{
	if(m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	auto buffer = new char[p_data.size() + 1];
	strcpy(buffer, p_data.c_str());
	m_data = buffer;
	m_size = static_cast<int>(p_data.size()) + 1;
	m_type_name = "string";
}

void typeRT::setChar(char p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(char);
	m_data = new char[m_size];
	*reinterpret_cast<char*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new char(p_data));
	m_type_name = "char";
}


void typeRT::setInt(int p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(int);
	m_data = new char[m_size];
	*reinterpret_cast<int*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new int(p_data));
	m_type_name = "int";
}

void typeRT::setUInt(unsigned int p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(unsigned int);
	m_data = new char[m_size];
	*reinterpret_cast<unsigned int*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new unsigned int(p_data));
	m_type_name = "unsigned int";
}


void typeRT::setObjID(objID p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(objID);
	m_data = new char[m_size];
	*reinterpret_cast<objID*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new objID(p_data));
	m_type_name = "unsigned long";
}

void typeRT::setULong(unsigned long p_data)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = sizeof(unsigned long);
	m_data = new char[m_size];
	*reinterpret_cast<unsigned long*>(m_data) = p_data;
	// m_data = reinterpret_cast<char*>(new unsigned long(p_data));
	m_type_name = "unsigned long";
}

void typeRT::setRawData(void* p_data, unsigned int p_size, std::string p_typeName)
{
	if (m_data)
	{
		delete m_data;
		m_data = nullptr;
	}
	m_size = p_size;

	m_data = new char[p_size];
	std::memcpy(m_data, p_data, p_size);
	// m_data = reinterpret_cast<char*>(p_data);
	m_type_name = p_typeName;
}

std::map<std::string, basicTypeHelperBase*> basicTypeHelperBase::m_base_types = {};

////////==========================================================================////////
////////  Class (typeRT)                                                            ////////
////////==========================================================================////////

	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		////==================================================================================////
		////      Copy                                                                        ////
		////==================================================================================////
		typeRT::typeRT(const typeRT& to_copy) : m_type_name(to_copy.m_type_name),
		                                  m_variable_name(to_copy.m_variable_name),
		                                  //m_members(to_copy.m_members), m_array(to_copy.m_array),
																			m_size(to_copy.m_size)

		{
			if (to_copy.m_data)
			{
				m_data = new char[to_copy.m_size];
				// std::cout << "Memory for " << m_variable_name << " allocated at " << (void*)m_data << std::endl;
				memcpy(m_data, to_copy.m_data, to_copy.m_size);
			}

			// deep copy members and array
			for (auto & l_member : to_copy.m_members)
				m_members.insert_or_assign(l_member.first, typeRT(l_member.second));

			for (auto & l_entry : to_copy.m_array)
				m_array.push_back(typeRT(l_entry));
		}

typeRT::~typeRT()
{
	m_members.clear();
	m_array.clear();
	if(m_data)
	{
		delete [] m_data;
		m_data = nullptr;
	}
}

typeRT& typeRT::operator=(const typeRT& p_other)
{
			if(m_data)
			{
				delete[] m_data;
				m_data = nullptr;
			}

			m_type_name = p_other.m_type_name;
			m_variable_name = p_other.m_variable_name;
			m_size = p_other.m_size;

      //Clear out members and array list
      m_members.clear();
      m_array.clear();

			if(p_other.m_data)
			{
				m_data = new char[p_other.m_size];
				memcpy(m_data, p_other.m_data, p_other.m_size);
			}

			// deep copy members and array
			for (auto & l_member : p_other.m_members)
				m_members.insert_or_assign(l_member.first, typeRT(l_member.second));

			for (auto & l_entry : p_other.m_array)
				m_array.push_back(typeRT(l_entry));

			return *this;
}

//======================================================================================//
		//        Operators                                                                    ///
		//======================================================================================//
		bool typeRT::operator==(const typeRT & rh) const
		{
			if (!m_members.empty())
			{
				for (const auto& member : m_members)
					if (!(member.second == rh.m_members.find(member.first)->second))
						return false;
				return true;
			}

			// at the base typeRT

			return basicTypeHelperBase::basicTypeHelpers()[m_type_name]->op_eq(reinterpret_cast<char*>(m_data), reinterpret_cast<char*>(rh.m_data));
		}

		bool typeRT::operator!=(const typeRT & right_hand_side) const
		{
			return !(*this == right_hand_side);
		}

		std::ostream & operator<<(std::ostream & os, const typeRT & p_type)
		{
			p_type.stream(os);
			return os;
		}

		//======================================================================================//
		//        Getters & Setters                                                            ///
		//======================================================================================//

		void typeRT::setTypeName(const std::string & name)
		{
			m_type_name = name;
		}

		std::string typeRT::getTypeName() const
		{
			return m_type_name;
		}

		void typeRT::setVariableName(const std::string & name)
		{
			m_variable_name = name;
		}

		const std::string &typeRT::getVariableName() const
		{
			return m_variable_name;
		}

		//======================================================================================//
		//        Helper                                                                        //
		//======================================================================================//

		void typeRT::insertMember(const typeRT p_member)
		{
			m_members[p_member.m_variable_name] = p_member;
		}

		bool typeRT::isBase() const
		{
			if (m_members.empty() && m_data)
				return true;
			if (!m_members.empty() && !m_data)
				return false;
			if (!m_members.empty() && m_data)
				{ assert("Type Incorrect. Cannot Have members & data"); return false; }
			else
				{ assert("Type Incorrect. Cannot have no members and no data"); return false; }
		}

		typeRT & typeRT::member(const std::string & p_member)
		{
			return m_members.find(p_member)->second;
		}

		const typeRT & typeRT::member(const std::string & p_member) const
		{
			return m_members.find(p_member)->second;
		}

		std::map<std::string, typeRT>& typeRT::members()
		{
			return m_members;
		}

		bool typeRT::isMember(std::string const & p_member) const
		{
			return m_members.find(p_member) != m_members.cend();
		}

		std::vector<typeRT>& typeRT::array()
		{
			return m_array;
		}

		const std::vector<typeRT>& typeRT::array() const
		{
			return m_array;
		}

		void typeRT::stream(std::ostream & os, unsigned tabs) const
		{
			for (unsigned i = 0; i < tabs; ++i)
				os << "	";
			os << m_type_name << " " << m_variable_name;
			if (m_data)
			{
				os << " = ";
				basicTypeHelperBase::basicTypeHelpers()[m_type_name]->output_stream(os, reinterpret_cast<char*>(m_data));
				os << std::endl;
			}
			else if(!m_array.empty())
			{
				os << "[]" << std::endl;
				for (const auto& i_type_rt : m_array)
					i_type_rt.stream(os, tabs + 1);
			}
			else
			{
				os << std::endl;
				for (const auto& member : m_members)
					member.second.stream(os, tabs + 1);
			}
		}


		std::string typeRT::to_csv_format() const
		{
			std::stringstream buffer;

			//if basic data, append to output stream
			if (m_data)
			{
				basicTypeHelperBase::basicTypeHelpers()[m_type_name]->output_stream(buffer, reinterpret_cast<char*>(m_data));
			}

			//convert os to a string
			std::string result = buffer.str() + ", ";

			//return that string
			return result;
		}


		std::map<std::string, typeRT> typeRT::vector_to_map(const std::vector<typeRT>& members) const
		{
			std::map<std::string, typeRT> result;
			for (const auto& member : members)
				result[member.m_variable_name] = member;
			return result;
		}
