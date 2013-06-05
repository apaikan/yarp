/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 *
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#include "behmodel.h"
#include <stdio.h>
#include <string>
using namespace std;


/*
 * Behavior
 */

BehaviorModel::BehaviorModel(void) 
{
}

BehaviorModel::~BehaviorModel()
{

}

bool BehaviorModel::createFrom(BehModelLoader& bloader) 
{
    ErrorLogger* logger  = ErrorLogger::Instance();
    if(!bloader.init())
    {
        logger->addError("Cannot create beahvior model");
        return false;
    }
    
    std::vector<Behavior>& behaviors = bloader.getBehaviors();
    std::vector<BehaviorGroup>& groups = bloader.getBehaviorGroups();

    // adding all behaviors as the leaf to the graph
    std::vector<Behavior>::iterator itr;
    for(itr = behaviors.begin(); itr != behaviors.end(); itr++)
    {
        Behavior* behavior = &(*itr);
        behavior->setLabel(behavior->getName());
        if(behGraph.hasNode(behavior))
        {
            OSTRINGSTREAM msg;
            msg<<"Cannot create doublicate behavior with the same name "<<behavior->getName();
            logger->addError(msg);
            return false;
        }

        /*Adding behavior to the graph */
        if(!(behavior = (Behavior*)behGraph.addNode(behavior)))
            return false;
    }

    // adding all groups to the graph
    std::vector<BehaviorGroup>::iterator jtr;
    for(jtr = groups.begin(); jtr != groups.end(); jtr++)
    {
        BehaviorGroup* group = &(*jtr);
        group->setLabel(group->getName());
        if(behGraph.hasNode(group))
        {
            OSTRINGSTREAM msg;
            msg<<"Cannot create doublicate group with the same name "<<group->getName();
            logger->addError(msg);
            return false;
        }

        /*Adding group to the graph */
        if(!(group = (BehaviorGroup*)behGraph.addNode(group)))
            return false;               
        
        /*
        // adding child behaviors as seperate nodes
        for(int i=0; i<group->behaviorCount(); i++)
        {
            // check if the original behavior exisits
            Behavior* behavior = dynamic_cast<Behavior*>(behGraph.getNode(group->getBehaviorAt(i)));
            if(!behavior)
            {
                OSTRINGSTREAM msg;
                msg<<"behavior "<<group->getBehaviorAt(i)<<" from "<<group->getName()<<" does not exist!";
                logger->addError(msg);
                return false;
            }
            
            // if it is an unsued behavior node, employ it
            if(!behavior->owner())
            {
                string behLabel = string(group->getName()) + string(behavior->getName());
                behavior->setLabel(behLabel.c_str());
            }
            else // create a new behvaior node
            {
                // check if there is not any child behavior with the same name in the group
                string behLabel = string(group->getName()) + string(behavior->getName());
                Behavior* newBehavior = dynamic_cast<Behavior*>(behGraph.getNode(behLabel.c_str()));
                if(newBehavior)
                {
                    OSTRINGSTREAM msg;
                    msg<<"Cannot create doublicate behavior with the same name "<<behavior->getName();
                    msg<<" in group "<<group->getName();
                    logger->addError(msg);
                    return false;
                }

                // create a new behavior node and add it to the graph
                newBehavior = new Behavior(*behavior);
                newBehavior->setLabel(behLabel.c_str());
                
                //Adding new behavior to the graph
                if(!(behavior = (Behavior*)behGraph.addNode(newBehavior)))
                {
                    return false;
                    delete newBehavior;
                }            
                delete newBehavior;
            }
            // make a link to the currently added behavior 
            behGraph.addLink(group, behavior, 0);
            behavior->setOwner(group);            
        }

        // adding child group as seperate nodes
        for(int i=0; i<group->groupCount(); i++)
        {
            // check if the original group exisits
            BehaviorGroup* chgroup = dynamic_cast<BehaviorGroup*>(behGraph.getNode(group->getBehaviorGroupAt(i)));
            if(!chgroup)
            {
                OSTRINGSTREAM msg;
                msg<<"Child group "<<group->getBehaviorGroupAt(i)<<" from "<<group->getName()<<" does not exist!";
                logger->addError(msg);
                return false;
            }
            // if it is an unsued group node, employ it
            if(!chgroup->owner())
            {
                string label = string(group->getName()) + string(chgroup->getName());
                chgroup->setLabel(label.c_str());
            }
            else // create a new child group node
            {
                // check if there is not any child group with the same name in the group
                string label = string(group->getName()) + string(chgroup->getName());
                BehaviorGroup* newGroup = dynamic_cast<BehaviorGroup*>(behGraph.getNode(label.c_str()));
                if(newGroup)
                {
                    OSTRINGSTREAM msg;
                    msg<<"Cannot create doublicate group with the same name "<<chgroup->getName();
                    msg<<" in group "<<group->getName();
                    logger->addError(msg);
                    return false;
                }

                // create a new group node and add it to the graph
                newGroup = new BehaviorGroup(*chgroup);
                newGroup->setLabel(label.c_str());
                
                //Adding new behavior to the graph //
                if(!(chgroup = (BehaviorGroup*)behGraph.addNode(newGroup)))
                {
                    return false;
                    delete newGroup;
                }            
                delete newGroup;
            }

            // make a link to the currently added group
            behGraph.addLink(group, chgroup, 0);
            chgroup->setOwner(group);
        }
        */
        
    }
    
    // adding the links 
    for(GraphIterator itr=behGraph.begin(); itr!=behGraph.end(); itr++)
    {
        BehaviorGroup* group = dynamic_cast<BehaviorGroup*>(*itr);
        if(group)
        {
            // adding link to the child groups
            for(int i=0; i<group->groupCount(); i++)
            {
                BehaviorGroup* chgroup = dynamic_cast<BehaviorGroup*>(behGraph.getNode(group->getBehaviorGroupAt(i)));
                if(!chgroup)
                {
                    OSTRINGSTREAM msg;
                    msg<<"Child group "<<group->getBehaviorGroupAt(i)<<" from "<<group->getName()<<" does not exist!";
                    logger->addError(msg);
                    return false;
                }
                behGraph.addLink(group, chgroup, 0);
                chgroup->addOwner(group);
            }

            // ading link to the behaviors
            for(int i=0; i<group->behaviorCount(); i++)
            {
                Behavior* behavior = dynamic_cast<Behavior*>(behGraph.getNode(group->getBehaviorAt(i)));
                if(!behavior)
                {
                    OSTRINGSTREAM msg;
                    msg<<"behavior "<<group->getBehaviorAt(i)<<" from "<<group->getName()<<" does not exist!";
                    logger->addError(msg);
                    return false;
                }
                behGraph.addLink(group, behavior, 0);
                behavior->addOwner(group);
            }
        }
    }
    
    PRINT_GRAPH(behGraph);
    exportBehaviorDotGraph(behGraph, "./beh.dot");
    exportDotGraph(behGraph, "./graph.dot");
    getArbitrators();
    return true;
}

Arbitrator& BehaviorModel::getArbitrator(const char* port) 
{
    vector<Arbitrator>::iterator itr;
    for(itr=arbitrators.begin(); itr!=arbitrators.end(); itr++)
        if(strcmp((*itr).getPort(), port) == 0)
            return (*itr);

    Arbitrator arb(port);
    arbitrators.push_back(arb);
    return arbitrators.back();
}

void BehaviorModel::getInhibitors(Behavior& behavior, Configuration& conf, 
                                  vector<string>& inhibitors)
{
    for(GraphIterator itr=behGraph.begin(); itr!=behGraph.end(); itr++)
    {
        Behavior* beh = dynamic_cast<Behavior*>(*itr);
        if(beh && (*beh != behavior))
        {
            // cheking the inhibtion tags
            for(int i=0; i<beh->inhibitionCount(); i++)
            {
                // there is a behvaior which inhibits the current behavior 
                if(strcmp(beh->getInhibitionAt(i), behavior.getName()) == 0)
                {
                    for(int i=0; i<beh->configurationCount(); i++)
                    {
                        Configuration beh_cfg = beh->getConfigurationAt(i);
                        // the inhibitor conflicts with the current behavior at a port
                        if(strcmp(beh_cfg.getAt(), conf.getAt()) == 0)
                        {
                            string inhibitor_port; 
                            // check if it is a blocking port
                            if((strlen(beh_cfg.getBlocker()) != 0))
                            {
                                // not a self inhibitor
                                if(strcmp(beh_cfg.getBlocker(), conf.getBlocker()) != 0)
                                    inhibitor_port = beh_cfg.getBlocker();
                            }                                
                            else if (strcmp(beh_cfg.getPort(), conf.getPort()) != 0) // not a self inhibitor
                                inhibitor_port = beh_cfg.getPort();
                            
                            if(inhibitor_port.length() && 
                               std::find(inhibitors.begin(), inhibitors.end(), inhibitor_port) == inhibitors.end() )
                            {   
                                inhibitors.push_back(inhibitor_port);
                            }                                
                        }
                    }                   
                }
            } // end for inhibitionCount

            /*
            // checking the configuration tags from other behaviors for any direct blocking
            for(int i=0; i<beh->configurationCount(); i++)
            {
                Configuration beh_cfg = beh->getConfigurationAt(i);
                if((strcmp(beh_cfg.getAt(), conf.getAt()) == 0) && 
                   (strlen(beh_cfg.getBlocker()) != 0) &&
                   (std::find(inhibitors.begin(), inhibitors.end(), beh_cfg.getBlocker()) == inhibitors.end()) )
                    inhibitors.push_back(beh_cfg.getBlocker());
            }
            */
        }
    }
}

vector<Arbitrator>& BehaviorModel::getArbitrators(void)
{ 
    // make a map of all inhibitors to the ports
    std::map<string, vector<string> > arbPorts;
    for(GraphIterator itr=behGraph.begin(); itr!=behGraph.end(); itr++)
    {
        Behavior* behavior = dynamic_cast<Behavior*>(*itr);
        if(behavior)
        {
            for(int i=0; i<behavior->configurationCount(); i++)
            {
                Configuration conf = behavior->getConfigurationAt(i);
                // first deal with inhibitors (not blockers)
                if(strlen(conf.getBlocker()) == 0)
                {
                    // get all inhibitors and add them to the rule
                    vector<string> inhibitors;
                    getInhibitors(*behavior, conf, arbPorts[conf.getPort()]);
                }
                /*
                 * TODO: investigate whether we need to inhbits blockers or not 
                 */
            }
        }
    }        

    // create arbitrators 
    arbitrators.clear();
    for(GraphIterator itr=behGraph.begin(); itr!=behGraph.end(); itr++)
    {
        Behavior* behavior = dynamic_cast<Behavior*>(*itr);
        if(behavior)
        {
            for(int i=0; i<behavior->configurationCount(); i++)
            {
                Configuration conf = behavior->getConfigurationAt(i);
                Arbitrator& arb = getArbitrator(conf.getAt());
                if(strlen(conf.getBlocker()) == 0)
                {
                    // get all inhibitors and add them to the rule
                    vector<string>& inhibitors = arbPorts[conf.getPort()];
                    string rule;
                    for(size_t i=0; i<inhibitors.size(); i++)
                        if(!rule.length())
                            rule = string("~ ") + inhibitors[i];
                        else
                            rule = rule + string(" & ~ ") + inhibitors[i];
                    
                    vector<string> conds = behavior->getInheritedCondition();
                    if(conds.size())
                    {
                        if(rule.length())
                            rule = rule + string(" & (") + conds[0] + string(")");
                        else
                            rule = conds[0];
                    }
                    if(rule.length())
                        arb.addRule(conf.getPort(), rule.c_str());
                }
            }
        }
    }        


   
   /*
    std::map<string, vector<string> >::iterator itr;
    for(itr=arbPorts.begin(); itr!=arbPorts.end(); itr++)
    {
        printf("\n%s:\n", (itr->first).c_str());
        vector<string>& inhibitors = itr->second;
        for(size_t i=0; i<inhibitors.size(); i++)
            printf("\t%s\n", inhibitors[i].c_str());
    }
    */
    vector<Arbitrator>::iterator itr;
    for(itr=arbitrators.begin(); itr!=arbitrators.end(); itr++)
    {
        Arbitrator& arb = (*itr);
        printf("\n%s:\n",arb.getPort() );
        map<std::string, std::string>& rules = arb.getRuleMap();
        map<std::string, std::string>::iterator jtr;
        for(jtr=rules.begin(); jtr!=rules.end(); jtr++)
            printf("\t%s = %s\n", jtr->first.c_str(), jtr->second.c_str());
    }

    return arbitrators;
}

