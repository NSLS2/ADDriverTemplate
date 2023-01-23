#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) configure
DIRS := $(DIRS) {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}App
DIRS := $(DIRS) {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}Support

{{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}App_DEPEND_DIRS += {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}Support
ifeq ($(BUILD_IOCS), YES)
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocs))
iocs_DEPEND_DIRS += {{ cookiecutter.driver_name[0].lower() + cookiecutter.driver_name[1:] }}App
endif
include $(TOP)/configure/RULES_TOP

uninstall: uninstall_iocs
uninstall_iocs:
	$(MAKE) -C iocs uninstall
.PHONY: uninstall uninstall_iocs

realuninstall: realuninstall_iocs
realuninstall_iocs:
	$(MAKE) -C iocs realuninstall
.PHONY: realuninstall realuninstall_iocs
