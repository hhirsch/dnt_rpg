#ifndef _message_h
#define _message_h

#include "../../gui/farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


#define TOOL_TERRAIN_UP             11
#define TOOL_TERRAIN_NIVELATE       12
#define TOOL_TERRAIN_DOWN           13
#define TOOL_TERRAIN_TEXTURE        14

#define TOOL_WALL_ADD_X             21
#define TOOL_WALL_ADD_Z             22
#define TOOL_WALL2_ADD_X            23
#define TOOL_WALL2_ADD_Z            24
#define TOOL_WALL_TEXTURE           25
#define TOOL_WALL_LESS_VER_TEXTURE  26
#define TOOL_WALL_MORE_VER_TEXTURE  27
#define TOOL_WALL_LESS_HOR_TEXTURE  28
#define TOOL_WALL_MORE_HOR_TEXTURE  29


#define TOOL_PORTAL_ADD             31
#define TOOL_PORTAL_TAG             32



string messageForTool(int tool);

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

