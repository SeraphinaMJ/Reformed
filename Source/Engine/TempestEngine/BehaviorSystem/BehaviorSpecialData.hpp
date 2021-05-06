/*!***************************************************************************************
\file       BehaviorSpecialData.hpp
\author     Charles Boudousquie
\date       11/7/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This POD contains metadata about a behavior, mainly for Node Graph Editor
            purposes.
*****************************************************************************************/
#pragma once

enum class DATA_TYPE { BOOL, INT, UNSIGNED_INT, FLOAT, DOUBLE };

struct SpecialData
{
    struct Entry
    {
        DATA_TYPE type;
        const char* name;
        std::any data;
    };

    std::vector<Entry> entries;
};
