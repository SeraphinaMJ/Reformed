/*!***************************************************************************************
\file       BehaviorTreeBuilder.cpp
\author     Charles Boudousquie
\date       9/24/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: TBD
\brief      Delegate (aka helper) object that constructs trees for the BehaviorTree Manager.
*****************************************************************************************/
#include "BehaviorTreeBuilder.hpp"
#include "BehaviorTree.hpp"
#include <stack>
#include <fstream>
#include <iomanip>

#include <rapidjson.h>
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "istreamwrapper.h"
#include "filereadstream.h"

#include "../Nodes/Behaviors/AllBehaviorsInclude.hpp"

#include "AssetManager.hpp"
#include "GeneralizedFileLoader.hpp"

void BehaviorTreeBuilder::PrintTree(BehaviorPtr beh, int depth, logger::loggerProxy & log)
{
    log << std::setw(depth * 4);
    log << "BehaviorName: " << beh->getName() << " id: " << beh->getId() << std::endl;

    auto children = beh->GetChildren();

    for (auto child : children)
    {
        PrintTree(child, depth + 1, log);
    }
}

static std::unordered_map<std::string, std::function<BehaviorPtr()>> nodeMap;

void CreateNodes(const rapidjson::Value & l_graph, /*std::map<int, std::vector<int>>& inputLinks,*/
    std::map<int, std::vector<int>>& outputLinks, std::map<int, BehaviorPtr>& behaviors);

typedef int NodeID;

BehaviorPtr GetRootFromTable(const std::map<NodeID, BehaviorPtr>& table)
{
    int counter = 0;
    BehaviorPtr root = nullptr;

    for (auto entry : table)
    {
        if (entry.second->getParent() == nullptr)
        {
            root = entry.second;
            counter++;
            // It is impossible to have more than 1 root
            assert(counter == 1);
        }
    }

    return root;
}

BehaviorTreeBuilder::BehaviorTreeBuilder()
{

#define ADD_TO_MAP(x) nodeMap[#x] = std::make_shared<x>
    // leaves
    ADD_TO_MAP(AttackClosestEnemy);
    ADD_TO_MAP(DefaultLeaf);
    ADD_TO_MAP(GoToEnemy);
    ADD_TO_MAP(JumpUp);
    //ADD_TO_MAP(JoinSquad);
    //ADD_TO_MAP(ReturnToLeader);

    // decorators
    ADD_TO_MAP(EnemyWithinRange);
    //ADD_TO_MAP(EnemySpotted);
    ADD_TO_MAP(HasSquad);
    ADD_TO_MAP(Inverter);
    ADD_TO_MAP(IsDead);
    ADD_TO_MAP(IsLowHealth);
    ADD_TO_MAP(IsSquadEngaged);
    ADD_TO_MAP(Timer);

    ADD_TO_MAP(Repeater);
    ADD_TO_MAP(RepeatUntilN);
    ADD_TO_MAP(RepeatUntilFailure);
    ADD_TO_MAP(RepeatUntilSuccess);
    ADD_TO_MAP(ReturnTrue);
    ADD_TO_MAP(PickEnemyToFight);


    // composites
    ADD_TO_MAP(Selector);
    ADD_TO_MAP(Sequencer);

}

std::shared_ptr<BehaviorTree> BehaviorTreeBuilder::CreateTree(const std::string & fileName)
{
    auto assetList = assetManager::assetList(".bht");
    std::string filePath = "../../Samples/Assets/";
    // find specific item in assetlist with fileName
    for (auto asset : assetList)
    {
        if (asset.find(fileName) != std::string::npos)
        {
            filePath += asset;
            break;
        }
    }

    std::ifstream l_if_stream(filePath);

    std::map<NodeID, std::vector<NodeID>> nodeLinks;
    std::map<NodeID, BehaviorPtr> behaviors;
    std::shared_ptr<BehaviorTree> tree = std::make_shared<BehaviorTree>();

    logger log("Behavior Tree");

    // if file is open
    if (l_if_stream.is_open())
    {
        // turn file into string for easier reading
        std::string file_as_string((std::istreambuf_iterator<char>(l_if_stream)), std::istreambuf_iterator<char>());

        // create json document from data
        rapidjson::Document l_document;
        l_document.Parse(file_as_string.c_str());

        // if this is a graph file
        if (l_document.HasMember("graph"))
        {

            // get graph object
            const rapidjson::Value & l_graph = l_document["graph"];

            CreateNodes(l_graph, nodeLinks, behaviors);
            LinkNodes(nodeLinks, behaviors);
        }

        tree->SetName(fileName);

        auto root = GetRootFromTable(behaviors);

        auto log_info = log.info();
        PrintTree(root, 0, log_info);

        tree->SetUpTree(root);
        tree->SetManager(manager);

    }
    else
    {
        log.error() << "tree" << fileName << "Cant be loaded";
        return nullptr;
    }
    l_if_stream.close();

    return tree;
}

// create node list from json object
void CreateNodes(const rapidjson::Value & l_graph, /*std::vector<int> &inputLinks,*/
    std::map<int, std::vector<int>>& outputLinks, std::map<int, BehaviorPtr>& behaviors)
{

    // for each node in graph
    for (unsigned int i = 0; i < l_graph.Size(); i++)
    {
        // get node
        auto& node = l_graph[i];

        // if it is an actual node
        if (node.IsObject())
        {
            std::string name;
            // get node name
            if (node.HasMember("name"))
            {
                name = node["name"].GetString();
            }
            else
            {
                throw std::runtime_error("Missing name for node in behavior tree!");
            }

            NodeID id;
            // get node id
            if (node.HasMember("id"))
            {
                id = node["id"].GetInt();
            }
            else
            {
                throw std::runtime_error("MISSING ID for a node in behavior tree!");
            }

            // create Behavior based on name
            BehaviorPtr behavior = nodeMap[name]();
            behavior->SetName(name);

            // set behaviors id
            behavior->setId(id);

            // store node in behavior list
            behaviors[id] = behavior;

            // if node has links
            if (node.HasMember("node output links"))
            {
                // get links
                const rapidjson::Value & l_output_Links = node["node output links"];
                //const rapidjson::Value & l_input_Links = node["node input links"];

                std::vector<NodeID> children;

                // get id of each child
                for (unsigned int j = 0; j < l_output_Links.Size(); ++j)
                {
                    ////InputIdx, InputSlot, OutputIdx, OutputSlot;

                    // child happens to be InputIDx
                    auto child = l_output_Links[j][0].GetInt();
                    children.push_back(child);
                }

                // store nodelink in nodelink list
                outputLinks[id] = children;
            }


            // set special data of node (if any)
            behavior->updateFromFile(node);
        }
    }
}

void BehaviorTreeBuilder::SetManager(BehaviorTreeManager * manager_)
{
    this->manager = manager_;
}


void BehaviorTreeBuilder::LinkNodes(std::map<int, std::vector<NodeID>>& nodeLinks, std::map<int, BehaviorPtr>& behaviors)
{
    // for every pairs in the nodelinks
    for (auto pair : nodeLinks)
    {
        //get parent node
        auto ParentID = pair.first;
        BehaviorPtr ParentNode = behaviors[ParentID];

        // get list of child ids
        auto Children = pair.second;

        // for each child, add it to node
        for (auto Child : Children)
        {
            auto ChildNode = behaviors[Child];
            ParentNode->addChild(ChildNode);
            ChildNode->setParent(ParentNode);
        }
    }
}

//BehaviorTreeBuilder::BehaviorPtr BehaviorTreeBuilder::BuildTree(const Table & table)
//{
//    std::for_each(table.begin(), table.end(),
//        [](const TableEntry& entry)
//    {
//        if (entry.behavior == nullptr)
//        {
//            throw std::exception("empty entry");
//        }});
//
//    // create tree // MAKE SURE TABLE'S POINTERS ARE CORRECT
//    BehaviorPtr tree = table.begin()->behavior;
//
//    // stack to help create tree
//    std::stack<TableEntry> stack;
//    // push on root
//    stack.push(*table.begin());
//
//    // set iterator which traverses the table,
//    // when done traversing the tree should be fully built
//    auto it = table.begin() + 1;
//
//    // iterate over each node in the list that was read from a file.
//    while (it != table.end())
//    {
//
//        auto top = stack.top();
//
//        // if lower level then it is a child
//        if (it->level > top.level)
//        {
//            // add as child to the current node
//            top.behavior->addChild(it->behavior);
//            it->behavior->setParent(top.behavior);
//            stack.push(*it);
//            it++;
//            continue;
//        }
//        // if higher level then go back up tree
//        else if (it->level <= top.level)
//        {
//            stack.pop();
//        }
//    }
//
//    return tree;
//}
//
//BehaviorTreeBuilder::Table BehaviorTreeBuilder::BuildTable(std::string fileName)
//{
//
//    std::wstring filePath(L"behavior_trees\\");
//    filePath.append(fileName.begin(), fileName.end());
//    filePath.append(L".bht");
//
//    auto handle = assetManager::getAsset(filePath);
//
//
//    auto data = handle->getData< genericAssetData>().getRaw(); // ??????????????
//
//    if (data.empty())
//    {
//        throw std::exception(("Failed to Load data file: " + fileName).c_str());
//    }
//
//    std::stringstream file(std::string(data.begin(), data.end()));
//
//    Table table;
//
//    bool firstLine = true;
//
//    // get line from file
//    std::string line;
//    while (std::getline(file, line))
//    {
//        if (firstLine == true)
//        {
//            firstLine = false;
//            continue;
//        }
//
//        // find first parenthesis
//        size_t leftParenthesisPos = line.find_first_of("(");
//        size_t rightParenthesisPos = line.find_last_of(")");
//        size_t commaPos = line.find(",");
//
//        // if file is not corrupted
//        if (leftParenthesisPos != std::string::npos && rightParenthesisPos != std::string::npos &&
//            commaPos != std::string::npos)
//        {
//            std::string className = line.substr(leftParenthesisPos + 1, commaPos - leftParenthesisPos - 1);
//            std::string depthString = line.substr(commaPos + 1, rightParenthesisPos - commaPos - 1);
//
//            int depthLevel = std::stoi(depthString);
//            if (nodeMap.find(className) == nodeMap.end())
//            {
//                std::string error("Could not find node of type: ");
//                error += className;
//                throw std::exception(error.c_str());
//            }
//            else
//            {
//                auto node = nodeMap[className]();
//                // add item to table
//                TableEntry newItem;
//                newItem.behavior = node;
//                newItem.level = (unsigned)depthLevel;
//                table.push_back(newItem);
//            }
//
//        }
//        else
//        {
//            std::cout << "File " << fileName << " corrupted!" << std::endl;
//        }
//    }
//
//    return table;
//}
//



