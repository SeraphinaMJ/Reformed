/*!***************************************************************************************
\file       FileOpener.cpp
\author     Ryan Booth
\date       11/14/2019
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/

#include "FileOpener.hpp"

Editor::FileOpener::FileOpener() : m_path("./"), m_show(false), m_closed(false), m_type(FileOpenerType::enm_INVALID)
{
  m_dlg_path = m_path.string();
}

Editor::FileOpener::FileOpener(const std::filesystem::path & p_path, FileOpenerType p_type) : m_path(p_path), m_show(false), m_closed(false), m_type(p_type)
{
  m_dlg_path = m_path.string();
}

void Editor::FileOpener::setVisible(bool p_show)
{
  m_show = p_show;
}

void Editor::FileOpener::setPath(const std::filesystem::path & p_path)
{
  m_path = p_path;
  m_dlg_path = m_path.string();
}

std::filesystem::path Editor::FileOpener::getPath() const
{
  return m_path;
}

std::string Editor::FileOpener::getPathAsString() const
{
  return m_path.string();
}

bool Editor::FileOpener::render()
{
  if(m_show)
  {
    if (m_type == FileOpenerType::enm_INVALID)
    {
      //Logger message
      return false;
    }

    std::string l_new_path;

    if(m_type == FileOpenerType::enm_file)
    {
      l_new_path = m_dlg.chooseFileDialog(m_show, m_dlg_path.c_str());
    }
    else if (m_type == FileOpenerType::enm_folder)
    {
      l_new_path = m_dlg.chooseFolderDialog(m_show, m_dlg_path.c_str());
    }

    l_new_path = m_dlg.getLastDirectory();

    if (l_new_path != m_dlg_path)
    {
      m_dlg_path = l_new_path;
    }

    if (strlen(m_dlg.getChosenPath()) > 0)
    {
      m_path = m_dlg.getChosenPath();

      return true;
    }

    if (m_dlg.hasUserJustCancelledDialog())
    {
      m_show = false;

      m_closed = true;
    }
  }

  return false;
}

bool Editor::FileOpener::getClosed()
{
  if(m_closed)
  {
    m_closed = false;
    return true;
  }

  return m_closed;
}
