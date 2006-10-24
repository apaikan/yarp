// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef __YARP2_DRIVERS__
#define __YARP2_DRIVERS__

#include <yarp/os/Property.h>
#include <yarp/dev/DeviceDriver.h>

namespace yarp {
    namespace dev {
        template <class T> class DriverCreatorOf;
        class DriverCreator;
        class Drivers;
    }
}


/**
 * A base class for factories that create driver objects.
 * The DriverCreatorOf class is probably what you want.
 */
class yarp::dev::DriverCreator {
public:
    /**
     * Destructor.
     */
    virtual ~DriverCreator() {}

    /**
     * Returns a simple description of devices the factory can make.
     * @return a description of what this factory makes
     */
    virtual yarp::os::ConstString toString() = 0;

    /**
     * Create a device.
     */
    virtual DeviceDriver *create() = 0;
    
    /**
     * Get the common name of the device.
     * @return the common name of the device this creates.
     */
    virtual yarp::os::ConstString getName() = 0;
    
    /**
     * Get the common name of a device that can wrap this one.
     *
     * @return the common name of the device that can wrap this one
     * for the network.
     */
    virtual yarp::os::ConstString getWrapper() = 0;

    /**
     * Get the name of the C++ class associated with this device.
     *
     * @return the name of the C++ class associated with this device.
     */
    virtual yarp::os::ConstString getCode() = 0;
};


/**
 * A factory for creating driver objects of a particular type.
 */
template <class T>
class yarp::dev::DriverCreatorOf : public DriverCreator {
private:
    yarp::os::ConstString desc, wrap, code;
public:
    /**
     * Constructor.  Sets up the name by which the device will be known.
     * @param name The "common name" of the device.  This is the name 
     * that will be used when naming the device externally, for example
     * from the command line.
     * @param wrap The "common name" of another device which can wrap
     * this device up for network access.  If there is no such device,
     * use an empty name: ""
     * @param code The name of a class associated with this device.
     * This is to give the user an entry point into the relevant
     * code documentation.
     *
     */
    DriverCreatorOf(const char *name, const char *wrap, const char *code) : 
        desc(name), wrap(wrap), code(code)
    {
    }

    virtual yarp::os::ConstString toString() {
        return desc;
    }

    virtual yarp::os::ConstString getName() {
        return desc;
    }

    virtual yarp::os::ConstString getWrapper() {
        return wrap;
    }

    virtual yarp::os::ConstString getCode() {
        return code;
    }

    virtual DeviceDriver *create() {
        return new T;
    }
};



/**
 * \ingroup dev_class
 *
 * Global factory for devices.  You can create your devices any way
 * you like, but if you register them with the Driver object returned
 * by Drivers::factory() by calling the add() method on it, then
 * those devices will be creatable by name through the open() methods.
 *
 * There is only one instance of the Drivers class.  Call 
 * Drivers::factory() to get that instance.
 */
class yarp::dev::Drivers {
public:
    /**
     * Get the global factory for devices.
     * @return the global factory for devices.
     */
    static Drivers& factory() {
        return instance;
    }

    /**
     * Create and configure a device, by name.  If you need
     * to pass configuration options to the device (usually the
     * case) use the other open method that takes a Searchable.
     *
     * @param device the common name of the device.
     *
     * @return the device, if it could be created and configured,
     * otherwise NULL. The user is responsible for deallocating the
     * device.
     */
    DeviceDriver *open(const char *device) {
        yarp::os::Property p;
        p.put("device",device);
        return open(p);
    }

    /**
     * Create and configure a device, by name.  The config
     * object should have a property called "device" that
     * is set to the common name of the device.  All other
     * properties are passed on the the device's
     * DeviceDriver::open method.
     *
     * @param config configuration options for the device
     *
     * @return  the device, if it could be created and configured,
     * otherwise NULL. The user is responsible for deallocating the
     * device.
     */
    virtual DeviceDriver *open(yarp::os::Searchable& config);

    /**
     * A description of the available devices.
     * @return a description of the available devices.
     */
    virtual yarp::os::ConstString toString();

    /**
     * Destructor.
     */
    virtual ~Drivers();

    /**
     * Add a factory for creating a particular device.
     * The library will be responsible for deallocating
     * this factor.
     * @param creator A factory for creating a particular device.
     */
    void add(DriverCreator *creator);

private:

    DriverCreator *find(const char *name);

    void init();

    void *implementation;

    Drivers();
    
    static Drivers instance;
};

#endif
