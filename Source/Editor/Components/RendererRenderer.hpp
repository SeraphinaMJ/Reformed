/*!***************************************************************************************
\file       RendererRenderer.hpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Renders the RendererRenderer :P
*****************************************************************************************/
#pragma once
#include "ComponentRenderer.hpp"
#include <vector>
#include <unordered_map>
#include <utility>

namespace Editor
{
    class rendererRenderer : public componentRenderer
    {
    public:
        rendererRenderer(editorWindow * p_parent_window);

    protected:
        void onRender(typeRT & p_type_data, objID p_editor_object_id) override;

    private:


        //std::map<const char *, std::vector<const char * > > getAssetMap(const std::vector<std::wstring> p_assetlist);




        void intiAssetMap(const std::vector<std::wstring> p_assetlist);

        std::vector<std::vector<char*>>  m_shaderfiles;


        std::string item_current_model;

        std::unordered_map<std::string, std::vector<const char*> >  l_fileList;

        bool assetListChanged = true;

        std::string m_item_current;
        std::string m_selectedItem;


    };
}
