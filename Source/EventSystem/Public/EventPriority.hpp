/*!****************************************************************************
@file EventPriority.h
@author AUTHOR

@brief DESCRIPTION

@par All content © 2018 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#pragma once

namespace EventSystem3
{
    enum class Priority
    {
        LOWEST,
        LOW,
        NORMAL,
        HIGH,
        HIGHEST
    };
}

#define ES3_PRI_() NORMAL

#define ES3_PRI_LOW() LOW
#define ES3_PRI_LOWEST() LOWEST
#define ES3_PRI_NORMAL() NORMAL
#define ES3_PRI_HIGH() HIGH
#define ES3_PRI_HIGHEST() HIGHEST
