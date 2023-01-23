TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
USR_CPPFLAGS += -D__X64
endif

USR_CPPFLAGS += -std=c++11

INC += AD{{ cookiecutter.driver_name }}.h

LIBRARY_IOC = AD{{ cookiecutter.driver_name }} 
LIB_SRCS += AD{{ cookiecutter.driver_name }}.cpp


DBD += {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}Support.dbd

include $(ADCORE)/ADApp/commonLibraryMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
