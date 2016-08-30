################################################################################
#
# medInria
#
# Copyright (c) INRIA 2013. All rights reserved.
# See LICENSE.txt for details.
# 
#  This software is distributed WITHOUT ANY WARRANTY; without even
#  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#  PURPOSE.
#
################################################################################

cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED ${TOOLS_SUPERBUILD_VERSION})
    set(TOOLS_SUPERBUILD_VERSION 1.0.0)
endif()

project(TOOLS_SUPERBUILD VERSION ${TOOLS_SUPERBUILD_VERSION})

add_subdirectory(superbuild)