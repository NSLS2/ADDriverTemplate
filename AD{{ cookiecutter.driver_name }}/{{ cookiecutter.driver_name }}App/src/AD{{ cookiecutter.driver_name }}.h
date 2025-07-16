/*
 * Header file for the AD{{ cookiecutter.driver_name }} EPICS driver
 *
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/NSLS2/ADDriverTemplate on {% now 'local', '%d/%m/%Y' %}
 *
 * Author: {{ cookiecutter.author }}
 *
 * Copyright (c): {{ cookiecutter.institution }}, {% now 'local', '%Y' %}
 *
 */

// header guard
#ifndef AD{{ cookiecutter.driver_name.upper() }}_H
#define AD{{ cookiecutter.driver_name.upper() }}_H

// version numbers
#define AD{{ cookiecutter.driver_name.upper() }}_VERSION 0
#define AD{{ cookiecutter.driver_name.upper() }}_REVISION 0
#define AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION 0

typedef enum AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL {
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_NONE = 0,
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_ERROR = 10,
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_WARNING = 20,
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_INFO = 30,
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_DEBUG = 40
} AD{{ cookiecutter.driver_name }}_LogLevel_t;

// Error message formatters
#define ERR(msg)                                  \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_ERROR) \
        printf("ERROR | %s::%s: %s\n", driverName, functionName, msg);

#define ERR_ARGS(fmt, ...)                        \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_ERROR) \
        printf("ERROR | %s::%s: " fmt "\n", driverName, functionName, __VA_ARGS__);

// Warning message formatters
#define WARN(msg)                                   \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_WARNING) \
        printf("WARNING | %s::%s: %s\n", driverName, functionName, msg);

#define WARN_ARGS(fmt, ...)                         \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_WARNING) \
        printf("WARNING | %s::%s: " fmt "\n", driverName, functionName, __VA_ARGS__);

// Info message formatters. Because there is no ASYN trace for info messages, we just use `printf`
// here.
#define INFO(msg)                                \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_INFO) \
        printf("INFO | %s::%s: %s\n", driverName, functionName, msg);

#define INFO_ARGS(fmt, ...)                      \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_INFO) \
        printf("INFO | %s::%s: " fmt "\n", driverName, functionName, __VA_ARGS__);

// Debug message formatters
#define DEBUG(msg)                                \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_DEBUG) \
        printf("DEBUG | %s::%s: %s\n", driverName, functionName, msg);

#define DEBUG_ARGS(fmt, ...)                      \
    if (this->logLevel >= AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_DEBUG) \
        printf("DEBUG | %s::%s: " fmt "\n", driverName, functionName, __VA_ARGS__);


// Define macros that correspond to string representations of PVs from EPICS database template here.
#define AD{{ cookiecutter.driver_name.upper() }}_LogLevelString "AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL"

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

    // Add parameter library index variables here.
    int AD{{ cookiecutter.driver_name }}_LogLevel;

   private:

    bool acquisitionActive; // Flag to indicate if acquisition is active
    epicsThreadId acquisitionThreadId;

    void acquireStart();
    void acquireStop();
    void acquisitionThread();

    AD{{ cookiecutter.driver_name }}_LogLevel_t logLevel = AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_INFO;
};

#endif
