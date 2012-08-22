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
#    -DCREATE_BUNDLE=1
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

# Some compiler options
if(UNIX)
   add_definitions(-Wall)
endif(UNIX)


# First, try to find required libraries

FIND_PACKAGE(SDL REQUIRED)
IF(NOT SDL_FOUND)
   MESSAGE("\n FATAL_ERROR_SDL: Could not find SDL Library.\n")
   return()
ENDIF(NOT SDL_FOUND)
include_directories(${SDL_INCLUDE_DIR})

FIND_PACKAGE(SDL_image REQUIRED)
IF(NOT SDLIMAGE_FOUND)
   MESSAGE("\n FATAL_ERROR_SDL_IMAGE: Could not find SDL_image Library.\n")
   return()
ENDIF(NOT SDLIMAGE_FOUND)
include_directories(${SDLIMAGE_INCLUDE_DIR})

FIND_PACKAGE(SDL_ttf REQUIRED)
IF(NOT SDLTTF_FOUND)
   MESSAGE("\n FATAL_ERROR_SDL_TTF: Could not find SDL_ttf Library.\n")
   return()
ENDIF(NOT SDLTTF_FOUND)
include_directories(${SDLTTF_INCLUDE_DIR})


FIND_PACKAGE(OpenGL REQUIRED)
IF(NOT OPENGL_FOUND)
   MESSAGE("\n FATAL_ERROR_OPENGL: Could not find OpenGL Library.\n")
   return()
ENDIF(NOT OPENGL_FOUND)
IF(NOT OPENGL_GLU_FOUND)
   MESSAGE("\n FATAL_ERROR_GLU: Could not find GLU Library.\n")
   return()
ENDIF(NOT OPENGL_GLU_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(OpenAL REQUIRED)
IF(NOT OPENAL_FOUND)
   MESSAGE("\n FATAL_ERROR_OPENAL: Could not find OpenAL Library.\n")
   return()
ENDIF(NOT OPENAL_FOUND)
include_directories(${OPENAL_INCLUDE_DIR})

FIND_PACKAGE(Cal3d REQUIRED)
IF(NOT CAL3D_FOUND)
   MESSAGE("\n FATAL_ERROR_CAL3D: Could not find Cal3D Library.\n")
   return()
ENDIF(NOT CAL3D_FOUND)
include_directories(${CAL3D_INCLUDE_DIR})

FIND_PACKAGE(Ogg REQUIRED)
IF(NOT OGG_FOUND)
   MESSAGE("\n FATAL_ERROR_OGG: Could not find ogg Library.\n")
   return()
ENDIF(NOT OGG_FOUND)
include_directories(${OGG_INCLUDE_DIR})

FIND_PACKAGE(Vorbis REQUIRED)
IF(NOT VORBIS_FOUND)
   MESSAGE("\n FATAL_ERROR_VORBIS: Could not find vorbis Library.\n")
   return()
ENDIF(NOT VORBIS_FOUND)
include_directories(${VORBIS_INCLUDE_DIR})

FIND_PACKAGE(Vorbisfile REQUIRED)
IF(NOT VORBISFILE_FOUND)
   MESSAGE("\n FATAL_ERROR_VORBIS_FILE: Could not find vorbisfile Library.\n")
   return()
ENDIF(NOT VORBISFILE_FOUND)
include_directories(${VORBISFILE_INCLUDE_DIR})

FIND_PACKAGE(Gettext)
IF(GETTEXT_FOUND)
   set(ENABLE_NLS 1)   
ELSE(GETTEXT_FOUND)
   set(ENABLE_NLS 0) 
ENDIF(GETTEXT_FOUND)

FIND_PACKAGE(Libintl)
include_directories(${LIBINTL_INCLUDE_DIR})

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
   add_executable(dnt ${DNT_SOURCES} ${DNT_HEADERS} )
   target_link_libraries(dnt ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY} 
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                          ${LIBINTL_LIBRARIES} pthread
                          dntlib farso)
   add_executable(dntMapEditor ${DNT_MAP_EDITOR_SOURCES} ${DNT_MAP_EDITOR_HEADERS})
   target_link_libraries(dntMapEditor ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                          ${LIBINTL_LIBRARIES} pthread
                          dntlib farso)
   add_executable(dntPartEditor 
               ${DNT_PART_EDITOR_SOURCES} ${DNT_PART_EDITOR_HEADERS})
   target_link_libraries(dntPartEditor ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} 
                          ${LIBINTL_LIBRARIES} pthread
                          dntlib farso)
ENDIF(${CREATE_BUNDLE})

# Create the .gmo files
include(GettextTranslate)
add_subdirectory(./po)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# Configure some files
configure_file("./src/config.h.in" "./src/config.h")
configure_file("./Doxyfile.in" "./Doxyfile")


if(NOT ${CREATE_BUNDLE})

# Install files
install(TARGETS dnt DESTINATION bin)
install(DIRECTORY ./data DESTINATION ${DATADIR}/${PACKAGE})

endif(NOT ${CREATE_BUNDLE})

