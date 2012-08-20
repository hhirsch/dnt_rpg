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

# include the ./src as default too
include_directories(${PROJECT_SOURCE_DIR}/src)


# Include the sources
include(sources.cmake)

# Make Libraries
add_library(farso ${FARSO_SOURCES} ${FARSO_HEADERS} )
add_library(dntlib ${DNT_LIB_SOURCES} ${DNT_LIB_HEADERS} )

# Make Binaries
add_executable(dnt ${DNT_SOURCES} ${DNT_HEADERS} )
target_link_libraries(dnt ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY} 
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} pthread
                          dntlib farso)
add_executable(dntMapEditor ${DNT_MAP_EDITOR_SOURCES} ${DNT_MAP_EDITOR_HEADERS})
target_link_libraries(dntMapEditor ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} pthread
                          dntlib farso)
add_executable(dntPartEditor 
               ${DNT_PART_EDITOR_SOURCES} ${DNT_PART_EDITOR_HEADERS})
target_link_libraries(dntPartEditor ${OPENGL_LIBRARY} 
                          ${OPENAL_LIBRARY}
                          ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLTTF_LIBRARY}
                          ${VORBISFILE_LIBRARY} ${VORBIS_LIBRARY}
                          ${OGG_LIBRARY} m ${CAL3D_LIBRARY} pthread
                          dntlib farso)

# Create the .gmo files
#include(po/linguas.cmake)
#GETTEXT_CREATE_TRANSLATIONS(po/dnt.pot ALL ${LINGUAS})
include(GettextTranslate)
add_subdirectory(./po)

#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

# Configure some files
configure_file("./src/config.h.in" "./src/config.h")
configure_file("./Doxyfile.in" "./Doxyfile")

# Install files
install(TARGETS dnt DESTINATION bin)
install(DIRECTORY ./data DESTINATION ${DATADIR}/${PACKAGE})

