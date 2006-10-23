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
#define TOOL_OBSTACLE_ADD     13

/* TP3 */
#define TOOL_TP3_BRIEF_ADD    20
#define TOOL_TP3_POLICE_ADD   21
#define TOOL_TP3_ROGUE_ADD    22
#define TOOL_TP3_PSDB_ADD     23
#define TOOL_TP3_PT_ADD       24
#define TOOL_TP3_PFL_ADD      25
#define TOOL_TP3_CONGRESS_ADD 26
#define TOOL_TP3_FEDERAL_ADD  27

string messageForTool(int tool);

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

