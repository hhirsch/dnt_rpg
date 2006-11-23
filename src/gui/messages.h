#ifndef _messages_h
#define _messages_h

#include "farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

void showMessage(string title, string message,
                 GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

