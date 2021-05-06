/*!***************************************************************************************
\file       DLLControl.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#if defined(CREATE_BULLET_DLL)
#   define BULLET_API __declspec(dllexport)
#else
#   define BULLET_API __declspec(dllimport)
#endif
