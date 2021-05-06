/*!***************************************************************************************
\file       FileOpener.hpp
\author     Ryan Booth
\date       11/14/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      
*****************************************************************************************/

//https://github.com/Flix01/imgui/tree/imgui_with_addons/addons/imguifilesystem

#pragma once
#include <string>
#include <filesystem>

#include <windows.h>
#include "imguifilesystem.h"

namespace Editor
{
  enum class FileOpenerType
  {
    enm_file,
    enm_folder,
    enm_INVALID
  };

  class FileOpener
  {
  public:
    FileOpener();

    FileOpener(const std::filesystem::path & p_path, FileOpenerType p_type);

    void setVisible(bool p_show);

    bool getVisible() const { return m_show; }

    void setPath(const std::filesystem::path & p_path);

    std::filesystem::path getPath() const;

    std::string getPathAsString() const;

    bool render();

    bool getClosed();

    void setType(FileOpenerType p_type) { m_type = p_type; }

  private:
    ImGuiFs::Dialog m_dlg;
    std::string m_dlg_path;
    std::filesystem::path m_path;
    bool m_show;
    bool m_closed;
    FileOpenerType m_type;

  };

}
