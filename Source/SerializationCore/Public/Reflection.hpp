/*!***************************************************************************************
\file       Reflection.hpp
\author     Michael-Paul Moore
\date       6/22/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  This is reflection for basic types, file name will change once more progress is
        made.
*****************************************************************************************/

#pragma once

//========3rd Party Includes============================================================//
#include <map>
#include <vector>
#include <cstring> // std::strcpy
#include <DataTypes.hpp>

////////==========================================================================////////
////////  Classes                                                                 ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: basicTypeHelperBase
\brief   Base for basicTypeHelper<templated>, allows for polymorphism
*****************************************************************************************/
class basicTypeHelperBase
{
	///////============================================================================///////
	///////   Public                                                                   ///////
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

		///====================================================================================///
		///       Helper                                                                       ///
		///====================================================================================///

		/*!***************************************************************************************
		\brief  Initializes the internal list needed for its functionality.
		\return The basic type helpers available.
		*****************************************************************************************/
		static std::map<std::string, basicTypeHelperBase*> basicTypeHelpers();

		//////==============================================================================//////
		//////    Non-Static                                                                //////
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

		/*!***************************************************************************************
		\brief  Copies a basic type
		\param to_coppy - data to copy
		\return copied data
		*****************************************************************************************/
		virtual char* copy(char* to_coppy) = 0;

		/*!***************************************************************************************
		\brief  Creates a copy of data to the correct type from a string.
		\param string_to_coppy - string equivalent of data to copy
		\return copied data
		*****************************************************************************************/
		virtual char* copy_from_string(const std::string& string_to_coppy) = 0;

		/*!***************************************************************************************
		\brief  Does == operator for basic types
		\param left_hand_side - data on the left hand side
		\param right_hand_side - data on the right hand side
		\return Result of '=='
		*****************************************************************************************/
		virtual bool op_eq(const char* left_hand_side, const char* right_hand_side) = 0;

		/*!***************************************************************************************
		\brief  Does output stream for basic types.
		\param os - output stream
		\param data - data to output
		\return What is this returning?
		*****************************************************************************************/
		virtual void output_stream(std::ostream& os, char* data) = 0;

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
	private:

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

		/*!***************************************************************************************
		\brief  Initializes the internal list needed for its functionality.
		*****************************************************************************************/
		static void initialize();

		/////================================================================================/////
		/////     Data                                                                       /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Configurable                                                            ////
		////==================================================================================////

		static std::map<std::string, basicTypeHelperBase*> m_base_types; /*!< Map of all basic
		                                                                 * type helpers. */
};

/*!***************************************************************************************
\par class: type
\brief   Run time type
*****************************************************************************************/
class typeRT final
{
	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////
	public:

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		//Getters for basic types
    float getFloat() const;
    double getDouble() const;
    bool getBool() const;
    std::string getString() const;
    char getChar() const;
		//char* getC_String();
    int getInt() const;
    unsigned int getUInt() const;
    objID getObjID() const;
    unsigned long getULong() const;
    void* getRawData();
    const void* getRawData() const;

		//Setters for basic types
    void setFloat(float p_data);
    void setDouble(double p_data);
    void setBool(bool p_data);
    void setString(std::string p_data);
    void setChar(char p_data);
		//void setC_String(char* p_data);
    void setInt(int p_data);
    void setUInt(unsigned int p_data);
    void setObjID(objID p_data);
    void setULong(unsigned long p_data);
    void setRawData(void* p_data, unsigned int size, std::string p_typeName);

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		////==================================================================================////
		////      Copy                                                                        ////
		////==================================================================================////

		/*!***************************************************************************************
		\brief  Deep copies a type.
		\param to_copy - type to copy
		*****************************************************************************************/
    typeRT(const typeRT& to_copy);
    ~typeRT();
		////==================================================================================////
		////      Conversion                                                                  ////
		////==================================================================================////

		/*!***************************************************************************************
		\brief  Creates a new type.
		\param type_name - name of new type
		*****************************************************************************************/
		explicit typeRT(const std::vector<typeRT>& types) : m_type_name("array"),
		                                                    m_variable_name(""),
		                                                    m_members({}),
		                                                    m_array(types),
		                                                    m_data(nullptr)
        { }




		///====================================================================================///
		///       Constructor                                                                  ///
		///====================================================================================///

		/*!***************************************************************************************
		\brief  Default constructor;
		*****************************************************************************************/
    typeRT() = default;


		typeRT(std::string&& variable_name, const bool& real_bool)
		    : m_type_name("bool"), m_variable_name(variable_name), m_data(nullptr), m_size(sizeof(real_bool))
		{
			m_data = new char[m_size];
			*reinterpret_cast<bool*>(m_data) = real_bool;
        }


		typeRT(std::string&& variable_name, const char& real_char)
		    : m_type_name("char"), m_variable_name(variable_name), m_data(nullptr), m_size(sizeof(real_char))
		{
			m_data = new char[m_size];
			*m_data = real_char;
        }


		typeRT(std::string&& variable_name, int real_int)
		    : m_type_name("int"), m_variable_name(variable_name), m_data(nullptr), m_size(sizeof(real_int))
		{
			m_data = new char[m_size];
			*reinterpret_cast<int*>(m_data) = real_int;
        }


		typeRT(std::string&& variable_name, const float& real_float)
		    : m_type_name("float"), m_variable_name(variable_name), m_data(nullptr), m_size(sizeof(real_float))
        {
			m_data = new char[m_size];
			*reinterpret_cast<float*>(m_data) = real_float;
		}

		typeRT(std::string&& variable_name, const double& real_double)
		    : m_type_name("double"), m_variable_name(variable_name), m_data(nullptr), m_size(sizeof(real_double))
        {
			m_data = new char[m_size];
			*reinterpret_cast<double*>(m_data) = real_double;
		}

		typeRT(std::string&& variable_name, const unsigned int& real_unsigned)
			: m_type_name("unsigned int"), m_variable_name(variable_name),
			m_data(nullptr), m_size(sizeof(real_unsigned))
		{
			m_data = new char[m_size];
			*reinterpret_cast<unsigned int*>(m_data) = real_unsigned;
		}

		typeRT(std::string var_name, std::string data) : m_type_name("string"), m_variable_name(var_name)
		{
			unsigned int size = static_cast<unsigned int>(data.length()) + 1;
			auto buffer = new char[size];
            std::strcpy(buffer, data.c_str());
			m_data = buffer;
			m_size = size;
		}

		typeRT(std::string&& variable_name, objID p_ID)
			: m_type_name("unsigned long"), m_variable_name(variable_name),
			m_data(nullptr), m_size(sizeof(p_ID))
		{
			m_data = new char[m_size];
			*reinterpret_cast<objID*>(m_data) = p_ID;
		}


		typeRT(std::string typeRT_name, const std::vector<typeRT>& types)
		    : m_type_name(typeRT_name), m_variable_name(typeRT_name), m_members({}), m_array(types),
		      m_data(nullptr) { }


		typeRT(std::string&& typeRT_name, std::string&& var_name, const std::vector<typeRT>& types)
		    : m_type_name(typeRT_name), m_variable_name(var_name),
		      m_members({}), m_array(types), m_data(nullptr) { }


		typeRT(const std::string& var_name) : m_type_name(""),
		                                      m_variable_name(var_name),
		                                      m_members({}), m_array({}), m_data(nullptr) { }

		///====================================================================================///
		///       Operators                                                                    ///
		///====================================================================================///

		typeRT& operator=(const typeRT& p_other);

		/*!***************************************************************************************
		\brief  Operator== for a real typeRT and a typeRT.
		\param real_typeRT - real typeRT to check against
		\param right_hand_side - typeRT on the right hand side
		\return What is this returning?
		*****************************************************************************************/
		template <typename T>
		friend bool operator==(const T& real_typeRT, const typeRT& right_hand_side)
		{
			return typeRT(real_typeRT) == right_hand_side;
		}

		/*!***************************************************************************************
		\brief  Checks equality between two typeRTs.
		\param right_hand_side - typeRT on the right hand side
		\return Result of '=='.
		*****************************************************************************************/
		bool operator==(const typeRT& right_hand_side) const;

		/*!***************************************************************************************
		\brief  Checks inequality between a real typeRT and a typeRT.
		\param real_typeRT - real typeRT to check against
		\param right_hand_side - typeRT on the right hand side
		\return Result of !=.
		*****************************************************************************************/
		template <typename T>
		friend bool operator!=(const T& real_typeRT, const typeRT& right_hand_side)
		{
			return typeRT(real_typeRT) != right_hand_side;
		}

		/*!***************************************************************************************
		\brief  Check inequality betwen two typeRTs.
		\param right_hand_side - typeRT on the right hand side
		\return Result of !=.
		*****************************************************************************************/
		bool operator!=(const typeRT& right_hand_side) const;

		/*!***************************************************************************************
		\brief  Output operator overload.
		\param os - output stream
		\param p_typeRT - typeRT to output
		\return Output stream.
		*****************************************************************************************/
		friend std::ostream& operator<<(std::ostream& os, const typeRT& p_typeRT);

		///====================================================================================///
		///       Getters & Setters                                                            ///
		///====================================================================================///

		/*!***************************************************************************************
		\brief  Sets's name of typeRT.
		\param name - new name
		*****************************************************************************************/
    void setTypeName(const std::string & name);

		/*!***************************************************************************************
		\brief  Get's name of typeRT.
		\return Name of typeRT.
		*****************************************************************************************/
    std::string getTypeName() const;

		/*!***************************************************************************************
		\brief  Set name of typeRT.
		\param name - new name
		*****************************************************************************************/
    void setVariableName(const std::string & name);

		/*!***************************************************************************************
		\brief  Gets name of variable.
		\return Name of variable.
		*****************************************************************************************/
    const std::string &getVariableName() const;

		///====================================================================================///
		///       Helper                                                                       ///
		///====================================================================================///

		/*!***************************************************************************************
		\brief  Adds members.
		\param typeRTs - members to insert
		*****************************************************************************************/
    void insertMember(const typeRT p_member);

		/*!***************************************************************************************
		\brief  Checks if typeRT is a base typeRT.
		\return Result of check.
		*****************************************************************************************/
		bool isBase() const;

		/*!***************************************************************************************
		\brief  Gets a member by name
		\param p_member - member variable to get
		\return Member to return.
		*****************************************************************************************/
		typeRT& member(const std::string & p_member);

		/*!***************************************************************************************
		\brief  Gets a member by name
		\param p_member - member variable to get
		\return Member to return.
		*****************************************************************************************/
		const typeRT& member(const std::string & p_member) const;

		/*!***************************************************************************************
		\brief  Gets all members.
		\return Members in this typeRT.
		*****************************************************************************************/
		std::map<std::string, typeRT>& members();

		bool isMember(std::string const& p_member) const;

		/*!***************************************************************************************
		\brief  Gets array.
		\return Array in this typeRT.
		*****************************************************************************************/
		std::vector<typeRT>& array();

		/*!***************************************************************************************
		\brief  Gets array.
		\return Array in this typeRT.
		*****************************************************************************************/
		const std::vector<typeRT>& array() const;

		/*!***************************************************************************************
		\brief  Recursive output stream for typeRT.
		\param os - output stream
		\param tabs - amount of indentation
		*****************************************************************************************/
		void stream(std::ostream& os, unsigned tabs = 0) const;

		/*!***************************************************************************************
		\brief  default bahavior for getting a preformatted string in the CSV style from a typeRT.
		\return string - formatted string in the proper notation for a CSV file. eg. "x, y, z, "
		*****************************************************************************************/
		std::string to_csv_format() const;

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
	private:

		//////==============================================================================//////
		//////    Non-Static                                                                //////
		//////==============================================================================//////

		/////================================================================================/////
		/////     Functions                                                                  /////
		/////================================================================================/////

		////==================================================================================////
		////      Non-Virtual                                                                 ////
		////==================================================================================////

		///====================================================================================///
		///       Helper                                                                       ///
		///====================================================================================///

		/*!***************************************************************************************
		\brief  Converts a vector of typeRTs to a map of typeRTs
		\param members - vector to convert
		\return Map equivalent of input.
		*****************************************************************************************/
		std::map<std::string, typeRT> vector_to_map(const std::vector<typeRT>& members) const;

		/////================================================================================/////
		/////     Data                                                                       /////
		/////================================================================================/////

		////==================================================================================////
		////      Configurable                                                                ////
		////==================================================================================////

		std::string                   m_type_name;     //!< name of real typeRT
		std::string                   m_variable_name; //!< variable name
		std::map<std::string, typeRT> m_members;       //!< member variables in this typeRT
		std::vector<typeRT>           m_array;         //!< Array, for array form

		////==================================================================================////
		////      Non-Configurable                                                            ////
		////==================================================================================////

		char* m_data = nullptr; //!< Pointer to real data (null if typeRT is not a basic typeRT).
		unsigned int m_size = 0; //size of data
};
