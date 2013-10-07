// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include "MonitorBinding.h"
#include "MonitorLua.h"

using namespace yarp::os;

#ifdef _MSC_VER
#define safe_printf sprintf_s
#else
#define safe_printf snprintf
#endif 


/**
 * Class MonitorBinding
 */

MonitorBinding::~MonitorBinding() 
{
} 

MonitorBinding* MonitorBinding::create(const char* script_type) 
{
    if(ConstString(script_type) == "lua")
        return new MonitorLua();
    return NULL;        
}


