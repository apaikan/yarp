// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#ifndef __BEHAVIOR_H_
#define __BEHAVIOR_H_

#include <vector>

#include "ymm-types.h" 
#include "node.h"
#include "utility.h"

using namespace std; 

//namespace ymm {

/*
 * Class Configuration
 */
class Configuration {
public:
    Configuration(void) {}
    Configuration(const char* at, const char* port) {
        if(at) { strAt = at;}
        if(port) { strPort = port; }        
    }
    ~Configuration() {}

    void setAt(const char* at) { if(at) strAt = at; }
    void setPort(const char* port) { if(port) strPort = port; }
    void setBlocker(const char* blocker) { if(blocker) strBlocker = blocker; }
    const char* getAt(void) { return strAt.c_str(); }
    const char* getPort(void) { return strPort.c_str(); }    
    const char* getBlocker(void) { return strBlocker.c_str(); }
    bool operator==(const Configuration& alt) {
        return (strAt == alt.strAt); 
    }
 
private:
    string strAt;
    string strPort;
    string strBlocker;
};

typedef vector<Configuration> ConfigContainer;
typedef vector<Configuration>::iterator ConfigIterator;
typedef vector<string> InhibContainer;
typedef vector<string>::iterator InhibIterator;


/**
 * Class Behavior  
 */
class Behavior : public Node{

public: 
    Behavior(void);
    Behavior(const char* szName);
    Behavior(const Behavior &beh);
    virtual ~Behavior();
    virtual Node* clone(void);        
    
    void setName(const char* szName) { if(szName) strName = szName; }
    const char* getName(void) { return strName.c_str(); }

    void addConfiguration(Configuration& config) { configurations.push_back(config); }
    int configurationCount(void) { return configurations.size(); }
    Configuration& getConfigurationAt(int index) { return configurations[index]; }

    void addInhibition(const char* inhib) { if(inhib) inhibitions.push_back(inhib); }
    int inhibitionCount(void) { return inhibitions.size(); }
    const char* getInhibitionAt(int index) { return inhibitions[index].c_str(); }

    void setCondition(const char* cond) { if(cond) conditions = cond; }
    const char* getCondition(void) { return conditions.c_str(); }

    const vector<string>& getInheritedCondition(void);

    void addOwner(Node* owner) { if(owner) owners.push_back(owner); }
    int ownerCount(void) { return owners.size(); }
    const Node* getOwnerAt(int index) { return owners[index]; }

    void clear(void) {
        strName.clear();
        configurations.clear();
        inhibitions.clear();
        conditions.clear();
        owners.clear();
    }

    Behavior& operator=(const Behavior& rhs);

    bool operator==(const Behavior& alt) {      
        return (strName == alt.strName); 
    }
 
private:
    string strName; 
    ConfigContainer configurations;
    vector<string> inhibitions;
    vector<Node*> owners;
    string conditions;
    vector<string> inheritedConditions;

private:
    void swap(const Behavior &beh);

};


/**
 * Class BehaviorGroup  
 */
class BehaviorGroup : public Node{

public: 
    BehaviorGroup(void);
    BehaviorGroup(const char* szName);
    BehaviorGroup(const BehaviorGroup &group);
    virtual ~BehaviorGroup();
    virtual Node* clone(void);        

    void setName(const char* szName) { if(szName) strName = szName; }
    const char* getName(void) { return strName.c_str(); }

    void addBehavior(const char* beh) { if(beh) behaviors.push_back(beh); }
    int behaviorCount(void) { return behaviors.size(); }
    const char* getBehaviorAt(int index) { return behaviors[index].c_str(); }

    void addBehaviorGroup(const char* group) { if(group) groups.push_back(group); }
    int groupCount(void) { return groups.size(); }
    const char* getBehaviorGroupAt(int index) { return groups[index].c_str(); }

    void setCondition(const char* cond) { if(cond) conditions = cond; }
    const char* getCondition(void) { return conditions.c_str(); }

    const vector<string>& getInheritedCondition(void);

    void addOwner(Node* owner) { if(owner) owners.push_back(owner); }
    int ownerCount(void) { return owners.size(); }
    const Node* getOwnerAt(int index) { return owners[index]; }

    void clear(void) {
        strName.clear();
        behaviors.clear();
        groups.clear();
        conditions.clear();
        owners.clear();
    }

    BehaviorGroup& operator=(const BehaviorGroup& rhs);

    bool operator==(const BehaviorGroup& alt) {      
        return (strName == alt.strName); 
    }
 
private:
    string strName; 
    vector<string> behaviors;
    vector<string> groups;
    vector<Node*> owners;
    string conditions;
    vector<string> inheritedConditions;

private:
    void swap(const BehaviorGroup &group);

};



typedef vector<Behavior*> BehaviorPContainer;
typedef vector<Behavior*>::iterator BehaviorPIterator;
typedef vector<BehaviorGroup*> BehaviorGroupPContainer;
typedef vector<BehaviorGroup*>::iterator BehaviorGroupPIterator;


#endif //__BEHAVIOR__
