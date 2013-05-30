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

Behavior::Behavior(const char* szName) : Node(BEHAVIOR)
{
    clear();
    if(szName) strName = szName;
}

Behavior::Behavior(const Behavior &beh) : Node(BEHAVIOR)
{
    Behavior::swap(beh);
}

Behavior::~Behavior() { }

Node* Behavior::clone(void) 
{
    Behavior* beh = new Behavior(*this);
    return beh;
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
 * Group
 */

Group::Group(const char* szName) : Node(GROUP)
{
    clear();
    if(szName) strName = szName;
}

Group::Group(const Group &group) : Node(GROUP)
{
    Group::swap(group);
}

Group::~Group() { }

Node* Group::clone(void) 
{
    Group* group = new Group(*this);
    return group;
}

void Group::swap(const Group &group)
{
    clear();
    strName = group.strName;
    groupOwner = group.groupOwner;
    behaviors = group.behaviors;
    groups = group.groups;
    conditions = group.conditions;
}

