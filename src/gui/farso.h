/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _farso_h
#define _farso_h


#include <libintl.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "draw.h"
#include "button.h"
#include "color.h"
#include "cxsel.h"
#include "window.h"
#include "interface.h"
#include "dntfont.h"
#include "healthBar.h"
#include "textbox.h"
#include "rolbar.h"
#include "seltext.h"
#include "textbar.h"
#include "guiobject.h"
#include "guilist.h"
#include "picture.h"
#include "menu.h"
#include "mouse.h"
#include "tabbutton.h"
#include "messages.h"

/*! Init the SDL/OpenGL Context
 * \param screen -> pointer to the pointer to the surface used as screen
 * \param title -> title of the screen
 * \param width -> screen width
 * \param height -> screen height
 * \param fullScreen -> true if is fullscreen
 * \param antiAliasingSample -> number of Antialising used (0 == disabled) */
void Farso_Init(SDL_Surface **screen, string title, int width, int height,
                bool fullScreen, int antiAliasingSample);

/*! Define the SDL/OpenGL Context Resolution
 * \param screen -> pointer to the pointer to the surface used as screen
 * \param title -> title of the screen
 * \param width -> screen width
 * \param height -> screen height
 * \param fullScreen -> true if is fullscreen
 * \param antiAliasingSample -> number of Antialising used (0 == disabled) */
void Farso_DefineResolution(SDL_Surface **screen, string title, 
                            int width, int height,
                            bool fullScreen, int antiAliasingSample);

/*! End the SDL/OpenGL Context
 * \param screen -> surface used as screen */
void Farso_End(SDL_Surface *screen);


#endif

