/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */


#ifndef _farso_h
#define _farso_h

#include <stdlib.h>
#include <SDL/SDL.h>
#include "desenho.h"
#include "botao.h"
#include "cores.h"
#include "cxsel.h"
#include "janela.h"
#include "eventos.h"
#include "erro.h"
#include "fonte.h"
#include "healthBar.h"
#include "quadtexto.h"
#include "rolbar.h"
#include "seltexto.h"
#include "textbar.h"
#include "guiobject.h"
#include "lista.h"
#include "figura.h"
#include "menu.h"
#include "mouse.h"
#include "tabbotao.h"
#include "messages.h"

void Farso_Iniciar(SDL_Surface **screen, char* titulo);
void Farso_Terminar(SDL_Surface *screen);


#endif

