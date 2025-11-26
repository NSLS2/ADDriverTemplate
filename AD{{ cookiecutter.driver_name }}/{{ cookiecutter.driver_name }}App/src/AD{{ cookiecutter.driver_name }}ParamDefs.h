#ifndef AD{{ cookiecutter.driver_name.upper() }}_PARAM_DEFS_H
#define AD{{ cookiecutter.driver_name.upper() }}_PARAM_DEFS_H

// This file is auto-generated. Do not edit directly.
// Generated from AD{{ cookiecutter.driver_name }}.template

// String definitions for parameters
#define AD{{ cookiecutter.driver_name }}_LogLevelString "{{ cookiecutter.driver_name.upper() }}_LOG_LEVEL"

// Parameter index definitions
int AD{{ cookiecutter.driver_name }}_LogLevel;

#define AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM AD{{ cookiecutter.driver_name }}_LogLevel
#define AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM AD{{ cookiecutter.driver_name }}_LogLevel

#define NUM_AD{{ cookiecutter.driver_name.upper() }}_PARAMS ((int)(&AD{{ cookiecutter.driver_name.upper() }}_LAST_PARAM - &AD{{ cookiecutter.driver_name.upper() }}_FIRST_PARAM + 1))

#endif
