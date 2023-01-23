/*
 * Header file for the AD{{ cookiecutter.driver_name }} EPICS driver
 *
 * This file contains the definitions of PV params, and the definition of the AD{{ cookiecutter.driver_name }} class and functions.
 * 
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/jwlodek/ADDriverTemplate on {% now 'local', '%d/%m/%Y' %}
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
#define AD{{ cookiecutter.driver_name }}_VERSION      0
#define AD{{ cookiecutter.driver_name }}_REVISION     0
#define AD{{ cookiecutter.driver_name }}_MODIFICATION 0


// Place PV string definitions here
// #define AD{{ cookiecutter.driver_name.upper() }}SHORT_PVNameString          "PV_NAME"            //asynInt32


// Place any required inclues here

#include "ADDriver.h"


// ----------------------------------------
// {{ cookiecutter.driver_name }} Data Structures
//-----------------------------------------

// Place any in use Data structures here



/*
 * Class definition of the AD{{ cookiecutter.driver_name }} driver. It inherits from the base ADDriver class
 *
 * Includes constructor/destructor, PV params, function defs and variable defs
 *
 */
class AD{{ cookiecutter.driver_name }} : ADDriver{

    public:

        // Constructor - NOTE THERE IS A CHANCE THAT YOUR CAMERA DOESNT CONNECT WITH SERIAL # AND THIS MUST BE CHANGED
        AD{{ cookiecutter.driver_name }}(const char* portName, const char* connectionParam, int maxBuffers, size_t maxMemory, int priority, int stackSize);


        // ADDriver overrides
        virtual asynStatus writeInt32(asynUser* pasynUser, epicsInt32 value);
        virtual asynStatus writeFloat64(asynUser* pasynUser, epicsFloat64 value);


        // destructor. Disconnects from camera, deletes the object
        ~AD{{ cookiecutter.driver_name }}();

    protected:

        // Add PV indexes here. You must also define the first/last index as you add them.
        // Ex: int ADUVC_UVCCompliance;
        #define AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM FIRST_PV_INDEX
        #define AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM LAST_PV_INDEX

    private:

        // Some data variables
        epicsEventId startEventId;
        epicsEventId endEventId;
        

        // ----------------------------------------
        // {{ cookiecutter.driver_name }} Global Variables
        //-----------------------------------------


        // ----------------------------------------
        // {{ cookiecutter.driver_name }} Functions - Logging/Reporting
        //-----------------------------------------

        //function used to report errors in {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }} operations
        // Note that vendor libraries usually have a status data structure, if not
        // it might be wise to make one
        void report{{ cookiecutter.driver_name }}Error(______ status, const char* functionName);

        // reports device and driver info into a log file
        void report(FILE* fp, int details);

        // writes to ADStatus PV
        void updateStatus(const char* status);

        // ----------------------------------------
        // UVC Functions - Connecting to camera
        //-----------------------------------------

        //function used for connecting to a {{ cookiecutter.driver_name }} device
        // NOTE - THIS MAY ALSO NEED TO CHANGE IF SERIAL # NOT USED
        asynStatus connectToDevice{{ cookiecutter.driver_name }}(const char* connectionParam);

        //function used to disconnect from {{ cookiecutter.driver_name }} device
        asynStatus disconnectFromDevice{{ cookiecutter.driver_name }}();

        // ----------------------------------------
        // {{ cookiecutter.driver_name }} Functions - Camera functions
        //-----------------------------------------


        //function that begins image aquisition
        asynStatus acquireStart();

        //function that stops aquisition
        void acquireStop();

};

// Stores number of additional PV parameters are added by the driver
#define NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS ((int)(&AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM - &AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM + 1))

#endif
