/**
 * Main source file for the AD{{ cookiecutter.driver_name }} EPICS driver 
 * 
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/NSLS2/ADDriverTemplate on {% now 'local', '%d/%m/%Y' %}
 * 
 * Author: {{ cookiecutter.author }}
 * 
 * Copyright (c) : {{ cookiecutter.institution }}, {% now 'local', '%Y' %}
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <iocsh.h>
#include <epicsExport.h>

#include "AD{{ cookiecutter.driver_name }}.h"

using namespace std;

const char* driverName = "AD{{ cookiecutter.driver_name }}";

/*
 * External configuration function for AD{{ cookiecutter.driver_name }}.
 * Envokes the constructor to create a new AD{{ cookiecutter.driver_name }} object
 * This is the function that initializes the driver, and is called in the IOC startup script
 *
 * @params[in]: all passed into constructor
 * @return:     status
 */
extern "C" int AD{{ cookiecutter.driver_name }}Config(const char* portName, .....){
    new AD{{ cookiecutter.driver_name }}(portName, ......);
    return(asynSuccess);
}


/*
 * Callback function fired when IOC is terminated.
 * Deletes the driver object and performs cleanup
 *
 * @params[in]: pPvt -> pointer to the driver object created in AD{{ cookiecutter.driver_name }}Config
 * @return:     void
 */
static void exitCallbackC(void* pPvt){
    AD{{ cookiecutter.driver_name }}* p{{ cookiecutter.driver_name }} = (AD{{ cookiecutter.driver_name }}*) pPvt;
    delete(p{{ cookiecutter.driver_name }});
}



// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Acquisition Functions
// -----------------------------------------------------------------------


/**
 * Function that spawns new acquisition thread, if able
 */
void AD{{ cookiecutter.driver_name }}::acquireStart(){
    const char* functionName = "acquireStart";
    
    epicsThreadOpts opts;
    opts.joinable = 

    epicsThreadCreateOpts
}




/**
 * Function responsible for stopping camera image acquisition. First check if the camera is connected.
 * If it is, execute the 'AcquireStop' command. Then set the appropriate PV values, and callParamCallbacks
 * 
 * @return: status  -> error if no camera or command fails to execute, success otherwise
 */ 
asynStatus AD{{ cookiecutter.driver_name }}::acquireStop(){
    const char* functionName = "acquireStop";
    asynStatus status;
    
    this->acquiring = false;
    if(this->acquisitionThreadId != NULL) {
        epicsThreadMustJoin(this->acquisitionThreadId);
        this->acquisitionThreadId = NULL;
    }

    setIntegerParam(ADStatus, ADStatusIdle);
    setIntegerParam(ADAcquire, 0);
    callParamCallbacks();
    
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s Stopping Image Acquisition\n", driverName, functionName);
    return status;
}


void AD{{ cookiecutter.driver_name }}::acquisitionThread(){
    const char* functionName = "acquisitionThread";

    this->acquiring = true;
    while(this->acquiring){
    }
}


//---------------------------------------------------------
// Base {{ cookiecutter.driver_name }} Camera functionality
//---------------------------------------------------------


//  Add functions for getting/setting various camera settings (gain, exposure etc.) here


//-------------------------------------------------------------------------
// ADDriver function overwrites
//-------------------------------------------------------------------------



/*
 * Function overwriting ADDriver base function.
 * Takes in a function (PV) changes, and a value it is changing to, and processes the input
 *
 * @params[in]: pasynUser       -> asyn client who requests a write
 * @params[in]: value           -> int32 value to write
 * @return: asynStatus      -> success if write was successful, else failure
 */
asynStatus AD{{ cookiecutter.driver_name }}::writeInt32(asynUser* pasynUser, epicsInt32 value){
    int function = pasynUser->reason;
    int acquiring;
    int status = asynSuccess;
    static const char* functionName = "writeInt32";
    getIntegerParam(ADAcquire, &acquiring);

    status = setIntegerParam(function, value);
    // start/stop acquisition
    if(function == ADAcquire){
        if(value && !acquiring){
            deviceStatus = acquireStart();
            if(deviceStatus < 0){
                report{{ cookiecutter.driver_name }}Error(deviceStatus, functionName);
                return asynError;
            }
        }
        if(!value && acquiring){
            acquireStop();
        }
    }

    else if(function == ADImageMode)
        if(acquiring == 1) acquireStop();

    else{
        if (function < AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM) {
            status = ADDriver::writeInt32(pasynUser, value);
        }
    }
    callParamCallbacks();

    if(status){
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s ERROR status=%d, function=%d, value=%d\n", driverName, functionName, status, function, value);
        return asynError;
    }
    else asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "%s::%s function=%d value=%d\n", driverName, functionName, function, value);
    return asynSuccess;
}


/*
 * Function overwriting ADDriver base function.
 * Takes in a function (PV) changes, and a value it is changing to, and processes the input
 * This is the same functionality as writeInt32, but for processing doubles.
 *
 * @params[in]: pasynUser       -> asyn client who requests a write
 * @params[in]: value           -> int32 value to write
 * @return: asynStatus      -> success if write was successful, else failure
 */
asynStatus AD{{ cookiecutter.driver_name }}::writeFloat64(asynUser* pasynUser, epicsFloat64 value){
    int function = pasynUser->reason;
    int acquiring;
    int status = asynSuccess;
    static const char* functionName = "writeFloat64";
    getIntegerParam(ADAcquire, &acquiring);

    status = setDoubleParam(function, value);

    if(function == ADAcquireTime){
        if(acquiring) acquireStop();
    }
    else{
        if(function < AD{{ cookiecutter.driver_name }}_FIRST_PARAM){
            status = ADDriver::writeFloat64(pasynUser, value);
        }
    }
    callParamCallbacks();

    if(status){
        asynPrint(this-> pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s ERROR status = %d, function =%d, value = %f\n", driverName, functionName, status, function, value);
        return asynError;
    }
    else asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER, "%s::%s function=%d value=%f\n", driverName, functionName, function, value);
    return asynSuccess;
}



/*
 * Function used for reporting ADUVC device and library information to a external
 * log file. The function first prints all libuvc specific information to the file,
 * then continues on to the base ADDriver 'report' function
 * 
 * @params[in]: fp      -> pointer to log file
 * @params[in]: details -> number of details to write to the file
 * @return: void
 */
void AD{{ cookiecutter.driver_name }}::report(FILE* fp, int details){
    const char* functionName = "report";
    int height;
    int width;
    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s reporting to external log file\n",driverName, functionName);
    if(details > 0){
        if(!connected){
            fprintf(fp, " No connected devices\n");
            ADDriver::report(fp, details);
            return;
        }
        fprintf(fp, " Connected Device Information\n");
        // GET CAMERA INFORMATION HERE AND PRINT IT TO fp
        getIntegerParam(ADSizeX, &width);
        getIntegerParam(ADSizeY, &height);
        fprintf(fp, " Image Width           ->      %d\n", width);
        fprintf(fp, " Image Height          ->      %d\n", height);
        fprintf(fp, " -------------------------------------------------------------------\n");
        fprintf(fp, "\n");
        
        ADDriver::report(fp, details);
    }
}




//----------------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Constructor/Destructor
//----------------------------------------------------------------------------


AD{{ cookiecutter.driver_name }}::AD{{ cookiecutter.driver_name }}(const char* portName, const char* connectionParam, int maxbuffers, size_t maxMemory, int priority, int stackSize )
    : ADDriver(portName, 1, (int)NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS, maxBuffers, maxMemory, asynEnumMask, asynEnumMask, ASYN_CANBLOCK, 1, priority, stackSize){
    static const char* functionName = "AD{{ cookiecutter.driver_name }}";

    // Call createParam here for all of your 
    // ex. createParam(ADUVC_UVCComplianceLevelString, asynParamInt32, &ADUVC_UVCComplianceLevel);

    // Sets driver version PV (version numbers defined in header file) 
    char versionString[25];
    epicsSnprintf(versionString, sizeof(versionString), "%d.%d.%d", AD{{ cookiecutter.driver_name.upper() }}_VERSION, AD{{ cookiecutter.driver_name.upper() }}_REVISION, AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION);
    setStringParam(NDDriverVersion, versionString);

    if(strlen(connectionParam) < 0){
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s Connection failed, abort\n", driverName, functionName);
    }
    else{
        connected = connectToDevice{{ cookiecutter.driver_name }}(serial);
        if(connected){
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s Acquiring device information\n", driverName, functionName);
            getDeviceInformation();
        }
    }

     // when epics is exited, delete the instance of this class
    epicsAtExit(exitCallbackC, this);
}


AD{{ cookiecutter.driver_name }}::~AD{{ cookiecutter.driver_name }}(){
    const char* functionName = "~AD{{ cookiecutter.driver_name }}";
    disconnectFromDevice{{ cookiecutter.driver_name }}();
    asynPrint(this->pasynUserSelf, ASYN_TRACEIO_DRIVER,"%s::%s AD{{ cookiecutter.driver_name }}\n", driverName, functionName);
    disconnect(this->pasynUserSelf);
}


//-------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} ioc shell registration
//-------------------------------------------------------------

/* {{ cookiecutter.driver_name }}Config -> These are the args passed to the constructor in the epics config function */
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg0 = { "Port name",        iocshArgString };

// This parameter must be customized by the driver author. Generally a URL, Serial Number, ID, IP are used to connect.
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg1 = { "Connection Param", iocshArgString };

static const iocshArg {{ cookiecutter.driver_name }}ConfigArg2 = { "maxBuffers",       iocshArgInt };
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg3 = { "maxMemory",        iocshArgInt };
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg4 = { "priority",         iocshArgInt };
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg5 = { "stackSize",        iocshArgInt };


/* Array of config args */
static const iocshArg * const {{ cookiecutter.driver_name }}ConfigArgs[] =
        { &{{ cookiecutter.driver_name }}ConfigArg0, &{{ cookiecutter.driver_name }}ConfigArg1, &{{ cookiecutter.driver_name }}ConfigArg2,
        &{{ cookiecutter.driver_name }}ConfigArg3, &{{ cookiecutter.driver_name }}ConfigArg4, &{{ cookiecutter.driver_name }}ConfigArg5 };


/* what function to call at config */
static void config{{ cookiecutter.driver_name }}CallFunc(const iocshArgBuf *args) {
    AD{{ cookiecutter.driver_name }}Config(args[0].sval, args[1].sval, args[2].ival, args[3].ival, args[4].ival, args[5].ival);
}


/* information about the configuration function */
static const iocshFuncDef configUVC = { "AD{{ cookiecutter.driver_name }}Config", 5, {{ cookiecutter.driver_name }}ConfigArgs };


/* IOC register function */
static void {{ cookiecutter.driver_name }}Register(void) {
    iocshRegister(&config{{ cookiecutter.driver_name }}, config{{ cookiecutter.driver_name }}CallFunc);
}


/* external function for IOC register */
extern "C" {
    epicsExportRegistrar({{ cookiecutter.driver_name }}Register);
}
