// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include <yarp/os/Log.h>
#include <yarp/os/ConstString.h>

#include "LuaModifier.h"


using namespace yarp::os;

#ifdef _MSC_VER
#define safe_printf sprintf_s
#else
#define safe_printf snprintf
#endif 


/**
 * Class LuaModifier
 */

// Read connection settings.
bool LuaModifier::configure(yarp::os::ConnectionState& proto) 
{    
    printf("configure\n");
    portName = proto.getRoute().getToName();
    sourceName = proto.getRoute().getFromName();
    
    Property options;
    options.fromString(proto.getSenderSpecifier().c_str());
    //timeConstant = fabs(options.check("tc",Value(1.0)).asDouble());
    return true;
}

void LuaModifier::setCarrierParams(const yarp::os::Property& params) 
{
    printf("setCarrierParams\n");
    /*
    yarp::os::Property property = params;
    timeConstant = property.check("tc", Value(timeConstant)).asDouble();
    */    
}

void LuaModifier::getCarrierParams(yarp::os::Property& params) 
{
    printf("getCarrierParams\n");
    /*
    params.put("tc", timeConstant);
    params.put("ex", excitation.toString());
    */
}


yarp::os::ConnectionReader& LuaModifier::modifyIncomingData(yarp::os::ConnectionReader& reader) 
{
    printf("modifiesIncomingData\n");
    /*
    // minimal test of image modification
    in.read(reader);
    out.copy(in);
    out.pixel(0,0).r = 42;
    out.write(con.getWriter());
    return con.getReader();
    */

    
    // minimal test of bottle modification
    con.setTextMode(reader.isTextMode());
    Bottle b;
    b.read(reader);
    b.addInt(42);
    b.addString("(p.s. bork bork bork)");
    b.write(con.getWriter());
    return con.getReader();
    
    //return *local;
}

YARP_SSIZE_T LuaModifier::read(const yarp::os::Bytes& b) 
{
    printf("read\n");

    /*
    // copy across small stuff - the image header
    if (consumed<sizeof(header)) {
        size_t len = b.length();
        if (len>sizeof(header)-consumed) {
            len = sizeof(header)-consumed;
        }
        memcpy(b.get(),((char*)(&header))+consumed,len);
        consumed += len;
        return (YARP_SSIZE_T) len;
    }
    // sane client will want to read image into correct-sized block
    if (b.length()==image_data_len) {
        // life is good!
        processDirect(b);
        consumed += image_data_len;
        return image_data_len;
    }
    // funky client, fall back on image copy
    processBuffered();
    if (consumed<sizeof(header)+out.getRawImageSize()) {
        size_t len = b.length();
        if (len>sizeof(header)+out.getRawImageSize()-consumed) {
            len = sizeof(header)+out.getRawImageSize()-consumed;
        }
        memcpy(b.get(),out.getRawImage()+consumed-sizeof(header),len);
        consumed += len;
        return (YARP_SSIZE_T) len;
    }
    */
    return -1;
}

