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

    return true;
}

