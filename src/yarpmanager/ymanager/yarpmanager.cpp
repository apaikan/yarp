/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */


#include "ymanager.h"

#include "xmlbehloader.h"
#include "behmodel.h"

int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    yarp.setVerbosity(-1);

    XmlBehModelLoader ld("./test.xml");
    BehaviorModel model;
    model.createFrom(ld);

    ErrorLogger* logger  = ErrorLogger::Instance(); 
    if(logger->errorCount() || logger->warningCount())
    {
        const char* msg;
        while((msg=logger->getLastError()))
            cout<<"ERROR:   "<<msg<<endl;

        while((msg=logger->getLastWarning()))
            cout<<"WARNING: "<<msg<<endl;
    }   

    //YConsoleManager ymanager(argc, argv);

    return 0;
}

