/*!***************************************************************************************
\file       DLLControl.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#if defined(CREATE_TEMPESTENGINE_DLL)
#   define TEMPESTENGINE_API __declspec(dllexport)
#else
#   define TEMPESTENGINE_API __declspec(dllimport)
#endif
