#ifndef _message_h
#define _message_h

#include "../../gui/farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;


string messageForTool(int tool);

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

