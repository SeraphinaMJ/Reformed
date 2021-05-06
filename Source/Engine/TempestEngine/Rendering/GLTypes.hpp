#pragma once

/*!***************************************************************************************
\file       GLTypes.hpp
\author     Patrick Cook
\date       11/14/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Tempest Engine
\brief forward declartions for GL types because glew.h is a heavyweight header
*****************************************************************************************/

#include <cstdint> // int, uint
#include <cstddef> // ptrdiff_t

// https://www.khronos.org/opengl/wiki/OpenGL_Type

// basic types
using GLboolean = std::uint8_t; // standard says it should be as few bits as possible
using GLenum    =  std::uint32_t;

// signed integer types
using GLchar = char;
using GLbyte  = std::int8_t;
using GLshort = std::int16_t;
using GLint   = std::int32_t;
using GLint64 = std::int64_t;

// unsigned integer types
using GLubyte  = std::uint8_t;
using GLushort = std::uint16_t;
using GLuint   = std::uint32_t;
using GLuint64 = std::uint64_t;

// special integer types
using GLfixed = std::int32_t; // internally used as a fixed-point number

// pointer types
using GLintptr   = std::ptrdiff_t;
using GLsizeiptr = std::ptrdiff_t;

// miscellanious
using GLbitfield = std::uint32_t;

// floating point types
using GLhalf   = std::uint16_t; // type-punned interally by GL to be a 16-bit float type
using GLfloat  = float;
using GLdouble = double;
using GLclampf = float;  // value is clamped to [0,1] by the caller when passed or returned
using GLclampd = double; // value is clamped to [0,1] by the caller when passed or returned
