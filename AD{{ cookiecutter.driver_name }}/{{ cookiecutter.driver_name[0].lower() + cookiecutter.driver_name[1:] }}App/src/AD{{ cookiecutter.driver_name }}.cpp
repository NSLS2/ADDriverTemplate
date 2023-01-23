/**
 * Main source file for the AD{{ cookiecutter.driver_name }} EPICS driver 
 * 
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/jwlodek/ADDriverTemplate on {% now 'local', '%d/%m/%Y' %}
 *
 * This file contains functions for connecting and disconnectiong from the camera,
 * for starting and stopping image acquisition, and for controlling all camera functions through
 * EPICS.
 * 
 * Author: {{ cookiecutter.author }}
 * 
 * Copyright (c) : {{ cookiecutter.institution }}, {% now 'utc', '%Y' %}
 * 
 */

// Standard includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// EPICS includes
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <iocsh.h>
#include <epicsExport.h>


// Area Detector include
#include "AD{{ cookiecutter.driver_name }}.h"


using namespace std;

// Add any additional namespaces here


const char* driverName = "AD{{ cookiecutter.driver_name }}";

// Add any driver constants here


// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Utility Functions (Reporting/Logging/ExternalC)
// -----------------------------------------------------------------------


/*
 * External configuration function for AD{{ cookiecutter.driver_name }}.
 * Envokes the constructor to create a new AD{{ cookiecutter.driver_name }} object
 * This is the function that initializes the driver, and is called in the IOC startup script
 * 
 * NOTE: When implementing a new driver with ADDriverTemplate, your camera may use a different connection method than
 * a const char* connectionParam. Just edit the param to fit your device, and make sure to make the same edit to the constructor below
 *
 * @params[in]: all passed into constructor
 * @return:     status
 */
extern "C" int AD{{ cookiecutter.driver_name }}Config(const char* portName, const char* connectionParam, int maxBuffers, size_t maxMemory, int priority, int stackSize){
    new AD{{ cookiecutter.driver_name }}(portName, connectionParam, maxBuffers, maxMemory, priority, stackSize);
    return(asynSuccess);
}


/*
 * Callback function called when IOC is terminated.
 * Deletes created object
 *
 * @params[in]: pPvt -> pointer to the ADDRIVERNAMESTANDATD object created in AD{{ cookiecutter.driver_name }}Config
 * @return:     void
 */
static void exitCallbackC(void* pPvt){
    AD{{ cookiecutter.driver_name }}* p{{ cookiecutter.driver_name }} = (AD{{ cookiecutter.driver_name }}*) pPvt;
    delete(p{{ cookiecutter.driver_name }});
}


/**
 * Simple function that prints all information about a connected camera
 * 
 * @return: void
 */
void AD{{ cookiecutter.driver_name }}::printConnectedDeviceInfo(){
    printf("--------------------------------------\n");
    printf("Connected to {{ cookiecutter.driver_name }} device\n");
    printf("--------------------------------------\n");
    // Add any information you wish to print about the device here
    printf("--------------------------------------\n");
}


// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Connect/Disconnect Functions
// -----------------------------------------------------------------------


/**
 * Function that is used to initialize and connect to the device.
 * 
 * NOTE: Again, it is possible that for your camera, a different connection type is used (such as a product ID [int])
 * Make sure you use the same connection type as passed in the AD{{ cookiecutter.driver_name }}Config function and in the constructor.
 * 
 * @params[in]: serialNumber    -> serial number of camera to connect to. Passed through IOC shell
 * @return:     status          -> success if connected, error if not connected
 */
asynStatus AD{{ cookiecutter.driver_name }}::connectToDevice{{ cookiecutter.driver_name }}(const char* serialNumber){
    const char* functionName = "connectToDevice{{ cookiecutter.driver_name }}";
    bool connected = false;


    // Implement connecting to the camera here
    // Usually the vendor provides examples of how to do this with the library/SDK


    if(connected) return asynSuccess;
    else{
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s Error: failed to connect to device\n", driverName, functionName);
        return asynError;
    }
}


/**
 * Function that disconnects from any connected device
 * First checks if is connected, then if it is, it frees the memory
 * for the info and the camera
 * 
 * @return: status  -> success if freed, error if never connected
 */
asynStatus AD{{ cookiecutter.driver_name }}::disconnectFromDevice{{ cookiecutter.driver_name }}(){
    const char* functionName = "disconnectFromDevice{{ cookiecutter.driver_name }}";

    // Free up any data allocated by driver here, and call the vendor libary to disconnect

    return asynSuccess;
}


/**
 * Function that updates PV values with camera information
 * 
 * @return: status
 */
asynStatus AD{{ cookiecutter.driver_name }}::getDeviceInformation(){
    const char* functionName = "collectCameraInformation";
    asynStatus status = asynSuccess;
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s Collecting camera information\n", driverName, functionName);

    // Use the vendor library to collect information about the connected camera here, and set the appropriate PVs
    // Make sure you check if camera is connected before calling on it for information

    //setStringParam(ADManufacturer,        _____________);
    //setStringParam(ADSerialNumber,        _____________);
    //setStringParam(ADFirmwareVersion,     _____________);
    //setStringParam(ADModel,               _____________);

    return status;
}



// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Acquisition Functions
// -----------------------------------------------------------------------


/**
 * Function responsible for starting camera image acqusition. First, check if there is a
 * camera connected. Then, set camera values by reading from PVs. Then, we execute the 
 * Acquire Start command. if this command was successful, image acquisition started.
 * 
 * @return: status  -> error if no device, camera values not set, or execute command fails. Otherwise, success
 */
asynStatus AD{{ cookiecutter.driver_name }}::acquireStart(){
    const char* functionName = "acquireStart";
    asynStatus status;
    if(this->pcamera == NULL){
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s Error: No camera connected\n", driverName, functionName);
        status = asynError;
    }
    else{

        // Here, you need to start acquisition. Generally there are two setups for drivers. Either the vendor provides
        // a function that starts acquisition and takes a callback function to process frames on a different thread,
        // or you will need to create your own acquisition thread here. 

        if(status != asynSuccess){
            setIntegerParam(ADAcquire, 0);
            setIntegerParam(ADStatus, ADStatusIdle);
            callParamCallbacks();
            status = asynError;
        }
        else{
            setIntegerParam(ADStatus, ADStatusAcquire);
            asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s Image acquistion start\n", driverName, functionName);
            callParamCallbacks();
        }

    }
    return status;
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
    if(this->pcamera == NULL){
        asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, "%s::%s Error: No camera connected\n", driverName, functionName);
        status = asynError;
    }
    else{
        // Here, you need to stop acquisition. If the vendor software spawned its own callback thread, you likely
        // just need to call some stop acquisition function. If you created your own callback thread, you should join
        // it here.
    }
    setIntegerParam(ADStatus, ADStatusIdle);
    setIntegerParam(ADAcquire, 0);
    callParamCallbacks();
    asynPrint(this->pasynUserSelf, ASYN_TRACE_FLOW, "%s::%s Stopping Image Acquisition\n", driverName, functionName);
    return status;
}

// Here, you will need three functions:
// * A function for converting the images supplied by vendor software into areaDetector NDArrays
// * A function that converts the data type and color mode of the vendor software supplied image structure into areaDetector NDDataType_t and NDColorMode_t
// * A function for that takes a pointer to a vendor software image data structure (your callback function - runs once per image)
// 
// If the vendor software is expecting a static function pointer as the callback parameter, you can create a static function with a void pointer as an argument
// cast that void pointer to type AD{{ cookiecutter.driver_name }}*, and call the callback function










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
