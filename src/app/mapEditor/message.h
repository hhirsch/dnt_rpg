#ifndef _message_h
#define _message_h

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
#define TOOL_WALL2_ADD_X            23
#define TOOL_WALL2_ADD_Z            24
#define TOOL_WALL_TEXTURE           25
#define TOOL_WALL_LESS_Y_TEXTURE    26
#define TOOL_WALL_MORE_Y_TEXTURE    27
#define TOOL_WALL_LESS_X_TEXTURE    28
#define TOOL_WALL_MORE_X_TEXTURE    29
#define TOOL_WALL_LESS_Z_TEXTURE    30
#define TOOL_WALL_MORE_Z_TEXTURE    31
#define TOOL_WALL_DESTROY           32
#define TOOL_WALL_NEXT              33
#define TOOL_WALL_PREVIOUS          34

#define TOOL_PORTAL_ADD             131
#define TOOL_PORTAL_TAG             132
#define TOOL_PORTAL_DOOR            133

#define TOOL_OBSTACLE_ADD           141

#define TOOL_PARTICLE_FIRE          151
#define TOOL_PARTICLE_SMOKE         152
#define TOOL_PARTICLE_WATERFALL     153
#define TOOL_PARTICLE_LAKE          154
#define TOOL_PARTICLE_SNOW          155
#define TOOL_PARTICLE_GRASS         156

#define TOOL_NPC_ADD                161

string messageForTool(int tool);

/*string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);*/


#endif

