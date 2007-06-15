/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _farso_h
#define _farso_h

#include <stdlib.h>
#include <SDL/SDL.h>
#include "draw.h"
#include "button.h"
#include "color.h"
#include "cxsel.h"
#include "janela.h"
#include "eventos.h"
#include "fonte.h"
#include "healthBar.h"
#include "textbox.h"
#include "rolbar.h"
#include "seltext.h"
#include "textbar.h"
#include "guiobject.h"
#include "lista.h"
#include "picture.h"
#include "menu.h"
#include "mouse.h"
#include "tabbutton.h"
#include "messages.h"

/*! Init the SDL/OpenGL Context
 * \param screen -> pointer to the pointer to the surface used as screen
 * \param title -> title of the screen */
void Farso_Init(SDL_Surface **screen, string title);

/*! End the SDL/OpenGL Context
 * \param screen -> surface used as screen */
void Farso_End(SDL_Surface *screen);


#endif

