/*!***************************************************************************************
\file       LeakDetection.hpp
\author     Aaron Damyen
\date       5/24/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief  Sets up the Visual Leak Detector dependency
*****************************************************************************************/
#pragma once

// Uncomment this if running Dr.Memory to clean up the report list
#define NO_VLD

#ifndef NO_VLD  // check manual disable switch
#ifdef _DEBUG   // disable in release modes
#include <vld.h>
#endif
#endif
#define MEM_DEBUG 1
