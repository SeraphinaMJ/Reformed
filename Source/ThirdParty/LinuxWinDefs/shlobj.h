/*!***************************************************************************************
\file       shlobj.h
\author     Tyler Robbins
\date       11/21/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Minimal re-implementation of windows.h from the Win32 API.
*****************************************************************************************/

#pragma once

#define SQL_WCHART_CONVERT

#include "windef.h"
#include "knownfolders.h"

typedef enum {
    KF_FLAG_DEFAULT,
    KF_FLAG_FORCE_APP_DATA_REDIRECTION,
    KF_FLAG_RETURN_FILTER_REDIRECTION_TARGET,
    KF_FLAG_FORCE_PACKAGE_REDIRECTION,
    KF_FLAG_NO_PACKAGE_REDIRECTION,
    KF_FLAG_FORCE_APPCONTAINER_REDIRECTION,
    KF_FLAG_NO_APPCONTAINER_REDIRECTION,
    KF_FLAG_CREATE,
    KF_FLAG_DONT_VERIFY,
    KF_FLAG_DONT_UNEXPAND,
    KF_FLAG_NO_ALIAS,
    KF_FLAG_INIT,
    KF_FLAG_DEFAULT_PATH,
    KF_FLAG_NOT_PARENT_RELATIVE,
    KF_FLAG_SIMPLE_IDLIST,
    KF_FLAG_ALIAS_ONLY
} KNOWN_FOLDER_FLAG;

// https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath

#ifdef __cplusplus
extern "C" {
#endif

HRESULT _SHGetKnownFolderPath(const KNOWNFOLDERID* rfid, DWORD dwFlags, HANDLE hToken, PWSTR* ppszPath);

#ifdef __cplusplus
}
#endif

#define SHGetKnownFolderPath(rfid, dwFlags, hToken, ppszPath) \
    _SHGetKnownFolderPath(&rfid, dwFlags, hToken, ppszPath)

