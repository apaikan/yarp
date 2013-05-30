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
    const char* getAt(void) { return strAt.c_str(); }
    const char* getPort(void) { return strPort.c_str(); }    
    bool operator==(const Configuration& alt) {      
        return (strAt == alt.strAt); 
    }
 
private:
    string strAt;
    string strPort;
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
    Behavior(const char* szName);
    Behavior(const Behavior &beh);
    virtual ~Behavior();
    virtual Node* clone(void);        

    void addConfiguration(Configuration& config) { configurations.push_back(config); }
    int configurationCount(void) { return configurations.size(); }
    Configuration& getConfigurationAt(int index) { return configurations[index]; }

    void addInhibition(const char* inhib) { inhibitions.push_back(inhib); }
    int inhibitionCount(void) { return inhibitions.size(); }
    const char* getInhibitionAt(int index) { return inhibitions[index].c_str(); }

    void setCondition(const char* cond) { if(cond) conditions = cond; }
    const char* getCondition(void) { return conditions.c_str(); }

    void setOwner(Node* owner) { behOwner = owner; }
    Node* owner(void) { return behOwner; }

    void clear(void) {
        behOwner = NULL;
        strName.clear();
        configurations.clear();
        inhibitions.clear();
        conditions.clear();
    }

    bool operator==(const Behavior& alt) {      
        return (strName == alt.strName); 
    }
 
private:
    Node* behOwner;
    string strName; 
    ConfigContainer configurations;
    vector<string> inhibitions;
    string conditions;

private:
    void swap(const Behavior &beh);

};


/**
 * Class Group  
 */
class Group : public Node{

public: 
    Group(const char* szName);
    Group(const Group &group);
    virtual ~Group();
    virtual Node* clone(void);        

    void addBehavior(const char* beh) { behaviors.push_back(beh); }
    int behaviorCount(void) { return behaviors.size(); }
    const char* getBehaviorAt(int index) { return behaviors[index].c_str(); }

    void addGroup(const char* group) { groups.push_back(group); }
    int groupCount(void) { return groups.size(); }
    const char* getGroupAt(int index) { return groups[index].c_str(); }

    void setCondition(const char* cond) { if(cond) conditions = cond; }
    const char* getCondition(void) { return conditions.c_str(); }

    void setOwner(Node* owner) { groupOwner = owner; }
    Node* owner(void) { return groupOwner; }

    void clear(void) {
        groupOwner = NULL;
        strName.clear();
        behaviors.clear();
        groups.clear();
        conditions.clear();
    }

    bool operator==(const Group& alt) {      
        return (strName == alt.strName); 
    }
 
private:
    Node* groupOwner;
    string strName; 
    vector<string> behaviors;
    vector<string> groups;
    string conditions;

private:
    void swap(const Group &group);

};



typedef vector<Behavior*> BehaviorPContainer;
typedef vector<Behavior*>::iterator BehaviorPIterator;
typedef vector<Group*> GroupPContainer;
typedef vector<Group*>::iterator GroupPIterator;


#endif //__BEHAVIOR__
