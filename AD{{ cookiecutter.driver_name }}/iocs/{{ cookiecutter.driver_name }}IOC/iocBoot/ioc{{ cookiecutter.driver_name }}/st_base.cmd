
errlogInit(20000)

< envPaths

epicsEnvSet("ENGINEER", "{{ cookiecutter.author }}")
epicsEnvSet("PORT",     "{{ cookiecutter.driver_name.upper() + '1' }}")
epicsEnvSet("IOC",      "ioc{{ cookiecutter.driver_name }}")
epicsEnvSet("PREFIX",   "DEV:{{ cookiecutter.driver_name.upper() }}1:")
epicsEnvSet("HOSTNAME", "localhost")
epicsEnvSet("IOCNAME",  "{{ cookiecutter.driver_name }}")
epicsEnvSet("QSIZE",    "30")
epicsEnvSet("NCHANS",   "2048")
epicsEnvSet("HIST_SIZE","4096")
epicsEnvSet("XSIZE",    "256")
epicsEnvSet("YSIZE",    "256")
epicsEnvSet("NELMT",    "65536")
epicsEnvSet("NDTYPE",   "Int16")  #'Int8' (8bit B/W, Color) | 'Int16' (16bit B/W)
epicsEnvSet("NDFTVL",   "SHORT") #'UCHAR' (8bit B/W, Color) | 'SHORT' (16bit B/W)
epicsEnvSet("CBUFFS",   "500")
epicsEnvSet("EPICS_DB_INCLUDE_PATH", "$(ADCORE)/db")

dbLoadDatabase("$(AD{{ cookiecutter.driver_name }})/iocs/{{ cookiecutter.driver_name }}IOC/dbd/{{ cookiecutter.driver_name }}App.dbd")
{{ cookiecutter.driver_name }}App_registerRecordDeviceDriver(pdbbase)

# Create instance of AD{{ cookiecutter.driver_name }} driver
#AD{{ cookiecutter.driver_name }}Config(const char* portName, const char* connectionParam)
AD{{ cookiecutter.driver_name }}Config("$(PORT)", "0")
epicsThreadSleep(2)

# Default logging settings
asynSetTraceIOMask($(PORT), 0, 2)

dbLoadRecords("$(ADCORE)/db/ADBase.template", "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")
dbLoadRecords("$(AD{{ cookiecutter.driver_name.upper() }})/db/AD{{ cookiecutter.driver_name }}.template","P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

#
# Create a standard arrays plugin, set it to get data from Driver.
#int NDStdArraysConfigure(const char *portName, int queueSize, int blockingCallbacks, const char *NDArrayPort, int NDArrayAddr, int maxBuffers, size_t maxMemory,
#                          int priority, int stackSize, int maxThreads)
NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0)
dbLoadRecords("$(ADCORE)/db/NDStdArrays.template", "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,NDARRAY_PORT=$(PORT),TIMEOUT=1,TYPE=Int16,FTVL=SHORT,NELEMENTS=20000000")

# Load all other plugins using commonPlugins.cmd
< $(ADCORE)/iocBoot/commonPlugins.cmd

set_requestfile_path("$(AD{{ cookiecutter.driver_name }})/db")

iocInit()

# save things every thirty seconds
create_monitor_set("auto_settings.req", 30, "P=$(PREFIX)")
