#ifndef _message_h
#define _message_h

#include "../../gui/farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#define TOOL_SIM_PLAY          1
#define TOOL_SIM_PAUSE         2
#define TOOL_SIM_STOP          3
#define TOOL_POTENTIAL_ADD    10
#define TOOL_PATTERN_ADD      11
#define TOOL_GOAL_ADD         12
#define TOOL_OBJECT_ADD       13

string messageForTool(int tool);

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

