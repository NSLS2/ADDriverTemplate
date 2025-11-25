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
extern "C" int AD{{ cookiecutter.driver_name }}Config(const char* portName, const char* connectionParam){
    new AD{{ cookiecutter.driver_name }}(portName, connectionParam);
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
    delete p{{ cookiecutter.driver_name }};
}



/**
 * @brief Wrapper C function passed to epicsThreadCreate to create acquisition thread
 *
 * @param drvPvt Pointer to instance of AD{{ cookiecutter.driver_name }} driver object
 */
static void acquisitionThreadC(void *drvPvt) {
    AD{{ cookiecutter.driver_name }} *pPvt = (AD{{ cookiecutter.driver_name }}*) drvPvt;
    pPvt->acquisitionThread();
}


// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Acquisition Functions
// -----------------------------------------------------------------------


/**
 * Function that spawns new acquisition thread, if able
 */
/**
 * @brief Starts acquisition by spawning main acq thread
 */
void AD{{ cookiecutter.driver_name }}::acquireStart() {
    const char *functionName = "acquireStart";

    // Spawn the acquisition thread. Make sure it's joinable.
    INFO("Spawning main acquisition thread...");

    epicsThreadOpts opts;
    opts.priority = epicsThreadPriorityHigh;
    opts.stackSize = epicsThreadGetStackSize(epicsThreadStackBig);
    opts.joinable = 1;

    this->acquisitionThreadId =
        epicsThreadCreateOpt("acquisitionThread", (EPICSTHREADFUNC)acquisitionThreadC, this, &opts);
}


/**
 * @brief Main acquisition function for ADKinetix
 */
void AD{{ cookiecutter.driver_name }}::acquisitionThread() {
    const char *functionName = "acquisitionThread";

    NDDataType_t dataType;
    NDColorMode_t colorMode;
    getIntegerParam(NDColorMode, (int *)&colorMode);
    getIntegerParam(NDDataType, (int *)&dataType);

    int ndims = 3; // For color
    if (colorMode == NDColorMono)
        ndims = 2; // For monochrome

    int dims[ndims];
    if(ndims == 2){
        getIntegerParam(ADSizeX, &dims[0]);
        getIntegerParam(ADSizeY, &dims[1]);
    } else {
        dims[0] = 3;
        getIntegerParam(ADSizeX, &dims[1]);
        getIntegerParam(ADSizeY, &dims[2]);
    }

    int collectedImages = 0;

    // Start the acquisition given resolution, data type, color mode here
    this->acquisitionActive = true;

    while (acquisitionActive) {
        setIntegerParam(ADStatus, ADStatusAcquire);

        // Get a new frame using the vendor SDK here here

        // Allocate the NDArray of the correct size
        this->pArrays[0] = pNDArrayPool->alloc(ndims, dims, dataType, 0, NULL);
        if (this->pArrays[0] != NULL) {
            pArray = this->pArrays[0];
        } else {
            this->pArrays[0]->release();
            ERR("Failed to allocate array!");
            setIntegerParam(ADStatus, ADStatusError);
            callParamCallbacks();
            break;
        }

        collectedImages += 1;
        setIntegerParam(ADNumImagesCounter, collectedImages);
        updateTimeStamp(&pArray->epicsTS);

        // Set array size PVs based on collected frame
        pArray->getInfo(&arrayInfo);
        setIntegerParam(NDArraySize, (int)arrayInfo.totalBytes);
        setIntegerParam(NDArraySizeX, arrayInfo.xSize);
        setIntegerParam(NDArraySizeY, arrayInfo.ySize);

        // Copy data from new frame to pArray
        // memcpy(pArray->pData, POINTER_TO_FRAME_DATA, arrayInfo.totalBytes);

        // increment the array counter
        int arrayCounter;
        getIntegerParam(NDArrayCounter, &arrayCounter);
        arrayCounter++;
        setIntegerParam(NDArrayCounter, arrayCounter);

        // set the image unique ID to the number in the sequence
        pArray->uniqueId = arrayCounter;
        pArray->pAttributeList->add("ColorMode", "Color Mode", NDAttrInt32, &colorMode);

        // Add any additional attributes here using pArray->pAttributeList->add(...);

        getAttributes(pArray->pAttributeList);
        doCallbacksGenericPointer(pArray, NDArrayData, 0);

        // If in single mode, finish acq, if in multiple mode and reached target number
        // complete acquisition.
        if (acquisitionMode == ADImageSingle) {
            this->acquisitionActive = false;
        } else if (acquisitionMode == ADImageMultiple && collectedImages == targetNumImages) {
            this->acquisitionActive = false;
        }
        // Release the array
        pArray->release();

        // refresh all PVs
        callParamCallbacks();
    }

    setIntegerParam(ADStatus, ADStatusIdle);
    setIntegerParam(ADAcquire, 0);
    callParamCallbacks();
}


/**
 * @brief stops acquisition by aborting exposure and joining acquisition thread
 */
void AD{{ cookiecutter.driver_name }}::acquireStop() {
    const char *functionName = "acquireStop";

    if (this->acquisitionActive) {

        // Mark acquisition as inactive
        this->acquisitionActive = false;

        // Wait for acquisition thread to join
        INFO("Waiting for acquisition thread to join...");
        epicsThreadMustJoin(this->acquisitionThreadId);
        INFO("Acquisition stopped.");

        // Refresh all PV values
        callParamCallbacks();
    } else {
        WARN_TO_STATUS("Acquisition is not active!");
    }
}


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

    // start/stop acquisition
    if(function == ADAcquire){
        if(value && !acquiring){
            acquireStart();
        }
        if(!value && acquiring){
            acquireStop();
        }
    } else if(function == ADImageMode) {
        if(acquiring == 1) acquireStop();
    } else {
        if (function < AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM) {
            status = ADDriver::writeInt32(pasynUser, value);
        }
    }
    callParamCallbacks();

    if(status){
        ERR_ARGS("status=%d, function=%d, value=%d\n", status, function, value);
        return asynError;
    }
    else DEBUG_ARGS("function=%d value=%d\n", function, value);
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
    asynStatus status = asynSuccess;
    static const char* functionName = "writeFloat64";
    getIntegerParam(ADAcquire, &acquiring);

    if(function == ADAcquireTime){
        if(acquiring) acquireStop();
    } else{
        if(function < AD{{ cookiecutter.driver_name }}_FIRST_PARAM){
            status = ADDriver::writeFloat64(pasynUser, value);
        }
    }
    callParamCallbacks();

    if(status){
        ERR_ARGS("status=%d, function=%d, value=%f\n", status, function, value);
        return asynError;
    }
    else DEBUG_ARGS("function=%d value=%f\n", function, value);
    return status;
}


//----------------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Constructor/Destructor
//----------------------------------------------------------------------------


AD{{ cookiecutter.driver_name }}::AD{{ cookiecutter.driver_name }}(const char* portName, .....)
    : ADDriver(portName, 1, (int)NUM_{{ cookiecutter.driver_name.upper() }}_PARAMS, 0, 0, 0, 0, 0, 1, 0, 0){
    static const char* functionName = "AD{{ cookiecutter.driver_name }}";

    this->createAllParams();

    // Sets driver version PV (version numbers defined in header file) 
    char versionString[25];
    epicsSnprintf(versionString, sizeof(versionString), "%d.%d.%d", AD{{ cookiecutter.driver_name.upper() }}_VERSION, AD{{ cookiecutter.driver_name.upper() }}_REVISION, AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION);
    setStringParam(NDDriverVersion, versionString);

    // Initialize vendor SDK and connect to the device here

    // Retrieve device information and populate all PVs.
    // setStringParam(ADManufacturer, ....);
    // setStringParam(ADModel, ....);
    // setStringParam(ADSerialNumber, ....);
    // setStringParam(ADFirmwareVersion, ....);
    // setStringParam(ADSDKVersion, ....);
    // setIntegerParam(ADMaxSizeX, ....);
    // setIntegerParam(ADMaxSizeY, ....);

    // setIntegerParam(ADMinX, 0);
    // setIntegerParam(ADMinY, 0);


    callParamCallbacks();

    // when epics is exited, delete the instance of this class
    epicsAtExit(exitCallbackC, this);
}


AD{{ cookiecutter.driver_name }}::~AD{{ cookiecutter.driver_name }}(){
    const char* functionName = "~AD{{ cookiecutter.driver_name }}";

    INFO("Shutting down AD{{ cookiecutter.driver_name }} driver...");
    if(this->acquisitionActive && this->acquisitionThreadId != NULL) acquireStop();

    // Destroy any resources allocated by the vendor SDK here
    INFO("Done.");
}


//-------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} ioc shell registration
//-------------------------------------------------------------

/* {{ cookiecutter.driver_name }}Config -> These are the args passed to the constructor in the epics config function */
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg0 = { "Port name", iocshArgString };

// This parameter must be customized by the driver author. Generally a URL, Serial Number, ID, IP are used to connect.
static const iocshArg {{ cookiecutter.driver_name }}ConfigArg1 = { "Connection Param", iocshArgString };


/* Array of config args */
static const iocshArg * const {{ cookiecutter.driver_name }}ConfigArgs[] =
        { &{{ cookiecutter.driver_name }}ConfigArg0, &{{ cookiecutter.driver_name }}ConfigArg1 };


/* what function to call at config */
static void config{{ cookiecutter.driver_name }}CallFunc(const iocshArgBuf *args) {
    AD{{ cookiecutter.driver_name }}Config(args[0].sval, args[1].sval);
}


/* information about the configuration function */
static const iocshFuncDef config{{ cookiecutter.driver_name }} = { "AD{{ cookiecutter.driver_name }}Config", 2, {{ cookiecutter.driver_name }}ConfigArgs };


/* IOC register function */
static void {{ cookiecutter.driver_name }}Register(void) {
    iocshRegister(&config{{ cookiecutter.driver_name }}, config{{ cookiecutter.driver_name }}CallFunc);
}


/* external function for IOC register */
extern "C" {
    epicsExportRegistrar({{ cookiecutter.driver_name }}Register);
}
