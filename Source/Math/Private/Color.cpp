/*!***************************************************************************************
\file       Color.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Used to make basic colors
*****************************************************************************************/

//========Self Include==================================================================//
#include "Color.hpp"
//========Third-Party Include===========================================================//
#include <cstring>

//========Static Deceleration==========================================================//
color const color::red(1, 0, 0);
color const color::green(0, 1, 0);
color const color::blue(0, 0, 1);
color const color::yellow(1, 1, 0);
color const color::cyan(0, 1, 1);
color const color::magenta(1, 0, 1);
color const color::white(1, 1, 1);
color const color::black(0, 0, 0);
color const color::gray(0.5f, 0.5f, 0.5f);
