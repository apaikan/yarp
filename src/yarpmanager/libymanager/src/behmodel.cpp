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

    printf("beh:%d, grp:%d\n", behaviors.size(), groups.size());

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
                chgroup->setOwner(group);
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
                behavior->setOwner(group);
            }
        }
    }
    
    PRINT_GRAPH(behGraph);
    exportBehaviorDotGraph(behGraph, "./beh.dot");

    return true;
}

