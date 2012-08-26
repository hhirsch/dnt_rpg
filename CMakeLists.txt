# This is the CMake definition file for DNT usage.
#
# Usually you should just call:
#
#    cmake .
#
# To create the needed makefiles for DNT compilation.
# 
# You could also set some debug options with:
#    -DDEBUG_ASTAR=ON 
#    -DDNT_DEBUG_PENDING_ACTION=ON
#    -DDNT_DEBUG_SCRIPTS=ON
#
# If you desire to create the OSX dnt.app bundle application, you should define:
#    -DCREATE_BUNDLE=ON
# 

cmake_minimum_required(VERSION 2.8)

set(PACKAGE dnt)
set(VERSION "0.10")
set(DATADIR ${CMAKE_INSTALL_PREFIX}/share)
set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(INSTALL_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)

set(BIN_DIR ./bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ./CMakeModules)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./CMakeFiles)

project(dnt)

# Some Options
option(DEBUG_ASTAR "Enable A* Visual Debug" OFF)
option(DNT_DEBUG_PENDING_ACTION "Enable Pending Action Debug Log" OFF)
option(DNT_DEBUG_SCRIPTS "Enable DNT Script Language Debug Log" OFF)
option(CREATE_BUNDLE "Create OSX Bundle Application" OFF)

# Some compiler options
if(UNIX)
   add_definitions(-Wall)
endif(UNIX)


# First, try to find required libraries

FIND_PACKAGE(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

FIND_PACKAGE(SDL_image REQUIRED)
include_directories(${SDLIMAGE_INCLUDE_DIR})

FIND_PACKAGE(SDL_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})


FIND_PACKAGE(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

FIND_PACKAGE(Cal3d REQUIRED)
include_directories(${CAL3D_INCLUDE_DIR})

FIND_PACKAGE(Ogg REQUIRED)
include_directories(${OGG_INCLUDE_DIR})

FIND_PACKAGE(Vorbis REQUIRED)
include_directories(${VORBIS_INCLUDE_DIR})

FIND_PACKAGE(Vorbisfile REQUIRED)
include_directories(${VORBISFILE_INCLUDE_DIR})

FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND)
   set(ENABLE_NLS 1)   
ELSE(GETTEXT_FOUND)
   set(ENABLE_NLS 0) 
ENDIF(GETTEXT_FOUND)

FIND_PACKAGE(Libintl)
include_directories(${LIBINTL_INCLUDE_DIR})

if(NOT ${LIBINTL_LIB_FOUND})
   set(LIBINTL_LIBRARIES "")
endif(NOT ${LIBINTL_LIB_FOUND})

# include the ./src as default too
include_directories(${PROJECT_SOURCE_DIR}/src)

# Include the sources
include(sources.cmake)

# Make Libraries
add_library(farso ${FARSO_SOURCES} ${FARSO_HEADERS} )
add_library(dntlib ${DNT_LIB_SOURCES} ${DNT_LIB_HEADERS} )

IF(${CREATE_BUNDLE})

   # Reset the data dir
   set(DATADIR "/data")

   # Add OSX icon
   set(OSX_ICON_FILES ${PROJECT_SOURCE_DIR}/macosx/dnt-icon.icns)
   set_source_files_properties(${OSX_ICON_FILES} PROPERTIES 
                      MACOSX_PACKAGE_LOCATION Resources)
   set(DNT_SOURCES ${DNT_SOURCES} ${OSX_ICON_FILES}) 

   # Create the executable as BUNDLE
   add_executable(dnt MACOSX_BUNDLE ${DNT_SOURCES} ${DNT_HEADERS} )
   
   # Search for bundle libraries
   find_library(IOKIT_LIBRARY IOKit)
   find_library(QUICKTIME_LIBRARY QuickTime)
   find_library(CARBON_LIBRARY Carbon)
   find_library(AUDIOUNIT_LIBRARY AudioUnit)
   find_library(COCOA_LIBRARY Cocoa)

   target_link_libraries(dnt
	         ${IOKIT_LIBRARY}
	         ${QUICKTIME_LIBRARY}
	         ${CARBON_LIBRARY}
	         ${AUDIOUNIT_LIBRARY}
	         ${COCOA_LIBRARY}
                 ${OPENGL_LIBRARY} 
                 ${OPENAL_LIBRARY} 
                 ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                 ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                 ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                 ${LIBINTL_LIBRARIES} pthread
                 dntlib farso)

   # configure CMake to use a custom Info.plist
   set_target_properties(dnt PROPERTIES MACOSX_BUNDLE_INFO_PLIST 
         ${PROJECT_SOURCE_DIR}/macosx/DNT-Info.plist)
   
   # Add data files to resources
   add_custom_target(dntData COMMAND cp -R ${PROJECT_SOURCE_DIR}/data 
                       ${CMAKE_BINARY_DIR}/bin/dnt.app/Contents/Resources)
   add_dependencies(dnt dntData)


ELSE(${CREATE_BUNDLE})

   # Make Binaries
   add_executable(dnt WIN32 ${DNT_SOURCES} ${DNT_HEADERS} )
   target_link_libraries(dnt dntlib farso ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY} 
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} ${CAL3D_LIBRARY} m
                          ${LIBINTL_LIBRARIES} pthread)
                          
   add_executable(dntMapEditor WIN32
                  ${DNT_MAP_EDITOR_SOURCES} ${DNT_MAP_EDITOR_HEADERS})
   target_link_libraries(dntMapEditor dntlib farso ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                          ${LIBINTL_LIBRARIES} pthread)
   add_executable(dntPartEditor WIN32 
               ${DNT_PART_EDITOR_SOURCES} ${DNT_PART_EDITOR_HEADERS})
   target_link_libraries(dntPartEditor dntlib farso ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                          ${LIBINTL_LIBRARIES} pthread)
ENDIF(${CREATE_BUNDLE})

# Create the .gmo files
IF(NOT WIN32 AND NOT MINGW)
  include(GettextTranslate)
  add_subdirectory(./po)
ENDIF(NOT WIN32 AND NOT MINGW)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# Configure some files
configure_file("./src/config.h.in" "./src/config.h")
configure_file("./Doxyfile.in" "./Doxyfile")

#IF(WIN32 OR MINGW)
#   Include(CPack)
#ENDIF(WIN32 OR MINGW)


if(NOT ${CREATE_BUNDLE})

# Install files
install(TARGETS dnt DESTINATION bin)
install(DIRECTORY ./data DESTINATION ${DATADIR}/${PACKAGE})

endif(NOT ${CREATE_BUNDLE})

