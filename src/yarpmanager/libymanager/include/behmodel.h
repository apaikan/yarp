// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#ifndef __BEHAVIOR_MODEL_H_
#define __BEHAVIOR_MODEL_H_

#include <vector>

#include "ymm-types.h" 
#include "graph.h"
#include "utility.h"

#include "behavior.h"
#include "manifestloader.h"
#include "arbitrator.h"

using namespace std; 

//namespace ymm {

/*
 * Class BehaviorModel
 */
class BehaviorModel {
public:
    BehaviorModel(void);
    ~BehaviorModel();

    bool createFrom(BehModelLoader& bloader);
    vector<Arbitrator>& getArbitrators(void);

    Graph& getGraph(void) { return behGraph; }

private:
    Arbitrator& getArbitrator(const char* port);
    void getInhibitors(Behavior& behavior, 
                       Configuration& conf, 
                       vector<string>& inhibitors);

private:
    Graph behGraph;
    vector<Arbitrator> arbitrators;
};


#endif //__BEHAVIOR_MODEL__