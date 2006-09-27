#ifndef _message_h
#define _message_h

#include "../../gui/farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


#define TOOL_TERRAIN_UP       11
#define TOOL_TERRAIN_NIVELATE 12
#define TOOL_TERRAIN_DOWN     13
#define TOOL_TERRAIN_TEXTURE  14

#define TOOL_PORTAL_ADD  31
#define TOOL_PORTAL_TAG  32



string messageForTool(int tool);

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

