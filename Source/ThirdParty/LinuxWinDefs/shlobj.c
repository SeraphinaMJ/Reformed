
#include "shlobj.h"
#include "winerror.h"

#include <stdlib.h> // mbstowcs
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>

// Maximum path we will be using
static char _TEMP_CHAR_ARRAY[1024] = { 0 };

static struct passwd* _userPW = NULL;

static struct passwd* getUserPW() {
    if(_userPW == NULL) {
        _userPW = getpwuid(getuid());
    }

    return _userPW;
}

/**
 * @brief Retrieves the full path of a known folder identified by its KNOWNFOLDERID
 * @details See: https://docs.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
 *
 * @param rfid A reference to the KNOWNFOLDERID that identifies this folder
 * @param dwFlags A flag which specifies special retrieval options. Can be 0 or one of KNOWN_FOLDER_FLAG.
 * @param HANDLE Access token for a user. Can be NULL. Not used on Linux.
 * @param ppszPath A pointer to the string to place the path of the known folder into.
 *
 * @return Returns S_OK if successful, or an error code of E_FAIL or E_INVALID_ARG
 */
HRESULT _SHGetKnownFolderPath(const KNOWNFOLDERID* rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath)
{
    (void)hToken;

    /*
     * Error Codes:
     * ===========
     * E_FAIL: Among other things, this value can indicate that the rfid
     *         parameter references a KNOWNFOLDERID which does not have a path
     *         (such as a folder marked as KF_CATEGORY_VIRTUAL).
     * E_INVALID_ARG: Among other things, this value can indicate that the rfid
     *                parameter references a KNOWNFOLDERID that is not present
     *                on the system. Not all KNOWNFOLDERID values are present on
     *                all systems. Use IKnownFolderManager::GetFolderIds to retrieve
     *                the set of KNOWNFOLDERID values for the current system.
     */

    if(IsEqualGUID(*rfid, FOLDERID_AccountPictures)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_AddNewPrograms)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_AdminTools)) {
        memcpy(*ppszPath, L"/bin", sizeof(wchar_t) * wcslen(L"/bin") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_AppsFolder)) {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Applications") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Applications", desktop_len);
        _TEMP_CHAR_ARRAY[hd_len + desktop_len] = '\0';

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_ApplicationShortcuts)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_AppUpdates)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_CDBurning)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ChangeRemovePrograms)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonAdminTools)) {
        memcpy(*ppszPath, L"/bin", sizeof(wchar_t) * wcslen(L"/bin") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonOEMLinks)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonPrograms) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFiles) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFilesX64) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFilesX86) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFilesCommon) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFilesCommonX64) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramFilesCommonX86) ||
              IsEqualGUID(*rfid, FOLDERID_UserProgramFiles) ||
              IsEqualGUID(*rfid, FOLDERID_UserProgramFilesCommon) ||
              IsEqualGUID(*rfid, FOLDERID_Programs))
    {
        memcpy(*ppszPath, L"/opt", sizeof(wchar_t) * wcslen(L"/opt") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonStartMenu)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonStartup)) {
        memcpy(*ppszPath, L"/etc/init.d", sizeof(wchar_t) * wcslen(L"/etc/init.d") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_CommonTemplates)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ComputerFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ConflictFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ConnectionsFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Contacts)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ControlPanelFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Cookies)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Desktop)) {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Desktop") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Desktop", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_DeviceMetadataStore)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Documents) ||
              IsEqualGUID(*rfid, FOLDERID_DocumentsLibrary))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Documents") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Documents", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_Downloads)) {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Downloads") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Downloads", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_Favorites)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Fonts)) {
        memcpy(*ppszPath, L"/usr/share/fonts", sizeof(wchar_t) * wcslen(L"/usr/share/fonts") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_Games)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_GameTasks)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_History)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_HomeGroup)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_HomeGroupCurrentUser)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ImplicitAppShortcuts)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_InternetCache)) {
        memcpy(*ppszPath, L"/tmp", sizeof(wchar_t) * wcslen(L"/tmp") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_InternetFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Libraries)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Links)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_LocalAppData) ||
              IsEqualGUID(*rfid, FOLDERID_LocalAppDataLow))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/.local") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/.local", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_LocalizedResourcesDir)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Music) ||
              IsEqualGUID(*rfid, FOLDERID_MusicLibrary) ||
              IsEqualGUID(*rfid, FOLDERID_Playlists) ||
              IsEqualGUID(*rfid, FOLDERID_SamplePlaylists) ||
              IsEqualGUID(*rfid, FOLDERID_SampleMusic))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Music") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Music", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_NetHood)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_NetworkFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_OriginalImages) ||
              IsEqualGUID(*rfid, FOLDERID_PhotoAlbums) ||
              IsEqualGUID(*rfid, FOLDERID_Pictures) ||
              IsEqualGUID(*rfid, FOLDERID_SamplePictures) ||
              IsEqualGUID(*rfid, FOLDERID_Screenshots) ||
              IsEqualGUID(*rfid, FOLDERID_PicturesLibrary))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Pictures") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Pictures", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_PrintHood)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PrintersFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Profile) ||
              IsEqualGUID(*rfid, FOLDERID_ProgramData))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        mbstowcs(*ppszPath, home_dir, hd_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_Public)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicDesktop)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicDocuments)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicDownloads)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicGameTasks)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicLibraries)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicMusic)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicPictures)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicRingtones)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicUserTiles)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_PublicVideos)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_QuickLaunch)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Recent)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_RecordedTVLibrary)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_RecycleBinFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_ResourceDir)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Ringtones)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_RoamingAppData)) {
        memcpy(*ppszPath, L"/usr/local/share", sizeof(wchar_t) * wcslen(L"/usr/local/share") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_RoamingTiles)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_RoamedTileImages)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SavedGames)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SavedSearches)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SEARCH_MAPI)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SEARCH_CSC)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SearchHome)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SendTo)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SidebarDefaultParts)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SidebarParts)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_StartMenu)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_Startup)) {
        memcpy(*ppszPath, L"/etc/init.d", sizeof(wchar_t) * wcslen(L"/etc/init.d") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_SyncManagerFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SyncResultsFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_SyncSetupFolder)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_System) ||
              IsEqualGUID(*rfid, FOLDERID_SystemX86))
    {
        memcpy(*ppszPath, L"/sys", sizeof(wchar_t) * wcslen(L"/sys") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_Templates)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_UserPinned)) {
        return E_INVALID_ARG;
    } else if(IsEqualGUID(*rfid, FOLDERID_UserProfiles) ||
              IsEqualGUID(*rfid, FOLDERID_UsersLibraries) ||
              IsEqualGUID(*rfid, FOLDERID_UsersFiles))
    {
        memcpy(*ppszPath, L"/home", sizeof(wchar_t) * wcslen(L"/home") + 1);
    } else if(IsEqualGUID(*rfid, FOLDERID_Videos) ||
              IsEqualGUID(*rfid, FOLDERID_SampleVideos) ||
              IsEqualGUID(*rfid, FOLDERID_VideosLibrary))
    {
        char* home_dir = getUserPW()->pw_dir;
        size_t hd_len = strlen(home_dir);
        size_t desktop_len = strlen("/Videos") + 1;

        memcpy(_TEMP_CHAR_ARRAY, home_dir, hd_len);
        memcpy(_TEMP_CHAR_ARRAY + hd_len, "/Videos", desktop_len);

        mbstowcs(*ppszPath, _TEMP_CHAR_ARRAY, hd_len + desktop_len);
    } else if(IsEqualGUID(*rfid, FOLDERID_Windows)) {
        memcpy(*ppszPath, L"/bin", sizeof(wchar_t) * wcslen(L"/bin") + 1);
    } else {
        return E_INVALID_ARG;
    }

    // Should we verify that the directory exists?
    if((dwFlags & KF_FLAG_DONT_VERIFY) == 0) {
        wcstombs(_TEMP_CHAR_ARRAY, *ppszPath, wcslen(*ppszPath) + 1);
        DIR* dir = opendir(_TEMP_CHAR_ARRAY);
        struct stat st = { 0 };

        // Did we fail to stat it?
        if(stat(_TEMP_CHAR_ARRAY, &st) == -1) {
            // First, should we attempt to create it?
            //  If we should (dwFlags & KF_FLAG_CREATE != 0), did mkdir fail?
            if((dwFlags & KF_FLAG_CREATE) == 0 || mkdir(_TEMP_CHAR_ARRAY, 0700) == -1)
                return E_FAIL;
        }
    }

    return S_OK;
}

