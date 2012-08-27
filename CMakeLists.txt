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
# You could also create a system specific package with CPack by defining:
#    -DCREATE_PACKAGE=ON 

cmake_minimum_required(VERSION 2.8)

set(PACKAGE dnt)

# Set Version number
set(VERSION_MAJOR 0)
set(VERSION_MINOR 10)
set(VERSION_PATCH 0)
if(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
else(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
endif(${VERSION_PATCH})

# Set directories
set(DATADIR ${CMAKE_INSTALL_PREFIX}/share)
set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(LOCALE_DIR share/locale)
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
option(CREATE_PACKAGE "Create package using CPack" OFF)

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

IF(${CREATE_BUNDLE} STREQUAL "ON")

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


ELSE(${CREATE_BUNDLE} STREQUAL "ON")

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

ENDIF(${CREATE_BUNDLE} STREQUAL "ON")

# Create the .gmo files
#IF(NOT WIN32 AND NOT MINGW)
  include(GettextTranslate)
  add_subdirectory(./po)
#ENDIF(NOT WIN32 AND NOT MINGW)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# Configure some files
configure_file("./src/config.h.in" "./src/config.h")
configure_file("./Doxyfile.in" "./Doxyfile")

IF(WIN32 OR MINGW)
   configure_file("./windows/resource.rc.in" "./windows/resource.rc")
ENDIF(WIN32 OR MINGW)

IF(${CREATE_PACKAGE} STREQUAL "ON")

   IF(WIN32 OR MINGW)
      # Add DLLs
      install(FILES 
              bin/libcal3d-12.dll
	      bin/libfreetype-6.dll
	      bin/libjpeg-8.dll
	      bin/libogg-0.dll
	      bin/libpng15-15.dll
	      bin/libtiff-5.dll
	      bin/libvorbis-0.dll
	      bin/libvorbisfile-3.dll
	      bin/libwebp-2.dll
	      bin/OpenAL32.dll
	      bin/SDL.dll
	      bin/SDL_image.dll
	      bin/SDL_ttf.dll
	      bin/wrap_oal.dll
	      bin/zlib1.dll
	      DESTINATION bin)
	#Add Editors
	install(TARGETS dntMapEditor DESTINATION bin)
	install(TARGETS dntPartEditor DESTINATION bin)

        #Set Shortcuts
	#CPACK_NSIS_CREATE_ICONS_EXTRA

	# Set executables to create shortcuts
	set(CPACK_PACKAGE_EXECUTABLES 
	    "dnt;DNT"
	    "dntMapEditor;dntMapEditor"
	    "dntPartEditor;dntPartEditor")

   ENDIF(WIN32 OR MINGW)

   # Set package description/info
   set(CPACK_PACKAGE_DESCRIPTION_SUMMARY 
       "DNT - Post-Apocalyptical RPG")
   set(CPACK_PACKAGE_VENDOR "DNTeam")
   
   set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/COPYING)

   # Set package version
   set(CPACK_PACKAGE_VERSION ${VERSION})
   set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
   set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
   set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

   Include(CPack)

ENDIF(${CREATE_PACKAGE} STREQUAL "ON")


if(NOT (${CREATE_BUNDLE} STREQUAL "ON"))
   # Install files
   install(TARGETS dnt DESTINATION bin)
   install(DIRECTORY ./data DESTINATION share/${PACKAGE})
endif(NOT (${CREATE_BUNDLE} STREQUAL "ON"))

