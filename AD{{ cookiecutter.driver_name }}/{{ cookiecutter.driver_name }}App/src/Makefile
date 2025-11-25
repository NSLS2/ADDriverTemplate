TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

USR_CPPFLAGS += -std=c++17

INC += AD{{ cookiecutter.driver_name }}.h
INC += AD{{ cookiecutter.driver_name }}ParamDefs.h

LIBRARY_IOC = AD{{ cookiecutter.driver_name }}
LIB_SRCS += AD{{ cookiecutter.driver_name }}.cpp
LIB_SRCS += AD{{ cookiecutter.driver_name }}ParamDefs.cpp

DBD += AD{{ cookiecutter.driver_name }}Support.dbd

include $(ADCORE)/ADApp/commonLibraryMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
