/*!***************************************************************************************
\file       NodeGraphWindow.cpp
\author     Ryan Booth
\date       9/10/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
#include "NodeGraphWindow.hpp"
#include "NodeManager.hpp"
#include "EditorNode.hpp"
#include "../EditorWindow.hpp"
#include "BehaviorTreeManipulatorInterface.hpp"
#include "GraphTypeSelector.hpp"
#include "AgentMenuSelection.hpp"
#include "GameObject.hpp"
#include "NGE_Watchdog.hpp"
#include "Agent.hpp"

#include "../EngineController.hpp"
#include <EngineRunner.hpp>
#include <Engine.hpp>

#include <iostream>

//======== 3rd Party Includes ==========================================================//
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include "../imguifilesystem.h"
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

static const float NODE_SLOT_RADIUS = 4.0f;
static const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

std::string getFileName(const std::string& name)
{
    // retrieve ONLY the name of the file, no extensions or any part of the path
    // will be returned except the actual file name
    // EX: if name = "assets/graphs/tree.bht"
    // function will then return "tree"
    return std::filesystem::path(name).stem().string();
}

Editor::nodeGraphWindow::nodeGraphWindow(editorWindow * p_parent_editor, engineController * p_engine_controller)
    : windowBase(p_parent_editor),
    m_draw(true),
    m_show_grid(true),
    m_open_context_menu(false),
    m_first_run(false),
    m_node_manager(new NodeManager(p_engine_controller, getLogger())),
    m_node_hovered_in_list(-1),
    m_node_hovered_in_scene(-1),
    m_node_slot_radius(4.0f),
    m_node_window_padding(8.0f, 8.0f),
    m_zoom_scalar(1.0f),
    m_graph_name("Unnamed_Graph"), 
    
    m_open_graph(false),
    m_dirty_graph(false),

    m_connect_node_line_start(0.0f, 0.0f),
    m_output_link_node(std::pair<int, int>(-1, -1)),
    m_connect_node_line_end(0.0f, 0.0f),
    m_drawing_connect_line(false),

    m_editor_window(p_parent_editor),
    g_type_selector(new GraphTypeSelector()),
    agentMenuSelector(new AgentMenuSelection(p_engine_controller))
{
}

Editor::nodeGraphWindow::~nodeGraphWindow()
{

    delete g_type_selector;
}

void Editor::nodeGraphWindow::reset()
{
    m_scrolling = ImVec2(0.0f, 0.0f);
    m_dirty_graph = false;
}

void Editor::nodeGraphWindow::saveGraph()
{
    if (m_node_manager->validateGraph())
    {
        std::string p_path = m_editor_window->getProjectFilePath();

        std::string p_file_name = getFileName(m_graph_name);

        if (g_type_selector->GetSelectedType() == GraphType::BEHAVIORTREE)
        {
            p_file_name += ".bht";
        }
        else if (g_type_selector->GetSelectedType() == GraphType::SCRIPT)
        {
            p_file_name += ".script";
        }

        if (m_node_manager->serializeNodes(p_path, p_file_name))
        {
            m_dirty_graph = false;
        }

        // update asset manager
        getEngineController().getEngineRunner()->getEngine()->getAssetManipulator().lock()->reloadAssets();

        // notify BTM that there was a change
        getEngineController().getEngineRunner()->getEngine()->getBehaviorManipulator().lock()->markTreeChanged(m_graph_name);
    }
    
}

void Editor::nodeGraphWindow::onRender()
{
    if (!ImGui::Begin("Node Graph", &m_draw))
    {
        ImGui::End();
        return;
    }

    if (!m_first_run)
    {
        m_node_manager->retrieveNodeFactoryList();
        m_first_run = true;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);

    // Draw a list of nodes on the left side
    m_open_context_menu = false;
    m_node_hovered_in_list = -1;
    m_node_hovered_in_scene = -1;

    ImGui::BeginChild("node_list", ImVec2(200, 0));
    ImGui::Text("Nodes");
    ImGui::Separator();

    auto l_node_list = m_node_manager->getNodeList();

    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        const EditorNode * l_node = i_node->second;
        int l_node_id = l_node->getId();

        ImGui::PushID(l_node_id);

        std::string l_display_name = l_node->getName() + ": " + std::to_string(l_node_id);

        if (ImGui::Selectable(l_display_name.c_str(), l_node_id == m_node_manager->getSelectedNodeId()))
        {
            m_node_manager->setSelectedNodeId(l_node_id);

            centerGraphOnNode(l_node);
        }

        if (ImGui::IsItemHovered())
        {
            m_node_hovered_in_list = l_node_id;
            m_open_context_menu |= ImGui::IsMouseClicked(1);
        }

        ImGui::PopID();
    }

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();

    // Create our child canvas
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", m_scrolling.x, m_scrolling.y);
    ImGui::SameLine();
    ImGui::Checkbox("Show grid", &m_show_grid);

    ImGui::SameLine();

    //Graph name code
    graphNameRender();

    //Open Graph Button code
    openGraphButton();

    //Save Graph Button code
    saveGraphButton();

    clearGraphButton();

    graphTypeMenu();
    agentMenuSelector->Display();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, IM_COL32(60, 60, 70, 200));
    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);

    ImVec2 offset = ImGui::GetCursorScreenPos() + m_scrolling;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Display grid
    if (m_show_grid)
    {
        ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
        float GRID_SZ = 64.0f * m_zoom_scalar;
        ImVec2 win_pos = ImGui::GetCursorScreenPos();
        m_canvas_size = ImGui::GetWindowSize();

        for (float x = fmodf(m_scrolling.x, GRID_SZ); x < m_canvas_size.x; x += GRID_SZ)
        {
            draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, m_canvas_size.y) + win_pos, GRID_COLOR);
        }

        for (float y = fmodf(m_scrolling.y, GRID_SZ); y < m_canvas_size.y; y += GRID_SZ)
        {
            draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(m_canvas_size.x, y) + win_pos, GRID_COLOR);
        }

    }

    //Display zoom level
    const int l_percent = static_cast<int>(m_zoom_scalar) * 100;
    ImGui::Text("Current Zoom: %i%%", l_percent);

    //Display node links
    drawNodeLinks(draw_list);

    //Display nodes
    drawNodes(draw_list);

    draw_list->ChannelsMerge();

    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
    {
        m_node_manager->setSelectedNodeId(-1);
        m_node_hovered_in_list = m_node_hovered_in_scene = -1;
        m_open_context_menu = true;
    }

    if (m_open_context_menu)
    {
        ImGui::OpenPopup("context_menu");

        if (m_node_hovered_in_list != -1)
        {
            m_node_manager->setSelectedNodeId(m_node_hovered_in_list);
        }

        if (m_node_hovered_in_scene != -1)
        {
            m_node_manager->setSelectedNodeId(m_node_hovered_in_scene);
        }
    }

    // Draw context menu (Right click)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

    m_node_definitions = m_node_manager->getNodeDefinitions();

    if (ImGui::BeginPopup("context_menu"))
    {
        auto i_node = m_node_manager->getSelectedNodeId() != -1 ? l_node_list->find(m_node_manager->getSelectedNodeId())
            : l_node_list->end();
        ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;

        //A node was right clicked
        if (i_node != l_node_list->end())
        {
            EditorNode * l_node = i_node->second;

            ImGui::Text("Node '%s'", l_node->getName().c_str());
            ImGui::Separator();

            if (ImGui::MenuItem("Delete"))
            {
                m_node_manager->removeNode(l_node->getId());

                m_dirty_graph = true;
            }

            linkNodeMenu(i_node, l_node);

            decoupleNodeMenu(i_node, l_node);
        }
        else
        {
            createNodeMenu(scene_pos);

            if (ImGui::MenuItem("Validate Graph"))
            {
                m_node_manager->validateGraph();
            }

            if (ImGui::MenuItem("Save Graph"))
            {
                saveGraph();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::PopStyleVar();

    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
    {
        m_scrolling = m_scrolling + ImGui::GetIO().MouseDelta;
    }

    //Node scaling (zooming)
    if (ImGui::IsWindowHovered())
    {
        if (ImGui::GetIO().KeyAlt)
        {
            //wheel_counter += ImGui::GetIO().MouseWheel;

            float l_scalar = ImGui::GetIO().MouseWheel;

            if (l_scalar != 0.0f)
            {
                float l_old_zoom = m_zoom_scalar;

                l_scalar = l_scalar / 5.0f;

                m_zoom_scalar += l_scalar;

                //Don't allow anything less than 20%
                if (m_zoom_scalar <= 0.2f)
                {
                    m_zoom_scalar = 0.2f;
                }

                if (m_zoom_scalar != l_old_zoom)
                {
                    //Move grid position based off of zooming
                    m_scrolling.x += m_scrolling.x * l_scalar;
                    m_scrolling.y += m_scrolling.y * l_scalar;

                    m_node_manager->setEditorNodesScale(m_zoom_scalar, l_scalar);
                }

            }

        }
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();

    ImGui::End();

    renderGraphPopup();
}

void Editor::nodeGraphWindow::createNodeMenu(ImVec2 p_scene_pos)
{
    if (ImGui::BeginMenu("Create Node"))
    {
        // for every node type
        for (auto i_node_type = m_node_definitions.begin(); i_node_type != m_node_definitions.end(); ++i_node_type)
        {
            std::string l_type_name = i_node_type->first;

            if (ImGui::BeginMenu(l_type_name.c_str()))
            {
                for (size_t i = 0; i < i_node_type->second.size(); ++i)
                {
                    std::string l_good_name = i_node_type->second[i].second;

                    if (ImGui::MenuItem(l_good_name.c_str()))
                    {
                        //Create a new node of this type (need a dedicated factory for this)
                        EditorNode * l_new_node = m_node_manager->createEditorNodeFromScratch(l_type_name, i_node_type->second[i]);

                        if (l_new_node != nullptr)
                        {
                            l_new_node->setPosition(p_scene_pos);

                            // ask node manager for unique id, then add node
                            m_node_manager->addNode(l_new_node, m_node_manager->createUniqueID());

                            m_dirty_graph = true;
                        }
                    }
                }

                ImGui::EndMenu();
            }
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::linkNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node)
{
    auto l_node_list = m_node_manager->getNodeList();

    if (ImGui::BeginMenu("Link"))
    {
        for (auto i_node_type = m_node_definitions.begin(); i_node_type != m_node_definitions.end(); ++i_node_type)
        {
            std::string l_type_name = i_node_type->first;

            //Can only link with other nodes of the same graph type
            if (p_iterator_node->second->getTypeName() == l_type_name)
            {
                if (ImGui::BeginMenu(l_type_name.c_str()))
                {
                    for (auto i_link_node = l_node_list->begin(); i_link_node != l_node_list->end(); ++i_link_node)
                    {
                        //Can't link with yourself
                        if (i_link_node->second->getId() != p_node->getId())
                        {
                            std::string l_name = i_link_node->second->getName();
                            l_name.append(": ");
                            l_name.append(std::to_string(i_link_node->second->getId()));

                            if (ImGui::MenuItem(l_name.c_str()))
                            {
                                //Link this node
                                m_node_manager->linkNodes(p_node->getId(), i_link_node->second->getId());

                                m_dirty_graph = true;
                            }
                        }
                    }

                    ImGui::EndMenu();
                }
            }
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::decoupleNodeMenu(std::unordered_map<int, EditorNode*>::iterator p_iterator_node, EditorNode* p_node)
{
    if (ImGui::BeginMenu("Decouple"))
    {
        std::vector<NodeLink> l_node_links = p_iterator_node->second->getNodeLinks();

        if (ImGui::BeginMenu("Input"))
        {
            //Loop over NodeLinks list
            for (size_t i = 0; i < l_node_links.size(); ++i)
            {
                //Only check input links
                if (l_node_links[i].InputIdx == p_iterator_node->second->getId())
                {
                    EditorNode * l_linked_node = m_node_manager->getNode(l_node_links[i].OutputIdx);

                    if (l_linked_node != nullptr)
                    {
                        std::string l_output_name = l_linked_node->getName();
                        l_output_name.append(": ");
                        l_output_name.append(std::to_string(l_node_links[i].OutputIdx));

                        if (ImGui::MenuItem(l_output_name.c_str()))
                        {
                            //Decouple this node
                            m_node_manager->removeLink(l_linked_node->getId(), p_node->getId());

                            m_dirty_graph = true;
                        }
                    }

                }

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Output"))
        {
            //Loop over NodeLinks list
            for (size_t i = 0; i < l_node_links.size(); ++i)
            {
                //Only check output links
                if (l_node_links[i].OutputIdx == p_iterator_node->second->getId())
                {
                    EditorNode * l_linked_node = m_node_manager->getNode(l_node_links[i].InputIdx);

                    if (l_linked_node != nullptr)
                    {
                        std::string l_output_name = l_linked_node->getName();
                        l_output_name.append(": ");
                        l_output_name.append(std::to_string(l_node_links[i].InputIdx));

                        if (ImGui::MenuItem(l_output_name.c_str()))
                        {
                            //Decouple this node
                            m_node_manager->removeLink(p_node->getId(), l_linked_node->getId());

                            m_dirty_graph = true;
                        }
                    }

                }

            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void Editor::nodeGraphWindow::centerGraphOnNode(const EditorNode * p_editor_node)
{
    if (p_editor_node != nullptr)
    {
        const ImVec2 l_position = p_editor_node->getPosition();
        const ImVec2 l_scale = p_editor_node->getScaleAfterZoom();;
        const ImVec2 l_node_center(l_position.x + l_scale.x / 2.0f, l_position.y + l_scale.y / 2.0f);

        const ImVec2 l_center(-1.0f * (l_node_center.x - (m_canvas_size.x / 2.0f)), -1.0f * (l_node_center.y - (m_canvas_size.y / 2.0f)));

        m_scrolling = l_center;
    }
}

void Editor::nodeGraphWindow::drawNodeLinks(ImDrawList* p_draw_list)
{
    p_draw_list->ChannelsSplit(2);
    p_draw_list->ChannelsSetCurrent(0); // Background

    ImVec2 l_offset = ImGui::GetCursorScreenPos() + m_scrolling;

    auto l_node_list = m_node_manager->getNodeList();

    //Loop over each node
    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        std::vector<NodeLink> l_node_links = i_node->second->getNodeLinks();

        //Display each output link
        for (size_t i = 0; i < l_node_links.size(); ++i)
        {
            if (l_node_links[i].OutputIdx == i_node->second->getId())
            {
                NodeLink l_link = l_node_links[i];

                EditorNode * l_node_input = l_node_list->at(l_link.InputIdx);
                EditorNode * l_node_output = i_node->second;

                ImVec2 l_point1 = l_offset + l_node_input->GetInputSlotPos(l_link.InputSlot);

                ImVec2 l_point2 = l_offset + l_node_output->GetOutputSlotPos(l_link.OutputSlot);

                //Line color is output node color (R,G,B,A)
                ImVec4 l_color = l_node_output->getColor();
                p_draw_list->AddBezierCurve(l_point1, l_point1 + ImVec2(-50, 0), l_point2 + ImVec2(+50, 0), l_point2, IM_COL32(l_color.x, l_color.y, l_color.z, 255), 3.0f);
            }
        }
    }

}

void Editor::nodeGraphWindow::drawNodes(ImDrawList * p_draw_list)
{
    // get mouse offset
    ImVec2 l_offset = ImGui::GetCursorScreenPos() + m_scrolling;

    //  get list of nodes
    auto l_node_list = m_node_manager->getNodeList();

    // check if user is in play mode
    bool l_playing = getEngineController().isPlaying();

    if (l_playing)
    {
        //TODO: Implement this function
        m_node_manager->setActiveNodeData();
    }

    // Check watchdog for changes in inspector
    // Specific change here would be that user changed which tree an agent will be using
    // in 
    if (NGE_Watchdog::GetInstance().inspectorChangedTree())
    {
        auto object = NGE_Watchdog::GetInstance().getChangedAgent();
        auto actor = getEngineController().getEngineRunner()->getEngine()->getCurrentScene().lock()->findGameObjectByID(object);

        // check if our selected agent is the same as the agent being modified
        // in the inspector window
        if(!agentMenuSelector->getSelectedObject().expired())
        {
            if (actor == agentMenuSelector->getSelectedObject().lock())
            {
                auto treeName = actor->getComponent<Agent>()->getTreeName();
                // clear entire graph
                m_node_manager->reset();
                // get name of tree that selected Actor is using
                auto path = this->m_editor_window->getProjectFilePath() + "/Assets/graphs/";

                //C:/Users/boudo/Desktop/senior_game/tempestengine/Samples/ + /Assets/graphs/ + graph_name
                m_graph_name = getFileName(treeName);
                m_node_manager->readFromFile(path + m_graph_name + ".bht");
            }

            NGE_Watchdog::GetInstance().Reset();
        }
        
    }

    // if user just chose a new agent's graph to display then load it in
    if (agentMenuSelector->userChangedSelection())
    {

        // clear entire graph
        m_node_manager->reset();
        // get name of tree that selected Actor is using
        auto path = this->m_editor_window->getProjectFilePath() + "/Assets/graphs/";

        //C:/Users/boudo/Desktop/senior_game/tempestengine/Samples/ + /Assets/graphs/ + graph_name
        m_graph_name = getFileName(agentMenuSelector->getSelectionTreeName());
        m_node_manager->readFromFile(path + m_graph_name + ".bht");

        // reset menu selector to watch out for new selection
        agentMenuSelector->rewatch();
    }

    // get node that is currently being processed by the BehaviorTreeManager
    auto behManipulator = getEngineController().getEngineRunner()->getEngine()->getBehaviorManipulator().lock();
    auto idOfActiveNode = behManipulator->getActiveNodeID(agentMenuSelector->getSelectedObject());


    // for all nodes
    for (auto i_node = l_node_list->begin(); i_node != l_node_list->end(); ++i_node)
    {
        // get node and id
        EditorNode * l_node = i_node->second;
        int l_node_id = l_node->getId();

        // check for active status, meaning it is the current node that the Behavior Tree is processing
        bool nodeIsActive = idOfActiveNode == i_node->first;
        // end of checking active status
        ImGui::PushID(l_node_id);

        ImVec2 l_node_rect_min = l_offset + l_node->getPosition();

        // Display node contents first
        p_draw_list->ChannelsSetCurrent(1); // Foreground

        bool l_old_any_active = ImGui::IsAnyItemActive();

        ImVec2 l_render_position = l_node_rect_min + NODE_WINDOW_PADDING;

        ImGui::SetCursorScreenPos(l_render_position);

        //EditorNode rendering
        //If Engine is not running, render the data that is saved inside object
        //If Engine is running, render the data that is currently active in engine
        l_node->render(l_playing, l_render_position);

        // Save the size of what we have emitted and whether any of the widgets are being used
        bool node_widgets_active = (!l_old_any_active && ImGui::IsAnyItemActive());
        //TODO: What was this line originally trying to do?
        //l_node->getScale() = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
        ImVec2 l_node_scale_after_zoom = l_node->getScaleAfterZoom();
        ImVec2 node_rect_max = l_node_rect_min + l_node_scale_after_zoom;

        // Display node box
        p_draw_list->ChannelsSetCurrent(0); // Background

        ImGui::SetCursorScreenPos(l_node_rect_min);
        ImGui::InvisibleButton("node", l_node_scale_after_zoom);

        if (ImGui::IsItemHovered())
        {
            m_node_hovered_in_scene = l_node_id;
            m_open_context_menu |= ImGui::IsMouseClicked(1);
        }

        bool node_moving_active = ImGui::IsItemActive();

        if (node_widgets_active || node_moving_active)
        {
            m_node_manager->setSelectedNodeId(l_node_id);
        }

        //Don't allow nodes to move while drawing connection line
        if (node_moving_active && ImGui::IsMouseDragging(0) && !m_drawing_connect_line)
        {
            l_node->setPosition(l_node->getPosition() + ImGui::GetIO().MouseDelta);
        }

        //Background rendering
        ImVec4 l_colorVec = l_node->getColor();
        ImU32 l_color = IM_COL32(l_colorVec.x, l_colorVec.y, l_colorVec.z, l_colorVec.w);

        float l_highlighted = 0.33f;
        float l_notHighligted = 0.23f;

        //Example for different colors
        //if (l_node->getType() == NodeType::Control_Flow)
        //{
        //	l_highlighted = 0.42f;
        //}

        ImU32 node_bg_color =
            (m_node_hovered_in_list == l_node_id || m_node_hovered_in_scene == l_node_id || (m_node_hovered_in_list == -1
                && m_node_manager->getSelectedNodeId() == l_node_id))
            ? IM_COL32(l_highlighted * l_colorVec.x, l_highlighted * l_colorVec.y, l_highlighted * l_colorVec.z, 200)
            : IM_COL32(l_notHighligted * l_colorVec.x, l_notHighligted * l_colorVec.y, l_notHighligted * l_colorVec.z, 200);

        //For while the game is running
        if (/*l_node->getActive()*/nodeIsActive)
        {
            //node_bg_color = IM_COL32(l_highlighted * l_colorVec.x, l_highlighted * l_colorVec.y, l_highlighted * l_colorVec.z, 200);
            node_bg_color = IM_COL32(0, 255, 0, 200);
        }

        //Outline rendering
        p_draw_list->AddRectFilled(l_node_rect_min, node_rect_max, node_bg_color, 4.0f);
        p_draw_list->AddRect(l_node_rect_min, node_rect_max, l_color, 4.0f);

        //Input circle rendering
        for (int slot_idx = 0; slot_idx < l_node->getInputs(); slot_idx++)
        {
            const ImVec2 l_input_slot_render_position = l_offset + l_node->GetInputSlotPos(slot_idx);
            ImVec2 l_input_slot_scale(NODE_SLOT_RADIUS + 4.5f, NODE_SLOT_RADIUS + 4.5f);
            const ImVec2 l_input_button_position(l_input_slot_render_position.x - NODE_SLOT_RADIUS, l_input_slot_render_position.y - NODE_SLOT_RADIUS);

            ImGui::SetCursorScreenPos(l_input_button_position);
            ImGui::InvisibleButton("input slot", l_input_slot_scale);

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
            {
                //Highlighted
                p_draw_list->AddCircleFilled(l_input_slot_render_position, NODE_SLOT_RADIUS, IM_COL32(255, 255, 0, 150));

                if (m_drawing_connect_line)
                {
                    if (ImGui::IsMouseReleased(0))
                    {
                        //Connect the output node to this node
                        m_node_manager->linkNodes(m_output_link_node.first, l_node_id, m_output_link_node.second, slot_idx);

                        m_dirty_graph = true;

                        //Reset variables
                        m_drawing_connect_line = false;

                        m_output_link_node.first = -1;
                        m_output_link_node.second = -1;

                        m_connect_node_line_start = ImVec2(0.0f, 0.0f);
                        m_connect_node_line_end = ImVec2(0.0f, 0.0f);
                    }
                }
                //Left Alt + Left click
                else if (ImGui::GetIO().KeyAlt && ImGui::IsMouseClicked(0))
                {
                    //Decouple this node
                    if (m_node_manager->removeLinkFromInputSlot(l_node_id, slot_idx))
                    {
                        m_dirty_graph = true;
                    }
                }

            }
            else
            {
                //Not highlighted
                p_draw_list->AddCircleFilled(l_input_slot_render_position, NODE_SLOT_RADIUS, IM_COL32(205, 205, 0, 100));
            }
        }

        //Output circle rendering
        for (int slot_idx = 0; slot_idx < l_node->getOutputs(); slot_idx++)
        {
            const ImVec2 l_output_slot_render_position = l_offset + l_node->GetOutputSlotPos(slot_idx);
            ImVec2 l_output_slot_scale(NODE_SLOT_RADIUS + 4.5f, NODE_SLOT_RADIUS + 4.5f);
            const ImVec2 l_output_button_position(l_output_slot_render_position.x - NODE_SLOT_RADIUS, l_output_slot_render_position.y - NODE_SLOT_RADIUS);

            ImGui::SetCursorScreenPos(l_output_button_position);
            ImGui::InvisibleButton("output slot", l_output_slot_scale);

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
            {
                //Highlighted
                p_draw_list->AddCircleFilled(l_output_slot_render_position, NODE_SLOT_RADIUS, IM_COL32(255, 165, 0, 150));

                //Left click
                if (ImGui::IsMouseClicked(0) && (!ImGui::GetIO().KeyAlt))
                {
                    m_drawing_connect_line = true;

                    m_output_link_node.first = l_node_id;
                    m_output_link_node.second = slot_idx;

                    m_connect_node_line_start = l_output_slot_render_position;
                    m_connect_node_line_end = m_connect_node_line_start;
                }
                //Left Alt + Left click
                else if ((ImGui::GetIO().KeyAlt && ImGui::IsMouseClicked(0)))
                {
                    //Decouple this node
                    if (m_node_manager->removeLinkFromOutputSlot(l_node_id, slot_idx))
                    {
                        m_dirty_graph = true;
                    }
                }
            }
            else
            {
                //Not highlighted
                p_draw_list->AddCircleFilled(l_output_slot_render_position, NODE_SLOT_RADIUS, IM_COL32(205, 105, 0, 100));
            }
        }

        ImGui::PopID();
    }

    if (m_drawing_connect_line)
    {
        if (ImGui::IsMouseDragging(0, 0.0f))
        {
            m_connect_node_line_end = m_connect_node_line_end + ImGui::GetIO().MouseDelta;
        }
        //User let go, reset saved variables
        else
        {
            m_drawing_connect_line = false;

            m_output_link_node.first = -1;
            m_output_link_node.second = -1;

            m_connect_node_line_start = ImVec2(0.0f, 0.0f);
            m_connect_node_line_end = ImVec2(0.0f, 0.0f);
        }

        p_draw_list->AddLine(m_connect_node_line_start, m_connect_node_line_end, IM_COL32(200, 200, 100, 255), 3.0f);
    }

}

void Editor::nodeGraphWindow::openGraphButton()
{
    ImGui::SameLine();
    if (ImGui::Button("Open Graph"))
    {
        m_open_graph = true;
    }
}

void Editor::nodeGraphWindow::saveGraphButton()
{
    ImGui::SameLine();
    if (ImGui::Button("Save Graph"))
    {
        saveGraph();
    }
    //Just for visuals
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);

    ImGui::SameLine();
    ImGui::Checkbox("Graph Dirty", &m_dirty_graph);

    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}

void Editor::nodeGraphWindow::clearGraphButton()
{
    ImGui::SameLine();
    if (ImGui::Button("Clear Graph"))
    {
        m_node_manager->reset();
        m_graph_name.clear();
    }
}

void Editor::nodeGraphWindow::graphTypeMenu()
{
    g_type_selector->DisplayMenu();
}

void Editor::nodeGraphWindow::renderGraphPopup()
{
    if (m_open_graph)
    {
        static ImGuiFs::Dialog dlg;    // one per dialog (and must be static)
        static std::string chosenPath = m_editor_window->getProjectFilePath() + "/Assets/graphs";
        std::string l_newPath;

        l_newPath = dlg.chooseFileDialog(m_open_graph, chosenPath.c_str());

        l_newPath = dlg.getLastDirectory();

        if (l_newPath != chosenPath)
        {
            chosenPath = l_newPath;
        }

        // If you want to copy the (valid) returned path somewhere, you can use something like:
        static char myPath[4096];

        if (dlg.hasUserJustCancelledDialog())
        {
            m_open_graph = false;
        }

        if (strlen(dlg.getChosenPath()) > 0)
        {
            strcpy(myPath, dlg.getChosenPath());

            //Make sure we are only opening up graph files
            char l_ext[40];
            ImGuiFs::PathGetExtension(myPath, l_ext);

            // if it is a behavior tree or a script
            if ((strcmp(l_ext, ".bht") == 0) || (strcmp(l_ext, ".script") == 0))
            {
                char l_return[4096];
                ImGuiFs::PathGetFileNameWithoutExtension(myPath, l_return);

                //m_logger.AddLog("[EDITOR] Loading graph: %s.\n", l_return);

                m_node_manager->reset();

                m_node_manager->readFromFile(myPath);

                m_graph_name = getFileName(l_return);

                m_open_graph = false;

                m_dirty_graph = false;
            }
            else
            {
                //m_logger.AddLog("[WARNING] %s is not a valid extension.\n Please select a .cee file.\n", l_ext);
            }
        }
    }
}

void Editor::nodeGraphWindow::graphNameRender()
{
    ImGui::SameLine();
    ImGui::PushItemWidth(250.0f);

    constexpr int buff_size = 2048;
    char buffer[buff_size] = { 0 };
    m_graph_name.copy(buffer, sizeof(buffer));
    buffer[buff_size - 1] = '\0';

    if (ImGui::InputText("Graph Name", &buffer[0], buff_size, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string l_new_name(buffer);
        m_graph_name = l_new_name;
        m_dirty_graph = true;
    }
}

