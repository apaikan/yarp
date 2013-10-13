// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef _MONITORLUA_INC_
#define _MONITORLUA_INC_

#include <yarp/os/ConstString.h>
#include "MonitorBinding.h"
#include "lua_swig.h"

class MonitorLua : public MonitorBinding
{

public:
    MonitorLua();
    virtual ~MonitorLua();
    
    bool loadScript(const char* script_file);
    bool setParams(const yarp::os::Property& params);
    bool getParams(yarp::os::Property& params);
    bool acceptData(yarp::os::ConnectionReader& reader);
    yarp::os::ConnectionReader& updateData(yarp::os::ConnectionReader& reader);    
    bool peerTrigged(void);
    bool setAcceptConstraint(const char* constraint);
    bool canAccept(void);

private:
    bool getLocalFunction(const char *name);

private:
    lua_State *L;   
    yarp::os::ConstString constraint;

};

#endif //_MONITORLUA_INC_


