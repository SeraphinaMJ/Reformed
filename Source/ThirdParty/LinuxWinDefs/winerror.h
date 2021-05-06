
#pragma once

#include "windef.h"

#ifndef _HRESULT_TYPEDEF_
# define _HRESULT_TYPEDEF_(_sc) ((HRESULT)_sc)
#endif

#ifndef S_OK
# define S_OK ((HRESULT)0x00000000)
#endif

#ifndef E_FAIL
# define E_FAIL _HRESULT_TYPEDEF_(0x80004005)
#endif


#ifndef E_INVALID_ARG
# define E_INVALID_ARG _HRESULT_TYPEDEF_(0x80070057)
#endif

