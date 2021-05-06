/*!***************************************************************************************
\file       BehaviorLogger.hpp
\author     Charles Boudousquie
\date       10/25/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class logs info for behaviors.
*****************************************************************************************/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

class BehaviorLogger
{
    BehaviorLogger() {}

    std::map<std::string, std::vector<std::string>> messages;

public:

    static BehaviorLogger& GetInstance()
    {
        static BehaviorLogger instance;
        return instance;
    }

    void addMessage(const std::string& message, const std::string& agentName)
    {
        messages[agentName].push_back(message);
    }


private:

};