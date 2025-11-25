TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

PROD_NAME = {{ cookiecutter.driver_name }}App
PROD_IOC += $(PROD_NAME)

DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += {{ cookiecutter.driver_name }}Support.dbd

$(PROD_NAME)_SRCS += $(PROD_NAME)_registerRecordDeviceDriver.cpp $(PROD_NAME)Main.cpp

# Add any libraries built by the EPICS build system here
$(PROD_NAME)_LIBS += AD{{ cookiecutter.driver_name }}

{% if cookiecutter.with_cpr == true %}
$(PROD_NAME)_LIBS += cpr
$(PROD_NAME)_SYS_LIBS += curl z
{% endif %}

# Add any system libraries here

{% if cookiecutter.with_zmq == true %}
$(PROD_NAME)_SYS_LIBS += zmq
{% endif %}

include $(ADCORE)/ADApp/commonDriverMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
