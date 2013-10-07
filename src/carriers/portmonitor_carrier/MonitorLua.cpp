// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <yarp/os/Log.h>
#include <lua.hpp>
#include "MonitorLua.h"

using namespace yarp::os;

#ifdef _MSC_VER
#define safe_printf sprintf_s
#else
#define safe_printf snprintf
#endif 


/**
 * Class MonitorLua
 */

MonitorLua::MonitorLua(void)
{
    L = luaL_newstate();
    luaL_openlibs(L);
}

MonitorLua::~MonitorLua()
{
    lua_close(L);
}

bool MonitorLua::loadScript(const char* script_file)
{
    //printf("script : %s\n", script_file);
    //printf("%s (%d)\n", __FILE__, __LINE__);
    if(luaL_loadfile(L, script_file))
    {   
        ConstString msg = lua_tostring(L, -1);
        lua_pop(L,1);
        if(luaL_loadfile(L, (ConstString(script_file)+".lua").c_str()))
        {
            YARP_LOG_ERROR("Cannot load script file");            
            YARP_LOG_ERROR(msg);
            lua_pop(L,1);
        return false;
        }
    }

    if(lua_pcall(L,0, LUA_MULTRET, 0))
    {
        YARP_LOG_ERROR("Cannot run script file");
        YARP_LOG_ERROR(lua_tostring(L, -1));
        lua_pop(L,1);
        return false;
    }

    return true;
}

bool MonitorLua::setParams(const yarp::os::Property& params)
{
    return true;
}

bool MonitorLua::getParams(yarp::os::Property& params)
{
    return true;
}

yarp::os::ConnectionReader& MonitorLua::updateData(yarp::os::ConnectionReader& reader)
{

    return reader;
}


