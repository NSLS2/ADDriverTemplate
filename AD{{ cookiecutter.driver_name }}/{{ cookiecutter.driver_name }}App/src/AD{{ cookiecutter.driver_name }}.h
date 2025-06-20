/*
 * Header file for the AD{{ cookiecutter.driver_name }} EPICS driver
 * 
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/NSLS2/ADDriverTemplate on {% now 'local', '%d/%m/%Y' %}
 *
 * Author: {{ cookiecutter.author }}
 * 
 * Copyright (c) : {{ cookiecutter.institution }}, {% now 'local', '%Y' %}
 *
 */

// header guard
#ifndef AD{{ cookiecutter.driver_name.upper() }}_H
#define AD{{ cookiecutter.driver_name.upper() }}_H

// version numbers
#define AD{{ cookiecutter.driver_name.upper() }}_VERSION      0
#define AD{{ cookiecutter.driver_name.upper() }}_REVISION     0
#define AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION 0

// Error message formatters
#define ERR(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: %s\n", driverName, functionName, msg)

#define ERR_ARGS(fmt, ...)                                                                    \
    asynPrint(pasynUserSelf, ASYN_TRACE_ERROR, "ERROR | %s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);

// Warning message formatters
#define WARN(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_WARNING, "WARNING | %s::%s: %s\n", driverName, functionName, msg)

#define WARN_ARGS(fmt, ...)                                                                     \
    asynPrint(pasynUserSelf, ASYN_TRACE_WARNING, "WARNING | %s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);

// Info message formatters
#define INFO(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "INFO | %s::%s: %s\n", driverName, functionName, msg)

#define INFO_ARGS(fmt, ...)                                                                      \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "INFO | %s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);

// Debug message formatters
#define DEBUG(msg) \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "DEBUG | %s::%s: %s\n", driverName, functionName, msg)

#define DEBUG_ARGS(fmt, ...)                                                                 \
    asynPrint(pasynUserSelf, ASYN_TRACE_FLOW, "DEBUG | %s::%s: " fmt "\n", driverName, functionName, \
              __VA_ARGS__);


// Define macros that correspond to string representations of PVs from EPICS database template here. For example:
// For a record with `OUT` field set to `@asyn($(PORT), $(ADDR=0), $(TIMEOUT=1))PV_NAME`, add,
// where the record represents an integer value:
// #define AD{{ cookiecutter.driver_name }}_PVNameString          "PV_NAME"            //asynInt32



#include "ADDriver.h"

/*
 * Class definition of the AD{{ cookiecutter.driver_name }} driver
 */
class AD{{ cookiecutter.driver_name }} : ADDriver{

    public:

        // Constructor for the AD{{ cookiecutter.driver_name }} driver
        AD{{ cookiecutter.driver_name }}(const char* portName, ......);


        // ADDriver overrides
        virtual asynStatus writeInt32(asynUser* pasynUser, epicsInt32 value);
        virtual asynStatus writeFloat64(asynUser* pasynUser, epicsFloat64 value);


        // Destructor. Disconnects from the detector and performs cleanup
        ~AD{{ cookiecutter.driver_name }}();

    protected:

        // Add PV indexes here. You must also define the first/last index as you add them.
        // Ex: int AD{{ cookiecutter.driver_name }}_PVName;
        #define AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM ......
        #define AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM ......

    private:

        asynStatus acquireStart();
        void acquireStop();

};

// Stores number of additional PV parameters are added by the driver
#define NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS ((int)(&AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM - &AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM + 1))

#endif
