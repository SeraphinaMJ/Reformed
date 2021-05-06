/*!***************************************************************************************
\file       Blackboard.hpp
\author     Charles Boudousquie
\date       10/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class is the general local storage that an AI/Actor can use throughout
            the game.
*****************************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include <any>

class Blackboard
{
public:
    template<typename T>
    T getValue(const std::string& variableName);

    template<typename T>
    void setValue(const std::string& variableName, T value);

private:
    std::unordered_map<std::string, std::any> blackboard;
};

template<typename T>
T Blackboard::getValue(const std::string& variableName)
{
    if (blackboard.find(variableName) != blackboard.end())
    {
        return std::any_cast<T>( blackboard[variableName]);
    }
    else
    {
        std::string message = "Tried to set value: " + variableName + " in blackboard";
        throw std::runtime_error(message);
    }
}

template<typename T>
void Blackboard::setValue(const std::string& variableName, T value)
{
    // get reference then set value
    blackboard[variableName] = value;
}