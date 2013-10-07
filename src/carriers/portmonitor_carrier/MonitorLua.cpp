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

    lua_getglobal(L, "PortMonitor");
    if(lua_istable(L, -1) == 0) 
    {
        YARP_LOG_ERROR("The script file does not contain any valid \'PortMonitor\' object.");
        lua_pop(L, 1);
        return false;
    }

    bool result = true;
    //  call PortMonitor.create if exists
    if(getLocalFunction("create"))
    {
        if(lua_pcall(L, 0, 1, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            result = false;
        }            
        else    
            result = lua_toboolean(L, -1);
        lua_pop(L,1);
    }

    return result;
}

yarp::os::ConnectionReader& MonitorLua::updateData(yarp::os::ConnectionReader& reader)
{
    if(getLocalFunction("update"))
    {
        // mapping to swig type
        swig_type_info *readerType = SWIG_TypeQuery(L, "yarp::os::ConnectionReader *");        
        if(!readerType)
        {            
            YARP_LOG_ERROR("Swig type of ConnectionReader is not found");
            lua_pop(L, 1);
            return reader;
        }
        
        // getting the swig-type pointer
        SWIG_NewPointerObj(L, &reader, readerType, 0);
        if(lua_pcall(L, 1, 1, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            return reader;
        }

        // converting the results
        yarp::os::ConnectionReader* result;
        if(SWIG_Lua_ConvertPtr(L, -1, (void**)(&result), readerType, 0) != SWIG_OK )
        {
            YARP_LOG_ERROR("Cannot get a valid return value from PortMonitor.update");
            lua_pop(L, 1);
            return reader;
        }   
        else        
        {
            lua_pop(L, 1);
            return *result;
        }
    }

    lua_pop(L, 1);
    return reader;
}

bool MonitorLua::setParams(const yarp::os::Property& params)
{
    return true;
}

bool MonitorLua::getParams(yarp::os::Property& params)
{
    return true;
}


bool MonitorLua::getLocalFunction(const char *name) 
{
  lua_pushstring(L, name);
  lua_gettable(L, -2);
  return (lua_isfunction(L, -1) == 1);
}


