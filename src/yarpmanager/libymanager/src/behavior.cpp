/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 *
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#include "behavior.h"
#include <stdio.h>
#include <string>
using namespace std;


/*
 * Behavior
 */

Behavior::Behavior(void) : Node(BEHAVIOR)
{
    clear();
}


Behavior::Behavior(const char* szName) : Node(BEHAVIOR)
{
    clear();
    if(szName) strName = szName;
}

Behavior::Behavior(const Behavior &beh) : Node(beh)
{
    Behavior::swap(beh);
}

Behavior::~Behavior() { }

Node* Behavior::clone(void) 
{
    Behavior* beh = new Behavior(*this);
    return beh;
}

Behavior& Behavior::operator=(const Behavior& rhs)
{
    Node::operator=(rhs);
    Behavior::swap(rhs);
    return *this;
}

void Behavior::swap(const Behavior &beh)
{
    clear();
    strName = beh.strName;
    behOwner = beh.behOwner;
    configurations = beh.configurations;
    inhibitions = beh.inhibitions;
    conditions = beh.conditions;
}


/*
 * BehaviorGroup
 */

BehaviorGroup::BehaviorGroup(void) : Node(GROUP)
{
    clear();
}


BehaviorGroup::BehaviorGroup(const char* szName) : Node(GROUP)
{
    clear();
    if(szName) strName = szName;
}

BehaviorGroup::BehaviorGroup(const BehaviorGroup &group) : Node(group)
{
    BehaviorGroup::swap(group);
}

BehaviorGroup::~BehaviorGroup() { }

Node* BehaviorGroup::clone(void) 
{
    BehaviorGroup* group = new BehaviorGroup(*this);
    return group;
}

BehaviorGroup& BehaviorGroup::operator=(const BehaviorGroup& rhs)
{
    Node::operator=(rhs);
    BehaviorGroup::swap(rhs);
    return *this;
}

void BehaviorGroup::swap(const BehaviorGroup &group)
{
    clear();
    strName = group.strName;
    groupOwner = group.groupOwner;
    behaviors = group.behaviors;
    groups = group.groups;
    conditions = group.conditions;
}

