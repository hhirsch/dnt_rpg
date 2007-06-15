#ifndef _messages_h
#define _messages_h

#include "farso.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

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
 * \param opt1 -> Option 1
 * \param opt2 -> Option 2
 * \param proj -> projection matrix
 * \param modl -> model view matrix
 * \param viewPort -> viewPort vector
 * \return the value of the option selected (1 or 2) */
int getOptionFromUser(string title, string message, string opt1, string opt2,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

/*! Show a message to user
 * \param title -> title of the window
 * \param message -> message to show to user
 * \param proj -> projection matrix
 * \param modl -> model view matrix
 * \param viewPort -> viewPort vector */
void showMessage(string title, string message,
                 GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);


#endif

