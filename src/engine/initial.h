/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#ifndef _initial_h
#define _initial_h

#include "../gui/farso.h"

/* initialScreen Returns Values */
#define EXIT_GAME     0
#define NEW_GAME      1
#define LOAD_GAME     2
#define SAVE_GAME     3
#define CONTINUE_GAME 4

/* initialScreen Status */
#define ON_INIT 1
#define IN_GAME 2

/* DccNiTghtmare's initial screen */
class initialScreen
{
   public:
     initialScreen();
     ~initialScreen();
     int Execute(int Status,GLdouble proj[16],
                           GLdouble modl[16], GLint viewPort[4],
                           GLuint* idTextura);
   private:
      interface* gui;  /* GUI for Windows */

};

#endif

