TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

PROD_NAME = {{ cookiecutter.driver_name_short.lower() }}App
PROD_IOC += $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += {{ cookiecutter.driver_name_short.lower() }}Support.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
$(PROD_NAME)_SRCS += $(PROD_NAME)_registerRecordDeviceDriver.cpp $(PROD_NAME)Main.cpp

# Add anything built by EPICS that will be statically linked (*.a libraries)
$(PROD_NAME)_LIBS += AD{{ cookiecutter.driver_name }}

# Add any system libraries + anything that will be dynamically linked (*.so libraries)
$(PROD_NAME)_SYS_LIBS += 

include $(ADCORE)/ADApp/commonDriverMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
