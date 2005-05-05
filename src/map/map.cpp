/*   DCC Nightmare - A Free 3d Role Playing Game
 *   Copyright (C) 2005  Inconha Software LTDA.
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "mapa.h"

/* !!!!!!!!!!!
 * TODO:
 * Hexagon::draw
 */


Hexagon::Hexagon()
{
	floor_texture_fname = NULL;
	up = NULL;
	down = NULL;
	upleft = NULL;
	upright = NULL;
	downleft = NULL;
	downright = NULL;
	return;
}

Hexagon::~Hexagon()
{
	if( floor_texture_name == NULL )
		free( floor_texture_fname );
	return;
}

int Hexagon::draw( SDL_Surface * screen, int x, int y )
{
	glBegin(GL_TRIANGLES);
	glVertexf( x + 10, y + 5, 0 );
	
	glEnd();
}

int Map::run_test( void )
{
	SDL_Surface * main_screen;
	SDL_Event event;
	
	// Abre a tela
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		perror("Não foi possível iniciar SDL\n",1);
	
	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1 );

    main_screen = SDL_SetVideoMode(800, 600, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);
    if ( main_screen == NULL )
       perror("Impossível ajustar o vídeo.\n");

    SDL_WM_SetCaption("Map test","");

    //Define quais eventos irao ser ignorados
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
    SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
    SDL_EventState(SDL_QUIT, SDL_IGNORE);
    SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
    SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
    SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
    SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	/* Luz no infinito, imitando o Sol */
	GLfloat light_position[] = { 0.0, 0.0, 100.0, 0.0 };
	GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	/* Define as propriedades de material */
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	/* Habilita o teste de profundidade dos poligonos */
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_FLAT);

	center.draw(main_screen);
	
	while ( SDL_WaitEvent(&event) >= 0 )
	{
		if( event.type == SDL_KEYDOWN )
			return(0);
	}
	return(0);
}

Map::draw( SDL_Surface * screen )
{
	while

}
