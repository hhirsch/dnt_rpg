/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_editor_message_h
#define _dnt_map_editor_message_h

#include "../../gui/farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#define TOOL_NONE                    0

#define TOOL_TERRAIN_UP             11
#define TOOL_TERRAIN_NIVELATE       12
#define TOOL_TERRAIN_DOWN           13
#define TOOL_TERRAIN_TEXTURE        14

#define TOOL_WALL_ADD_X             21
#define TOOL_WALL_ADD_Z             22
#define TOOL_WALL_CUT               23
#define TOOL_WALL_TEXTURE           25
#define TOOL_WALL_LESS_Y_TEXTURE    26
#define TOOL_WALL_MORE_Y_TEXTURE    27
#define TOOL_WALL_LESS_X_TEXTURE    28
#define TOOL_WALL_MORE_X_TEXTURE    29
#define TOOL_WALL_LESS_Z_TEXTURE    30
#define TOOL_WALL_MORE_Z_TEXTURE    31
#define TOOL_WALL_DESTROY           32

#define TOOL_PORTAL_ADD             131
#define TOOL_PORTAL_TAG             132
#define TOOL_PORTAL_DOOR            133

#define TOOL_OBSTACLE_ADD           141

#define TOOL_PARTICLE_DEFAULT       151
#define TOOL_PARTICLE_LAKE          152
#define TOOL_PARTICLE_GRASS         133

#define TOOL_NPC_ADD                161

string messageForTool(int tool);

/*! Get a string from user input
 * \param title -> title of the window
 * \param previous -> previous value of the string
 * \param proj -> projection matrix
 * \param modl -> model view matrix
 * \param viewPort -> viewPort vector
 * \return the value of the screen selected */
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

/*! Get a Option from user input
 * \param title -> title of the window
 * \param message -> message to show
 * \param opt1 -> Option 1
 * \param opt2 -> Option 2
 * \param proj -> projection matrix
 * \param modl -> model view matrix
 * \param viewPort -> viewPort vector
 * \return the value of the option selected (1 or 2) */
int getOptionFromUser(string title, string message, string opt1, string opt2,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);



#endif

