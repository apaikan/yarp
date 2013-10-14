// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <yarp/os/Network.h>
#include <yarp/os/Log.h>
#include <lua.hpp>
#include "MonitorLua.h"

using namespace yarp::os;

/**
 * Class MonitorLua
 */
MonitorLua::MonitorLua(void)
{
    L = luaL_newstate();
    luaL_openlibs(L);

    /**
     * Registring extra functions for lua :
     *  - PortMonitor.setEvent()
     *  - PortMonitor.unsetEvent()
     */ 
    registerExtraFunctions();
}

MonitorLua::~MonitorLua()
{
    if(L)
    {
        //  call PortMonitor.destroy if exists
        if(getLocalFunction("destroy"))
        {
            if(lua_pcall(L, 0, 0, 0) != 0)
                YARP_LOG_ERROR(lua_tostring(L, -1));
        }
        // closing lua state handler
        lua_close(L);
    }        
}

bool MonitorLua::loadScript(const char* script_file)
{
    //printf("%s (%d)\n", __FILE__, __LINE__);
    if(luaL_loadfile(L, script_file))
    {   
        YARP_LOG_ERROR("Cannot load script file");            
        YARP_LOG_ERROR(lua_tostring(L, -1));
        lua_pop(L,1);
        lua_close(L);
        L = NULL;
        return false;
    }

    if(lua_pcall(L,0, LUA_MULTRET, 0))
    {
        YARP_LOG_ERROR("Cannot run script file");
        YARP_LOG_ERROR(lua_tostring(L, -1));
        lua_pop(L,1);
        lua_close(L);
        L = NULL;
        return false;
    }

    /**
     * TODO: make PortMonitor's element read only!
     */
    lua_pushlightuserdata(L, this);
    lua_setglobal(L, "PortMonitor_Owner");

    lua_getglobal(L, "PortMonitor");
    if(lua_istable(L, -1) == 0) 
    {
        YARP_LOG_ERROR("The script file does not contain any valid \'PortMonitor\' object.");
        lua_pop(L, 1);
        lua_close(L);
        L = NULL;
        return false;
    }

    bool result = true;
    //  call PortMonitor.create if exists
    if(getLocalFunction("create"))
    {
        if(lua_pcall(L, 0, 1, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            lua_close(L);
            L = NULL;
            return false;
        }            
        else    
            result = lua_toboolean(L, -1);
    }
    
    lua_pop(L,1);
    return result;
}

bool MonitorLua::acceptData(yarp::os::ConnectionReader& reader)
{
    if(getLocalFunction("accept"))
    {
        // mapping to swig type
        swig_type_info *readerType = SWIG_TypeQuery(L, "yarp::os::ConnectionReader *");        
        if(!readerType)
        {            
            YARP_LOG_ERROR("Swig type of ConnectionReader is not found");
            lua_pop(L, 1);
            return false;
        }
        
        // getting the swig-type pointer
        SWIG_NewPointerObj(L, &reader, readerType, 0);
        if(lua_pcall(L, 1, 1, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }
            
        // converting the results        
        bool result = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return result;        
    }

    lua_pop(L, 1);
    return true;
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

    lua_pop(L,1);
    return reader;
}

bool MonitorLua::setParams(const yarp::os::Property& params)
{
    if(getLocalFunction("setparam"))
    {
        // mapping to swig type
        swig_type_info *propType = SWIG_TypeQuery(L, "yarp::os::Property *");
        if(!propType)
        {            
            YARP_LOG_ERROR("Swig type of Property is not found");
            lua_pop(L, 1);
            return false;
        }
        
        // getting the swig-type pointer
        SWIG_NewPointerObj(L, &params, propType, 0);
        if(lua_pcall(L, 1, 0, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }
        return true;
    }

    lua_pop(L,1);
    return true;
}

bool MonitorLua::getParams(yarp::os::Property& params)
{
    if(getLocalFunction("getparam"))
    {
        // mapping to swig type
        swig_type_info *propType = SWIG_TypeQuery(L, "yarp::os::Property *");
        if(!propType)
        {            
            YARP_LOG_ERROR("Swig type of Property is not found");
            lua_pop(L, 1);
            return false;
        }
        
        // calling PortMonitor.getparam from lua
        if(lua_pcall(L, 0, 1, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }

        // converting the results
        yarp::os::Property* result;
        if(SWIG_Lua_ConvertPtr(L, -1, (void**)(&result), propType, 0) != SWIG_OK )
        {
            YARP_LOG_ERROR("Cannot get a valid return value from PortMonitor.getparam");
            lua_pop(L, 1);
            return false;
        }   
        else        
        {
            params = *result;
            lua_pop(L, 1);
            return true;
        }
    }

    lua_pop(L,1);
    return true;
}

bool MonitorLua::peerTrigged(void)
{
    if(getLocalFunction("trig"))
    {
        if(lua_pcall(L, 0, 0, 0) != 0)
        {
            YARP_LOG_ERROR(lua_tostring(L, -1));
            lua_pop(L, 1);
            return false;
        }
        return true;
    }

    lua_pop(L, 1);
    return true;
}

bool MonitorLua::setAcceptConstraint(const char* constraint)
{
    if(!constraint)
        return false;

    MonitorLua::constraint = constraint;
    return true;
}



bool MonitorLua::getLocalFunction(const char *name) 
{
  lua_pushstring(L, name);
  lua_gettable(L, -2);
  return (lua_isfunction(L, -1) == 1);
}


bool MonitorLua::registerExtraFunctions(void)
{
    luaL_openlib(L, "PortMonitor", MonitorLua::portMonitorLib, 0);
    return true;
}


bool MonitorLua::canAccept(void)
{
    if(constraint == "")
        return true;

    return true;
}

int MonitorLua::setEvent(lua_State* L)
{
    const char *event_name = luaL_checkstring(L, 1);
    if(event_name)
    {
        lua_getglobal(L, "PortMonitor_Owner");
        if(!lua_islightuserdata(L, -1))
        {
            YARP_LOG_ERROR("Cannot get PortMonitor_Owner");
            return 0;
        }
        MonitorLua* owner = static_cast<MonitorLua*>(lua_touserdata(L, -1));
        MonitorEventRecord& record = MonitorEventRecord::getInstance();
        record.lock();
        record.setEvent(event_name, owner);
        record.unlock();
    }        
    return 0;
}

int MonitorLua::unsetEvent(lua_State* L)
{
    const char *event_name = luaL_checkstring(L, 1);
    if(event_name)
    {
        lua_getglobal(L, "PortMonitor_Owner");
        if(!lua_islightuserdata(L, -1))
        {
            YARP_LOG_ERROR("Cannot get PortMonitor_Owner");
            return 0;
        }
        MonitorLua* owner = static_cast<MonitorLua*>(lua_touserdata(L, -1));
        MonitorEventRecord& record = MonitorEventRecord::getInstance();
        record.lock();
        record.unsetEvent(event_name, owner);
        record.unlock();
    }        
    return 0;
}

const struct luaL_reg MonitorLua::portMonitorLib [] = {
    {"setEvent", MonitorLua::setEvent},
    {"unsetEvent", MonitorLua::unsetEvent},
    {NULL, NULL}
};


