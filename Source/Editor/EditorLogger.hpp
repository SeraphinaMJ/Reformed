/*!***************************************************************************************
\file       EditorLogger.hpp
\author     Ryan Booth
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once

#include "imgui.h"

#include <string>

namespace Editor
{
  struct EditorLogger
  {
    // Usage:
    //  static EditorLogger my_log;
    //  my_log.AddLog("Hello %d world\n", 123);
    //  my_log.Draw("title");

    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear() { Buf.clear(); LineOffsets.clear(); }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
      int old_size = Buf.size();
      va_list args;
      va_start(args, fmt);
      Buf.appendfv(fmt, args);
      va_end(args);
      for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
          LineOffsets.push_back(old_size);
      ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
      ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
      if (!ImGui::Begin(title, p_open))
      {
        ImGui::End();
        return;
      }
      if (ImGui::Button("Clear")) Clear();
      ImGui::SameLine();
      bool copy = ImGui::Button("Copy");
      ImGui::SameLine();
      Filter.Draw("Filter", -100.0f);
      ImGui::Separator();
      ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
      if (copy) ImGui::LogToClipboard();

      if (Filter.IsActive())
      {
        const char* buf_begin = Buf.begin();
        const char* line = buf_begin;
        for (int line_no = 0; line != NULL; line_no++)
        {
          const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
          if (Filter.PassFilter(line, line_end))
          {
            //ImGui::TextUnformatted(line, line_end);
            PrintText(line, line_end);
          }

          line = line_end && line_end[1] ? line_end + 1 : NULL;
        }
      }
      else
      {
        //ImGui::TextUnformatted(Buf.begin());
        PrintText(Buf.begin());
      }

      if (ScrollToBottom)
        ImGui::SetScrollHereY(1.0f);
      ScrollToBottom = false;
      ImGui::EndChild();
      ImGui::End();
    }

  private:
    void PrintText(const char* text, const char* text_end = nullptr)
    {
      //char buffer[4096];
      //size_t l_size;

      //if(text_end != nullptr)
      //{
      //  l_size = text_end - text;
      //}
      //else
      //{
      //  l_size = strlen(text);
      //}

      //strncpy_s(buffer, 4096, text, l_size);

      std::string l_string;

      if(text_end != nullptr)
      {
        size_t l_length = text_end - text;
        l_string.insert(0, text, l_length);
      }
      else
      {
        l_string = text;
      }

      size_t l_position = 0;

      for(int i = 0; i < LineOffsets.Size; ++i)
      {
        std::string l_line;

        if(text_end == nullptr)
        {
          if (i < LineOffsets.Size - 1)
          {
            int l_offset = LineOffsets[i];
            l_line = l_string.substr(l_position, l_offset - l_position);
            l_position = LineOffsets[i] + 1;
          }
          //Last one
          else
          {
            l_line = l_string.substr(l_position);
          }
        }
        else
        {
          l_line = l_string;
        }

        if (l_line.find("[ERROR]") != std::string::npos)
        {
          //if (text_end != nullptr)
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 1.0f), text, text_end);
          //}
          //else
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 1.0f), l_line.c_str());
          //}

          ImGui::TextColored(ImVec4(255.0f, 0.0f, 0.0f, 1.0f), "%s", l_line.c_str());
        }
        else if (l_line.find("[WARNING]") != std::string::npos)
        {
          //if (text_end != nullptr)
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 165.0f, 0.0f, 1.0f), text, text_end);
          //}
          //else
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 165.0f, 0.0f, 1.0f), text);
          //}

          ImGui::TextColored(ImVec4(255.0f, 165.0f, 0.0f, 1.0f), "%s", l_line.c_str());
        }
        else if (l_line.find("[EDITOR]") != std::string::npos)
        {
          //if (text_end != nullptr)
          //{
          //  ImGui::TextColored(ImVec4(0.0f, 255.0f, 0.0f, 1.0f), text, text_end);
          //}
          //else
          //{
          //  ImGui::TextColored(ImVec4(0.0f, 255.0f, 0.0f, 1.0f), text);
          //}

          ImGui::TextColored(ImVec4(0.0f, 255.0f, 0.0f, 1.0f), "%s", l_line.c_str());
        }
        else if (l_line.find("[ENGINE]") != std::string::npos)
        {
          //if (text_end != nullptr)
          //{
          //  ImGui::TextColored(ImVec4(0.0f, 0.0f, 255.0f, 1.0f), text, text_end);
          //}
          //else
          //{
          //  ImGui::TextColored(ImVec4(0.0f, 0.0f, 255.0f, 1.0f), text);
          //}

          ImGui::TextColored(ImVec4(0.0f, 0.0f, 255.0f, 1.0f), "%s", l_line.c_str());
        }
        else if (l_line.find("[NOTICE]") != std::string::npos)
        {
          //if (text_end != nullptr)
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 223.0f, 0.0f, 1.0f), text, text_end);
          //}
          //else
          //{
          //  ImGui::TextColored(ImVec4(255.0f, 223.0f, 0.0f, 1.0f), text);
          //}

          ImGui::TextColored(ImVec4(255.0f, 223.0f, 0.0f, 1.0f), "%s", l_line.c_str());
        }
        else
        {
          ImGui::TextUnformatted(l_line.c_str());
        }


        if(text_end != nullptr)
        {
          break;
        }
      }
    }
  };
}


