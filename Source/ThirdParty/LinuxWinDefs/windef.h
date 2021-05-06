/*!***************************************************************************************
\file       windef.h
\author     Tyler Robbins
\date       9/7/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a header file meant to mimic windef.h as found on the Windows OS
*****************************************************************************************/

#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
#include <wchar.h> // wchar_t
#include <limits.h> // PATH_MAX

//========3rd Party Includes============================================================//

//========Types=========================================================================//
typedef int BOOL;

// Do this to maintain compatibility with ODBC libraries (sqltypes.h)
#ifdef SQL_WCHART_CONVERT
typedef wchar_t WCHAR;
#else
typedef unsigned short WCHAR;
#endif

typedef char CHAR;

typedef short SHORT;
typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef long LONG;

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;
typedef void VOID;

typedef VOID *PVOID;
typedef WCHAR *LPWSTR, *PWSTR;
typedef CHAR* LPSTR;

typedef BOOL* LPBOOL;

// TODO: Not certain if these are the right typedefs, but they don't seem to be
//       documented _anywhere_
typedef LPWSTR LPCWCH;
typedef LPSTR LPCCH;

typedef LONG HRESULT;
typedef PVOID HANDLE;
//========Defines=======================================================================//

// Mimic the bare number of error codes in winerror.h that will be needed for
//  XInput.h

#ifndef ERROR_SUCCESS
# define ERROR_SUCCESS 0L
#endif

#ifndef ERROR_DEVICE_NOT_CONNECTED
# define ERROR_DEVICE_NOT_CONNECTED 1167L
#endif

#ifndef ERROR_ACCESS_DENIED
# define ERROR_ACCESS_DENIED 5L
#endif

#ifndef TRUE
# define TRUE 1
#endif

#ifndef FALSE
# define FALSE 0
#endif

#ifndef MAX_PATH
# define MAX_PATH PATH_MAX
#endif

//========Forward Deceleration==========================================================//

/////////========================================================================/////////
///////// Namespace                                                              /////////
/////////========================================================================/////////

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

