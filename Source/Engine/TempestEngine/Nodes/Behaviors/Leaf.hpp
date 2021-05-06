/*!***************************************************************************************
\file       Leaf.hpp
\author     Charles Boudousquie
\date       9/20/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par
\brief      This class represents a node with no children. It is also known as an action 
            node.
*****************************************************************************************/
#pragma once
#include "Behavior.hpp"


class Leaf : public Behavior
{
public:

    BehaviorPtr getCurrentChild()override;

    BehaviorType GetType() override;

    virtual void Init() override;

    static typeRT leafOnRender();

    // returns empty vector since leaves have no child nodes
    std::vector<BehaviorPtr> GetChildren() override;

    // no children should ever be added to leaf node
    void addChild(BehaviorPtr)override;

    //virtual void Exit() override;
};

