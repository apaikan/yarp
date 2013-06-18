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
    owners = beh.owners;
    configurations = beh.configurations;
    inhibitions = beh.inhibitions;
    conditions = beh.conditions;
}


bool Behavior::hasParent(const char* parentName)
{
    bool bParent = false;
    vector<Node*>::iterator itr;
    for(itr = owners.begin(); itr != owners.end(); itr++) 
    {
        BehaviorGroup* parent = dynamic_cast<BehaviorGroup*>(*itr);
        bParent |= (strcmp(parent->getName(), parentName) == 0); 
        bParent |= parent->hasParent(parentName);
    } 
    
    return bParent;
}

const vector<string>& Behavior::getInheritedCondition(void)
{
    string tmpCond;
    inheritedConditions.clear();
    vector<Node*>::iterator itr;
    for(itr = owners.begin(); itr != owners.end(); itr++) 
    {
        BehaviorGroup* parent = dynamic_cast<BehaviorGroup*>(*itr);
        vector<string> parentConds = parent->getInheritedCondition();
        if(parentConds.size())
        {
            if(tmpCond.length())
                tmpCond += string(" | (") + parentConds[0] + string(")");
            else
                tmpCond = string("(") + parentConds[0] + string(")");            
        }            
    }
    
    if(conditions.length())
    {
        if(tmpCond.length())
            tmpCond = conditions + " & (" + tmpCond + ")";  
        else
            tmpCond = conditions;
    }

    if(tmpCond.length())
        inheritedConditions.push_back(tmpCond);
    return inheritedConditions;
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
    owners = group.owners;
    behaviors = group.behaviors;
    groups = group.groups;
    conditions = group.conditions;
}

bool BehaviorGroup::hasParent(const char* parentName)
{
    bool bParent = false;
    vector<Node*>::iterator itr;
    for(itr = owners.begin(); itr != owners.end(); itr++) 
    {
        BehaviorGroup* parent = dynamic_cast<BehaviorGroup*>(*itr);
        bParent |= (strcmp(parent->getName(), parentName) == 0); 
        bParent |= parent->hasParent(parentName);
    } 
    
    return bParent;
}


const vector<string>& BehaviorGroup::getInheritedCondition(void)
{
    string tmpCond;
    inheritedConditions.clear();
    vector<Node*>::iterator itr;
    for(itr = owners.begin(); itr != owners.end(); itr++) 
    {
        BehaviorGroup* parent = dynamic_cast<BehaviorGroup*>(*itr);
        vector<string> parentConds = parent->getInheritedCondition();
        if(parentConds.size())
        {
            if(tmpCond.length())
                tmpCond += string(" | (") + parentConds[0] + string(")");
            else
                tmpCond = string("(") + parentConds[0] + string(")");            
        }            
    }
    
    if(conditions.length())
    {
        if(tmpCond.length())
            tmpCond = conditions + " & (" + tmpCond + ")";  
        else
            tmpCond = conditions;
    }
    if(tmpCond.length())
        inheritedConditions.push_back(tmpCond);
    return inheritedConditions;

    /*
    inheritedConditions.clear();
    vector<Node*>::iterator itr;
    for(itr = owners.begin(); itr != owners.end(); itr++) 
    {
        BehaviorGroup* parent = dynamic_cast<BehaviorGroup*>(*itr);
        vector<string> parentConds = parent->getInheritedCondition();
        for(size_t i=0; i<parentConds.size(); i++)
        {
            if(std::find(inheritedConditions.begin(), 
                         inheritedConditions.end(), parentConds[i]) == inheritedConditions.end() )
                inheritedConditions.push_back(parentConds[i]);
        }                         
        //inheritedConditions.insert(inheritedConditions.end(), 
        //                           parentConds.begin(), parentConds.end());
    }
    // add its own condition to the list
    if(conditions.length())
        inheritedConditions.push_back(conditions);
    return inheritedConditions;
    */
}


