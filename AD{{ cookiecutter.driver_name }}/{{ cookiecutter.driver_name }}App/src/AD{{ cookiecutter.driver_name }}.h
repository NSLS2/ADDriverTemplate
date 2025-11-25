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

typedef enum AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL {
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_NONE = 0,      // No logging
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_ERROR = 10,    // Error messages only
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_WARNING = 20,  // Warnings and errors
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_INFO = 30,     // Info, warnings, and errors
    AD{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL_DEBUG = 40     // Debugging information
} AD{{ cookiecutter.driver_name }}_LogLevel_t;


// Error message formatters
#define ERR(msg)                                      \
    if (this->getLogLevel() >= ADXSPDLogLevel::ERROR) \
        fprintf(stderr, "ERROR | %s::%s: %s\n", driverName, __func__, msg);

#define ERR_ARGS(fmt, ...)                            \
    if (this->getLogLevel() >= ADXSPDLogLevel::ERROR) \
        fprintf(stderr, "ERROR | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

#define ERR_TO_STATUS(fmt, ...)                     \
    if (this->getLogLevel() >= ADXSPDLogLevel::ERROR) { \
        char errMsg[256];                              \
        snprintf(errMsg, sizeof(errMsg), fmt, __VA_ARGS__); \
        printf("ERROR | %s::%s: %s\n", driverName, __func__, errMsg); \
        setStringParam(ADStatusMessage, errMsg);          \
        setIntegerParam(ADStatus, ADStatusError);         \
        callParamCallbacks();                             \
    }

// Warning message formatters
#define WARN(msg)                                       \
    if (this->getLogLevel() >= ADXSPDLogLevel::WARNING) \
        fprintf(stderr, "WARNING | %s::%s: %s\n", driverName, __func__, msg);

#define WARN_ARGS(fmt, ...)                             \
    if (this->getLogLevel() >= ADXSPDLogLevel::WARNING) \
        fprintf(stderr, "WARNING | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

#define WARN_TO_STATUS(fmt, ...)                      \
    if (this->getLogLevel() >= ADXSPDLogLevel::WARNING) { \
        char warnMsg[256];                               \
        snprintf(warnMsg, sizeof(warnMsg), fmt, __VA_ARGS__); \
        printf("WARNING | %s::%s: %s\n", driverName, __func__, warnMsg); \
        setStringParam(ADStatusMessage, warnMsg);          \
        callParamCallbacks();                             \
    }

// Info message formatters
#define INFO(msg)                                    \
    if (this->getLogLevel() >= ADXSPDLogLevel::INFO) \
        fprintf(stdout, "INFO | %s::%s: %s\n", driverName, __func__, msg);

#define INFO_ARGS(fmt, ...)                          \
    if (this->getLogLevel() >= ADXSPDLogLevel::INFO) \
        fprintf(stdout, "INFO | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

#define INFO_TO_STATUS(fmt, ...)                      \
    if (this->getLogLevel() >= ADXSPDLogLevel::INFO) { \
        char infoMsg[256];                               \
        snprintf(infoMsg, sizeof(infoMsg), fmt, __VA_ARGS__); \
        printf("INFO | %s::%s: %s\n", driverName, __func__, infoMsg); \
        setStringParam(ADStatusMessage, infoMsg);          \
        callParamCallbacks();                             \
    }

// Debug message formatters
#define DEBUG(msg)                                    \
    if (this->getLogLevel() >= ADXSPDLogLevel::DEBUG) \
        fprintf(stdout, "DEBUG | %s::%s: %s\n", driverName, __func__, msg);

#define DEBUG_ARGS(fmt, ...)                          \
    if (this->getLogLevel() >= ADXSPDLogLevel::DEBUG) \
        fprintf(stdout, "DEBUG | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

#include "ADDriver.h"

/*
 * Class definition of the AD{{ cookiecutter.driver_name }} driver
 */
class AD{{ cookiecutter.driver_name }} : ADDriver{

    public:

        // Constructor for the AD{{ cookiecutter.driver_name }} driver
        AD{{ cookiecutter.driver_name }}(const char* portName, const char* connectionParam);

        // ADDriver overrides
        virtual asynStatus writeInt32(asynUser* pasynUser, epicsInt32 value);
        virtual asynStatus writeFloat64(asynUser* pasynUser, epicsFloat64 value);

        // Destructor. Disconnects from the detector and performs cleanup
        ~AD{{ cookiecutter.driver_name }}();

    protected:

// Parameter definitions auto-generated from *.template files, by `make paramdefs`
#include "AD{{ cookiecutter.driver_name }}ParamsDefs.h"

    private:

        bool acquisitionActive; // Flag to indicate if acquisition is active
        epicsThreadId acquisitionThreadId;
        AD{{ cookiecutter.driver_name }}_LogLevel_t logLevel = AD{{ cookiecutter.driver_name }}_LOG_LEVEL_INFO; // Current logging level
        AD{{ cookiecutter.driver_name }}_LogLevel_t getLogLevel() { return this->logLevel; };

        void acquireStart();
        void acquireStop();
        void acquisitionThread();

};

// Stores number of additional PV parameters are added by the driver

#endif
`