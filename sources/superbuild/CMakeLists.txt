## #############################################################################
## Add exteral projects
## #############################################################################
                                                                                                                                                                                                                                          
# There are three possibilities:
# - if the option USE_SYSTEM_${external_project} is set, then the system version  
#   will be used (not yet fully tested)
# - if the option USE_SYSTEM_${external_project} is not set:
#   * there is a subdir with the package name, in which case this subdir is used 
#     directly
#   * otherwise use download and compile locally the package as an external 
#     module.

list(APPEND external_projects
  ITK
  TCLAP
  SegPerfAnalyzer
  )

## #############################################################################
## Setup build configuration
## #############################################################################

unset(CMAKE_BUILD_TYPE CACHE)
set(CMAKE_BUILD_TYPE_thirdparts Release CACHE STRING "Build type configuration for third parts libraries.")
set(CMAKE_BUILD_TYPE_tools Debug CACHE STRING "Build type configuration for tools.")


## #############################################################################
## Third parts prefix
## #############################################################################

set(EP_PREFIX_thirdparts 
  ${CMAKE_BINARY_DIR}/thirdparts CACHE STRING 
  "Prefix path where to download and build third parts libraries."
  )

## #############################################################################
## SegPerfAnalyzer dirs.
## #############################################################################
set(SegPerfAnalyzer_SOURCE_DIR ${CMAKE_SOURCE_DIR}/SegPerfAnalyzer)
set(SegPerfAnalyzer_BINARY_DIR ${CMAKE_BINARY_DIR}/SegPerfAnalyzer-build)
set(SegPerfAnalyzer_STAMP_DIR ${CMAKE_BINARY_DIR}/SegPerfAnalyzer-stamp)


## #############################################################################
## Add modules
## #############################################################################

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/environment
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration_steps
  ${CMAKE_CURRENT_SOURCE_DIR}/projects_modules
  ${CMAKE_MODULE_PATH}
  )

## #############################################################################
## Check build environment
## #############################################################################

include(CheckEnvironment)

## #############################################################################
## Configure external-projects
## #############################################################################

include(ConfigureExternalProjects)