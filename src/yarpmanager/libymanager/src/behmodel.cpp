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
                if((strcmp(beh->getInhibitionAt(i), behavior.getName()) == 0) || 
                  behavior.hasParent(beh->getInhibitionAt(i)) )
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
            //printf("%s:\n", behavior->getName());
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
                    //printf("\t%s: %s\n", conf.getPort(), conds[0].c_str());
                    if(conds.size())
                    {
                        if(rule.length())
                            rule = rule + string(" & (") + conds[0] + string(")");
                        else
                            rule = conds[0];
                    }
                    if(rule.length())
                    {
                        //printf("\t@%s, %s : %s\n", conf.getAt(), conf.getPort(), rule.c_str());
                        string old = arb.getRule(conf.getPort());
                        if(old.length())
                            rule = string("(") + old + string(") | (") + rule + string(")"); 
                        arb.addRule(conf.getPort(), rule.c_str());                     
                    }   
                }
            }
            //printf("\n\n");
        }
    } 

    return arbitrators;
}

