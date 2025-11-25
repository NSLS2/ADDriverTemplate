TOP=..
include $(TOP)/configure/CONFIG

#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

ifeq ($(WITH_CPR), YES)
DIRS := $(DIRS) cpr
endif

ifeq ($(WITH_JSON), YES)
DIRS := $(DIRS) json
endif

ifeq ($(WITH_GTEST), YES)
DIRS := $(DIRS) gtest
endif

ifeq ($(WITH_MAGIC_ENUM), YES)
DIRS := $(DIRS) magic_enum
endif


#=============================

include $(TOP)/configure/RULES_DIRS
#----------------------------------------
#  ADD RULES AFTER THIS LINE
