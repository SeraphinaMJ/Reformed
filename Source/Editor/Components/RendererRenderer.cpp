/*!***************************************************************************************
\file       RendererRenderer.cpp
\author     cody cannell
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			Renders the RendererRenderer :P
*****************************************************************************************/
#include "RendererRenderer.hpp"
#include "Reflection.hpp"
#include "../Renderers/ToggleButton.hpp"

#include "../EngineController.hpp"

#include <string>
#include <iostream>
#include <filesystem>
#include <regex>
#include <EngineRunner.hpp>
#include <string>
#include <Engine.hpp>
#include "../EditorObjectManager.hpp"
#include "../ObjectCommand.hpp"
#include "../EditorWindow.hpp"
#include "Logger.hpp"

namespace fs = std::filesystem;

char *convert(const std::string & s)
{
    char *pc = new char[s.size() + 1];
    std::strcpy(pc, s.c_str());
    return pc;
}


static 	std::vector<std::pair<std::string, std::string>> l_listOrder = {
                                                                        {"fonts", "fontName"},
                                                                        {"materials","materialName"},
                                                                        {"models", "modelName"},
                                                                        /*{"textures", "textureName"},*/
                                                                        {"vertex", "vertexShaderName"},
                                                                        {"fragment", "fragmentShaderName"},
                                                                        {"geometry", "geometryShaderName"},
                                                                        {"tessellation_control", "tessellationControlShaderName"},
                                                                        {"tessellation_evaluation", "tessellationEvaluationShaderName"},
                                                                        {"compute", "computeShaderName"}
};

void Editor::rendererRenderer::intiAssetMap(const std::vector<std::wstring> p_assetlist)
{


    for (auto l_path : p_assetlist)
    {
        logger("rendererRenderer").info() << l_path.c_str() << std::endl;

        std::regex l_r("(.*/)?([A-z]*)/([A-z]*.[A-z]*)");

        std::smatch l_match;

        auto string = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->stringFromWide(l_path);

        std::regex_search(string, l_match, l_r);

        std::string key = l_match[2];
        std::string type = l_match[3];

        auto checkIfExsists = l_fileList.emplace(std::make_pair<std::string, std::vector<const char*>>(key.c_str(), { convert(type) }));

        if (!checkIfExsists.second)
            l_fileList[key.c_str()].push_back(convert(type));
    }


}

Editor::rendererRenderer::rendererRenderer(editorWindow* p_parent_window)
    : componentRenderer(p_parent_window)
{
    for (auto typrRTName : l_listOrder)
        m_override_members.insert(typrRTName.second);

}

//If you don't use type data then leave this commented out
void Editor::rendererRenderer::onRender(typeRT&  p_type_data, objID p_editor_object_id)
{
  bool l_modified = false;
  auto l_engine_controller = &getEngineController();
  EditorObjectManager & l_editor_object_manager = getTopWindow()->getSceneWindow().getEditorObjectManager();
    // =============================== //
    //   show List of all Model Files  //
    // =============================== //

    auto assetlist = getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->assetList();

    if (assetListChanged)
    {
        intiAssetMap(assetlist);
        assetListChanged = false;
    }

    // =============================== //
    //   show all asset file lists     //
    // =============================== //
    for (size_t i = 0; i < l_listOrder.size(); ++i)
    {
        if (!p_type_data.member(l_listOrder[i].second).getString().empty())
            m_item_current = p_type_data.member(l_listOrder[i].second).getString();

        std::string l_listName = l_listOrder[i].first;

        if (ImGui::BeginCombo(l_listName.c_str(), m_item_current.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
        {
            for (size_t n = 0; n < l_fileList[l_listName].size(); n++)
            {
                bool is_selected = (l_fileList[l_listName][n] == m_selectedItem);
                if (ImGui::Selectable(l_fileList[l_listName][n], is_selected))
                {
                  m_item_current = l_fileList[l_listName][n];
                  l_modified = true;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }

      if(l_modified)
      {
        l_modified = false;
        EditorObject * l_editor_object = l_editor_object_manager.getEditorObject(p_editor_object_id);

        std::string l_value = m_item_current;

        typeRT & p_data = p_type_data.member(l_listOrder[i].second);

        func_t<std::string> l_function_pointer = Command::setString;

        CommandPtr l_command = ObjectDataChangeCommand<std::string>::createDataChangeCommand(l_engine_controller, l_editor_object, l_function_pointer, p_data, l_value);

        l_editor_object_manager.addCommandToQueue(l_command);

        //p_type_data.member(l_listOrder[i].second).setString(m_item_current);
      }
    }

}

