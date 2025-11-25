/**
 * Main source file for the ADMyNewDriver EPICS driver
 *
 * This file was initially generated with the help of the ADDriverTemplate:
 * https://github.com/NSLS2/ADDriverTemplate on 16/07/2025
 *
 * Author: John Smith
 *
 * Copyright (c) : Brookhaven National Laboratory, 2025
 *
 */

#include "ADMyNewDriver.h"


using namespace std;

const char* driverName = "AD{{ cookiecutter.driver_name }}";

/*
 * External configuration function for AD{{ cookiecutter.driver_name }}.
 * Envokes the constructor to create a new AD{{ cookiecutter.driver_name }} object
 */
extern "C" int AD{{ cookiecutter.driver_name }}Config(const char* portName, const char* connectionParam){
    new AD{{ cookiecutter.driver_name }}(portName, connectionParam);
    return(asynSuccess);
}

/*
 * Callback function fired when IOC is terminated.
 * Deletes the driver object and performs cleanup
 */
static void exitCallbackC(void* pPvt) {
    AD{{ cookiecutter.driver_name }}* p{{ cookiecutter.driver_name }} = (AD{{ cookiecutter.driver_name }}*) pPvt;
    delete p{{ cookiecutter.driver_name }};
}

/**
 * Wrapper C function passed to epicsThreadCreate to create acquisition thread
 */
static void acquisitionThreadC(void *drvPvt) {
    AD{{ cookiecutter.driver_name }} *pPvt = (AD{{ cookiecutter.driver_name }}*) drvPvt;
    pPvt->acquisitionThread();
}

// -----------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Acquisition Functions
// -----------------------------------------------------------------------


/**
 * Spawns the main acquisition thread
 */
void AD{{ cookiecutter.driver_name }}::acquireStart() {
    const char* functionName = "acquireStart";

    // Spawn the acquisition thread. Make sure it's joinable.
    INFO("Spawning main acquisition thread...");

    epicsThreadOpts opts;
    opts.priority = epicsThreadPriorityHigh;
    opts.stackSize = epicsThreadGetStackSize(epicsThreadStackBig);
    opts.joinable = 1;

    this->acquisitionThreadId = epicsThreadCreateOpt(
        "acquisitionThread", (EPICSTHREADFUNC)acquisitionThreadC, this, &opts);
}

/**
 * Main acquisition loop function.
 * Ran on a dedicated thread, spawend by acquireStart().
 */
void AD{{ cookiecutter.driver_name }}::acquisitionThread() {
    const char* functionName = "acquisitionThread";

    NDDataType_t dataType;
    NDColorMode_t colorMode;
    getIntegerParam(NDColorMode, (int*) &colorMode);
    getIntegerParam(NDDataType, (int*) &dataType);

    // Determine dimensions depending on color vs. monochrome
    int ndims = 3;
    if (colorMode == NDColorMono) ndims = 2;

    int dims[ndims];
    if(ndims == 2) {
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
        setIntegerParam(NDArraySize, (int) arrayInfo.totalBytes);
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
    const char* functionName = "acquireStop";

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
 * Callback function fired when integer parameters are written to.
 */
asynStatus AD{{ cookiecutter.driver_name }}::writeInt32(asynUser* pasynUser, epicsInt32 value) {
    const char* functionName = "writeInt32";
    int function = pasynUser->reason;
    int status = asynSuccess;

    int acquiring;
    getIntegerParam(ADAcquire, &acquiring);

    // start/stop acquisition
    if(function == ADAcquire){
        if(value && !acquiring){
            acquireStart();
        } else if (!value && acquiring) {
            acquireStop();
        }
    } else if(function == ADImageMode) {
        if(acquiring == 1) acquireStop();
    } else if(function == AD{{ cookiecutter.driver_name }}_LogLevel) {
        this->logLevel = (AD{{ cookiecutter.driver_name }}_LogLevel_t) value;
    } else {
        if (function < AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM) {
            status = ADDriver::writeInt32(pasynUser, value);
        }
    }
    callParamCallbacks();

    if (status) {
        ERR_ARGS("status=%d, function=%d, value=%d\n", status, function, value);
        return asynError;
    } else {
        DEBUG_ARGS("function=%d value=%d\n", function, value);
    }
    return asynSuccess;
}

/*
 * Callback function fired when float parameters are written to.
 */
asynStatus AD{{ cookiecutter.driver_name }}::writeFloat64(asynUser* pasynUser, epicsFloat64 value) {
    const char* functionName = "writeFloat64";

    int function = pasynUser->reason;
    asynStatus status = asynSuccess;

    int acquiring;
    getIntegerParam(ADAcquire, &acquiring);

    if(function == ADAcquireTime){
        if(acquiring) acquireStop();
    } else{
        if(function < AD{{ cookiecutter.driver_name }}_FIRST_PARAM){
            status = ADDriver::writeFloat64(pasynUser, value);
        }
    }

    callParamCallbacks();

    if (status) {
        ERR_ARGS("status=%d, function=%d, value=%f\n", status, function, value);
        return asynError;
    } else {
        DEBUG_ARGS("function=%d value=%f\n", function, value);
    }
    return status;
}


//----------------------------------------------------------------------------
// AD{{ cookiecutter.driver_name }} Constructor/Destructor
//----------------------------------------------------------------------------

AD{{ cookiecutter.driver_name }}::AD{{ cookiecutter.driver_name }}(const char* portName, .....)
    : ADDriver(portName, 1, 0, 0, 0, 0, 0, 1, 0, 0) {
    const char* functionName = "AD{{ cookiecutter.driver_name }}";

    this->createAllParams();

    // Sets driver version PV (version numbers defined in header file)
    char versionString[25];
    epicsSnprintf(versionString, sizeof(versionString), "%d.%d.%d", AD{{ cookiecutter.driver_name.upper() }}_VERSION, 
                  AD{{ cookiecutter.driver_name.upper() }}_REVISION, AD{{ cookiecutter.driver_name.upper() }}_MODIFICATION);
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

    // Register an exit callback, so driver object is deleted when IOC is exited,
    // to allow for cleanup and resource deallocation
    epicsAtExit(exitCallbackC, this);
}

AD{{ cookiecutter.driver_name }}::~AD{{ cookiecutter.driver_name }}() {
    const char* functionName = "~AD{{ cookiecutter.driver_name }}";

    INFO("Shutting down AD{{ cookiecutter.driver_name }} driver...");
    if (this->acquisitionActive && this->acquisitionThreadId != NULL) acquireStop();

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


// Place the args into an array
static const iocshArg* const {{ cookiecutter.driver_name }}ConfigArgs[] = {&{{ cookiecutter.driver_name }}ConfigArg0,
                                                                           &{{ cookiecutter.driver_name }}ConfigArg1};

// Specify the C function to call when the equivalent iocsh command is run
static void config{{ cookiecutter.driver_name }}CallFunc(const iocshArgBuf* args) {
    AD{{ cookiecutter.driver_name }}Config(args[0].sval, args[1].sval);
}


/* information about the configuration function */
static const iocshFuncDef config{{ cookiecutter.driver_name }} = { "AD{{ cookiecutter.driver_name }}Config", 2, {{ cookiecutter.driver_name }}ConfigArgs };


// Static function that registers the command with the ioc shell
static void {{ cookiecutter.driver_name }}Register(void) {
    iocshRegister(&config{{ cookiecutter.driver_name }}, config{{ cookiecutter.driver_name }}CallFunc);
}

// Export the registration function
extern "C" {
epicsExportRegistrar({{ cookiecutter.driver_name }}Register);
}
