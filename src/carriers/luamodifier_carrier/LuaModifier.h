// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2012 IITRBCS
 * Authors: Ali Paikan
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef LUAMODIFIERCARRIER_INC
#define LUAMODIFIERCARRIER_INC

#include <math.h>
#include <yarp/os/ModifyingCarrier.h>
#include <yarp/os/Election.h>
#include <yarp/os/NullConnectionReader.h>
#include <yarp/os/Semaphore.h>
#include <yarp/os/Time.h>
#include <yarp/os/RateThread.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/ConnectionReader.h>
#include <yarp/os/DummyConnector.h>


namespace yarp {
    namespace os {
        class LuaModifier;
    }
}


/**
 *
 * Allow priority-based message selection.  Under development.
 * Affected by carrier modifiers.  Examples:
 *   tcp+recv.luamodifier
 *
 */
class yarp::os::LuaModifier : public yarp::os::ModifyingCarrier,
                               public yarp::os::ConnectionReader,
                               public yarp::os::InputStream {

public:
    LuaModifier(){
        printf("LuaModifier\n");
        local = yarp::os::ConnectionReader::createConnectionReader(*this);
        happy = (local!=0);
    }

    virtual ~LuaModifier() {
        if (local) delete local;
    }

    virtual Carrier *create() {
        return new LuaModifier();
    }

    virtual ConstString getName() {
        return "luamodifier";
    }

    virtual ConstString toString() {
        return "luamodifier_carrier";
    }

    virtual bool configure(yarp::os::ConnectionState& proto);
    
    //virtual bool modifiesIncomingData();
    virtual yarp::os::ConnectionReader& modifyIncomingData(yarp::os::ConnectionReader& reader);

    virtual void setCarrierParams(const yarp::os::Property& params);

    virtual void getCarrierParams(yarp::os::Property& params);


    void setParent(yarp::os::ConnectionReader& reader) {
        parent = &reader;
    }

    virtual bool expectBlock(const char *data, size_t len) {
        return local->expectBlock(data,len);
    }

    virtual ConstString expectText(int terminatingChar = '\n') {
        return local->expectText(terminatingChar);
    }

    virtual int expectInt() {
        return local->expectInt();
    }

    virtual bool pushInt(int x) {
        return local->pushInt(x);
    }

    virtual double expectDouble() {
        return local->expectDouble();
    }

    virtual bool isTextMode() {
        return false;
    }

    virtual bool convertTextMode() {
        return true;
    }

    virtual size_t getSize() {
        /*
        if (image_data_len) processBuffered();
        return sizeof(header)+image_data_len;
        */
        return 0;
    }

    virtual ConnectionWriter *getWriter() {
        return parent->getWriter();
    }

    virtual Bytes readEnvelope() { 
        return parent->readEnvelope();
    }

    virtual Portable *getReference() {
        return parent->getReference();
    }

    virtual Contact getRemoteContact() {
        return parent->getRemoteContact();
    }

    virtual Contact getLocalContact() {
        return parent->getLocalContact();
    }

    virtual bool isValid() {
        return true;
    }

    virtual bool isActive() {
        return parent->isActive();
    }

    virtual bool isError() {
        return parent->isError()||!happy;
    }

    virtual void requestDrop() {
        parent->requestDrop();
    }

    virtual yarp::os::Searchable& getConnectionModifiers() {
        return parent->getConnectionModifiers();
    }

    virtual bool setSize(size_t len) {
        return parent->setSize(len);
    }

    ////////////////////////////////////////////////////////////////////////
    // InputStream methods

    virtual YARP_SSIZE_T read(const yarp::os::Bytes& b);

    virtual void close() {
    }

    virtual bool isOk() {
        return happy;
    }

private:
    bool happy;
    ConstString portName;
    ConstString sourceName;
    yarp::os::ConnectionReader *local;
    yarp::os::ConnectionReader *parent;
    yarp::os::DummyConnector con;

};

#endif


