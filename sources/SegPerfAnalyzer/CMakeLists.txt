# INRIA and LaTIM Copyright (C) 2014. <License>.

#
# Project Properties
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)
PROJECT (SegPerfAnalyzer)
SET (APPLICATION_NAME "SegPerfAnalyzer")
SET (APPLICATION_CODENAME "${PROJECT_NAME}")
SET (APPLICATION_COPYRIGHT_YEARS "2014")
SET (APPLICATION_VERSION_MAJOR 2)
SET (APPLICATION_VERSION_MINOR 1)
SET (APPLICATION_VERSION_PATCH 0)
SET (APPLICATION_VERSION_TYPE SNAPSHOT)
SET (APPLICATION_VERSION_STRING "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}.${APPLICATION_VERSION_PATCH}-${APPLICATION_VERSION_TYPE}")
#SET (APPLICATION_VENDOR_ID "")
SET (APPLICATION_VENDOR_NAME "INRIA")
SET (APPLICATION_VENDOR_URL "http://www.inria.fr/")
SET (APPLICATION_ID "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")

#DOXY_OUTPUT_DIRECTORY
#DOXY_OUTPUT_WARNING
#DOXY_INPUT_SRCS
#DOXY_EXCLUDE_SRCS
#DOXY_PERL_BIN_PATH  /usr/bin/perl.

include(DirectoryFinder.cmake)

###############################################################################
###############################################################################
### Add sources to the project
###############################################################################

message("TCLAP include dir :${TCLAP_INCLUDE_DIRS}" )
	
if(NOT DEFINED TCLAP_INCLUDE_DIRS)
	message("TCLAP include dir not defined. Default will be used")
    set(TCLAP_INCLUDE_DIRS ../../common/libs/project/ext/tclap-1.2.1/include CACHE PATH "Please indicate the TCLAP directory...")
endif()
	
include_directories(AFTER
    ${TCLAP_INCLUDE_DIRS}
    ${EXTERNAL_LIBS}
    src)
	
SET(ANALYZER_SRC 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/SegPerfApp.cpp
	${PROJECT_SOURCE_DIR}/src/SegPerfApp.h
	
	${PROJECT_SOURCE_DIR}/src/Analyzer/Analyzer.cpp
	${PROJECT_SOURCE_DIR}/src/Analyzer/Analyzer.h
	${PROJECT_SOURCE_DIR}/src/Analyzer/SegmentationMeasuresImageFilter.hxx
	${PROJECT_SOURCE_DIR}/src/Analyzer/SegmentationMeasuresImageFilter.h
	
	${PROJECT_SOURCE_DIR}/src/Results/Results.cpp
	${PROJECT_SOURCE_DIR}/src/Results/Results.h)


###############################################################################
###############################################################################
### Creation of groups of sources for IDE
###############################################################################

source_group("Header Files" FILES       )

source_group("Source Files" FILES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/SegPerfApp.cpp
    ${PROJECT_SOURCE_DIR}/src/SegPerfApp.h)

source_group("Source Files\\Analyzer" FILES
    ${PROJECT_SOURCE_DIR}/src/Analyzer/Analyzer.cpp
    ${PROJECT_SOURCE_DIR}/src/Analyzer/Analyzer.h
    ${PROJECT_SOURCE_DIR}/src/Analyzer/SegmentationMeasuresImageFilter.hxx
    ${PROJECT_SOURCE_DIR}/src/Analyzer/SegmentationMeasuresImageFilter.h)

source_group("Source Files\\Results" FILES
    ${PROJECT_SOURCE_DIR}/src/Results/Results.cpp
    ${PROJECT_SOURCE_DIR}/src/Results/Results.h)


###############################################################################
###############################################################################
### Find ITK library
###############################################################################
if(MSVC)
	if(CMAKE_CL_64)
		LINK_DIRECTORIES()
	else(CMAKE_CL_64)
		LINK_DIRECTORIES()
	endif(CMAKE_CL_64)
else(MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		LINK_DIRECTORIES()
	else(CMAKE_SIZEOF_VOID_P EQUAL 8)
		LINK_DIRECTORIES()
	endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif()


FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build ImageD without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)


###############################################################################
###############################################################################
### Setup documentation platform
###############################################################################


FIND_PACKAGE(Doxygen)
if(NOT DOXYGEN_DOT_PATH)
	SET(baseDirs
	    "C:/Program\ Files\ (x86)"
		"C:/Program\ Files")
		
	DIRSMATCHING( result baseDirs  "Graphviz(.*)" )
	GETFIRSTELEM( DOXYGEN_DOT_PATH result)
	FIND_PACKAGE(Doxygen)
	message(${DOXYGEN_DOT_PATH})
	SET(DOXYGEN_DOT_EXECUTABLE "${DOXYGEN_DOT_PATH}/bin/dot.exe" CACHE FILEPATH "Graphviz Dot tool for using Doxygen" FORCE)
	#UNSET(DOXYGEN_DOT_PATH)
endif()

SET(DOXYFILE_FOUND false)
IF(DOXYGEN_DOT_EXECUTABLE AND DOXYGEN_EXECUTABLE)
	SET( DOXYFILE_IN            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in             )
	SET( DOXYFILE               ${PROJECT_BINARY_DIR}/DoxyDoc/Doxyfile              )
	SET( DOXY_HTML_INDEX_FILE   ${CMAKE_CURRENT_BINARY_DIR}/DoxyDoc/html/index.html )
	SET( DOXY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/DoxyDoc/                ) # Pasted into Doxyfile.in
	SET( DOXY_OUTPUT_WARNING    ${CMAKE_CURRENT_BINARY_DIR}/DoxyDoc/DoxyWarn.txt    ) # Pasted into Doxyfile.in
	SET( DOXY_INPUT_SRCS        ${PROJECT_SOURCE_DIR}/src                           ) # Pasted into Doxyfile.in
	SET( DOXY_EXCLUDE_SRCS                                                          ) # Pasted into Doxyfile.in
	SET( DOXY_PERL_BIN_PATH     /usr/bin/perl                                       ) # Pasted into Doxyfile.in
	
	file(REMOVE "${DOXY_OUTPUT_WARNING}")
	FOREACH(curSubDir ${EXTERNAL_LIBS})
	    set(DOXY_EXCLUDE_SRCS "${DOXY_EXCLUDE_SRCS} \\\n\t\"${CMAKE_CURRENT_SOURCE_DIR}/${curSubDir}\"")
	ENDFOREACH()
	
	CONFIGURE_FILE( ${DOXYFILE_IN} ${DOXYFILE} @ONLY )
	ADD_CUSTOM_COMMAND( OUTPUT ${DOXY_HTML_INDEX_FILE}
					  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
					  # The following should be ${DOXYFILE} only but it
					  # will break the dependency.
					  # The optimal solution would be creating a 
					  # custom_command for ${DOXYFILE} generation
					  # but I still have to figure out how...
					  MAIN_DEPENDENCY ${DOXYFILE} ${DOXYFILE_IN}
					  #DEPENDS ${DOXYFILE}
					  COMMENT "Generating HTML documentation")
 
  ADD_CUSTOM_TARGET( doc ALL DEPENDS ${DOXY_HTML_INDEX_FILE} )
 
  INSTALL( DIRECTORY ${DOXY_OUTPUT_DIRECTORY}/html DESTINATION share/doc )
  
ELSE()


# option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ON)
# IF(BUILD_DOCUMENTATION AND DOXYFILE_FOUND)
  # MESSAGE(FATAL_ERROR
      # "Doxygen is needed to build the documentation.")

  # SET( DOXYFILE_IN            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in     )
  # SET( DOXYFILE               ${PROJECT_BINARY_DIR}/Doxyfile              )
  # SET( DOXY_HTML_INDEX_FILE   ${CMAKE_CURRENT_BINARY_DIR}/html/index.html )
  # SET( DOXY_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}                 ) # Pasted into Doxyfile.in
  # SET( DOXY_OUTPUT_WARNING    ${CMAKE_CURRENT_BINARY_DIR}                 ) # Pasted into Doxyfile.in
  # SET( DOXY_INPUT_SRCS        ${PROJECT_SOURCE_DIR}/src                   ) # Pasted into Doxyfile.in
  # SET( DOXY_EXCLUDE_SRCS      ""                                          ) # Pasted into Doxyfile.in
  # SET( DOXY_PERL_BIN_PATH     /usr/bin/perl                               ) # Pasted into Doxyfile.in
  
  
  # CONFIGURE_FILE( ${DOXYFILE_IN} ${doxyfile} @ONLY )
 
  # ADD_CUSTOM_COMMAND( OUTPUT ${DOXY_HTML_INDEX_FILE}
                      # COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
                      # The following should be ${doxyfile} only but it
                      # will break the dependency.
                      # The optimal solution would be creating a 
                      # custom_command for ${doxyfile} generation
                      # but I still have to figure out how...
                      # MAIN_DEPENDENCY ${doxyfile} ${DOXYFILE_IN}
                      # DEPENDS project_targets
                      # COMMENT "Generating HTML documentation")
 
  # ADD_CUSTOM_TARGET( doc ALL DEPENDS ${DOXY_HTML_INDEX_FILE} )

  # INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc )
# ENDIF()

endif()


###############################################################################
###############################################################################
### Defines target
###############################################################################
# Add the executable
include_directories(${TCLAP_INCLUDE_DIRS})
ADD_EXECUTABLE(${APPLICATION_NAME} ${ANALYZER_SRC})
target_link_libraries(${APPLICATION_NAME} ${ITK_LIBRARIES}) 


###############################################################################
###############################################################################
### Add externals libraries
###############################################################################



