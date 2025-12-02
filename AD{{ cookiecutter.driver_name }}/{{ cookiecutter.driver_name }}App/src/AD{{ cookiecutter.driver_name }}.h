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

{% if cookiecutter.with_cpr == true %}
#include <cpr/cpr.h>
{% endif %}
{% if cookiecutter.with_zmq == true %}
#include <zmq.h>
{% endif %}
{% if cookiecutter.with_json %}
#include <nlohmann/json.hpp>
{% endif %}
{% if cookiecutter.with_magic_enum %}
#include <magic_enum/magic_enum.hpp>
{% endif %}

#include <epicsExit.h>
#include <epicsExport.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <epicsTime.h>
#include <iocsh.h>
#include <string>
#include "ADDriver.h"


// version numbers
#define AD{{ cookiecutter.driver_name.upper() }}_VERSION 0
#define AD{{ cookiecutter.driver_name.upper() }}_REVISION 0
#define AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION 0

#define driverName "AD{{ cookiecutter.driver_name }}"


enum class AD{{ cookiecutter.driver_name }}LogLevel {
    NONE = 0,      // No logging
    ERROR = 10,    // Error messages only
    WARNING = 20,  // Warnings and errors
    INFO = 30,     // Info, warnings, and errors
    DEBUG = 40     // Debugging information
};


// Error message formatters
#define ERR(msg)                                      \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::ERROR) \
        fprintf(stderr, "ERROR | %s::%s: %s\n", driverName, __func__, msg);

#define ERR_ARGS(fmt, ...)                            \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::ERROR) \
        fprintf(stderr, "ERROR | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

// Warning message formatters
#define WARN(msg)                                       \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::WARNING) \
        fprintf(stderr, "WARNING | %s::%s: %s\n", driverName, __func__, msg);

#define WARN_ARGS(fmt, ...)                             \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::WARNING) \
        fprintf(stderr, "WARNING | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

// Info message formatters
#define INFO(msg)                                    \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::INFO) \
        fprintf(stdout, "INFO | %s::%s: %s\n", driverName, __func__, msg);

#define INFO_ARGS(fmt, ...)                          \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::INFO) \
        fprintf(stdout, "INFO | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

// Debug message formatters
#define DEBUG(msg)                                    \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::DEBUG) \
        fprintf(stdout, "DEBUG | %s::%s: %s\n", driverName, __func__, msg);

#define DEBUG_ARGS(fmt, ...)                          \
    if (this->logLevel >= AD{{ cookiecutter.driver_name }}LogLevel::DEBUG) \
        fprintf(stdout, "DEBUG | %s::%s: " fmt "\n", driverName, __func__, __VA_ARGS__);

{% if cookiecutter.with_json %}
using json = nlohmann::json;
{% endif %}

using namespace std;

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

    // Must be public to be used as thread entry point
    void acquisitionThread();

    // Destructor. Disconnects from the detector and performs cleanup
    ~AD{{ cookiecutter.driver_name }}();

   protected:

// Parameter definitions auto-generated from *.template files, by `make paramdefs`
#include "AD{{ cookiecutter.driver_name }}ParamDefs.h"

    private:

        bool acquisitionActive; // Flag to indicate if acquisition is active
        epicsThreadId acquisitionThreadId;
        AD{{ cookiecutter.driver_name }}LogLevel logLevel = AD{{ cookiecutter.driver_name }}LogLevel::INFO; // Current logging level

        void acquireStart();
        void acquireStop();
        void createAllParams();

};

#endif
