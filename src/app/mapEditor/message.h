#ifndef _message_h
#define _message_h

#include <string>
using namespace std;


#define TOOL_TERRAIN_UP       11
#define TOOL_TERRAIN_NIVELATE 12
#define TOOL_TERRAIN_DOWN     13
#define TOOL_TERRAIN_TEXTURE  14

#define TOOL_PORTAL_ADD  31
#define TOOL_PORTAL_TAG  32



string messageForTool(int tool);


#endif

