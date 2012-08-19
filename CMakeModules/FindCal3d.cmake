# Locate Cal3d library
# This module defines
# CAL3D_LIBRARY, the name of the library to link against
# CAL3D_FOUND, if false, do not try to link to CAL3D
# CAL3D_INCLUDE_DIR, where to find cal3d/cal3d.h
#
# $CAL3DDIR is an environment variable that would
# correspond to the ./configure --prefix=$CAL3DDIR
# used in building Cal3D.
#
# Created by Farrer. This was influenced by the FindSDL_image.cmake module.

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================

FIND_PATH(CAL3D_INCLUDE_DIR cal3d/cal3d.h
  HINTS
  $ENV{CAL3DDIR}
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local/include/cal3d
  /usr/include/cal3d
  /usr/local/include
  /usr/include
  /sw/include/cal3d # Fink
  /sw/include
  /opt/local/include/cal3d # DarwinPorts
  /opt/local/include
  /opt/csw/include/cal3d # Blastwave
  /opt/csw/include 
  /opt/include/cal3d
  /opt/include
)

FIND_LIBRARY(CAL3D_LIBRARY 
  NAMES cal3d
  HINTS
  $ENV{CAL3DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CAL3D
                                  REQUIRED_VARS CAL3D_LIBRARY CAL3D_INCLUDE_DIR)
