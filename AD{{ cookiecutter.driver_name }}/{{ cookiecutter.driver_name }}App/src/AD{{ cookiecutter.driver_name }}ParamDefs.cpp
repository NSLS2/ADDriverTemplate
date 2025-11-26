// This file is auto-generated. Do not edit directly.
// Generated from AD{{ cookiecutter.driver_name }}.template

#include "AD{{ cookiecutter.driver_name }}.h"

void AD{{ cookiecutter.driver_name }}::createAllParams() {
    createParam(AD{{ cookiecutter.driver_name }}_LogLevelString, asynParamInt32, &AD{{ cookiecutter.driver_name }}_LogLevel);
}
