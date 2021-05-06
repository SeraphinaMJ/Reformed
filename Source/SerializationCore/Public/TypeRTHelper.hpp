#include "Reflection.hpp"

#include <typeindex>
#include <typeinfo>

template <typename T>
T getValue(typeRT & data)
{
    auto& typeOfData = typeid(T);
    if (typeOfData == typeid(float))
    {
        return data.getFloat();
    }
    else if (typeOfData == typeid(double))
    {
        return data.getDouble();
    }
    else if (typeOfData == typeid(bool))
    {
        return data.getBool();
    }
    else if (typeOfData == typeid(char))
    {
        return data.getChar();
    }
    else if (typeOfData == typeid(int))
    {
        return data.getInt();
    }
    else if (typeOfData == typeid(unsigned int))
    {
        return data.getUInt();
    }
    //float getFloat() const;
    //double getDouble() const;
    //bool getBool() const;
    //std::string getString() const;
    //char getChar() const;
    ////char* getC_String();
    //int getInt() const;
    //unsigned int getUInt() const;

    throw std::runtime_error("Type not recognized");
}