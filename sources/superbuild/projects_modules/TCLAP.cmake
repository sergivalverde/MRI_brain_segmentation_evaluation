function(TCLAP_project)
set(ep TCLAP)


## #############################################################################
## List the dependencies of the project
## #############################################################################

list(APPEND ${ep}_dependencies 
  )
  
## #############################################################################
## Prepare the project
## ############################################################################# 

EP_Initialisation(${ep})

if (NOT USE_SYSTEM_${ep})

## #############################################################################
## Set download url
## #############################################################################

set(download_url http://vorboss.dl.sourceforge.net/project/tclap/tclap-1.2.1.tar.gz)

## #############################################################################
## Add external-project
## #############################################################################

ExternalProject_Add(${ep}
  PREFIX ${EP_PREFIX_thirdparts}
  URL ${download_url}
  DEPENDS ${${ep}_dependencies}
  CMAKE_COMMAND ""
  BUILD_COMMAND ""
  CONFIGURE_COMMAND ""
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  )

## #############################################################################
## Set variable to provide infos about the project
## #############################################################################

ExternalProject_Get_Property(TCLAP source_dir)
set(${ep}_DIR ${source_dir})
set(${ep}_INCLUDE_DIRS ${${ep}_DIR}/include  PARENT_SCOPE)


endif() #NOT USE_SYSTEM_ep

endfunction()
