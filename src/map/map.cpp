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

#include "map.h"

Square::Square()
{
	floor_texture_fname = NULL;
	up = NULL;
	down = NULL;
	left = NULL;
	right = NULL;
	flags = 0;
	return;
}

Square::~Square()
{
	if( floor_texture_fname == NULL )
		free( floor_texture_fname );
	return;
}

int Square::draw( GLfloat x, GLfloat y )
{
#ifdef DEBUG_MAP
	printf("map.cpp - Square::draw(): Drawing square in (%f,%f)\n", x, y);
#endif
//	glEnable(GL_TEXTURE_2D);
	glColor3f( 0.0, 0.0, 0.0 );
	glBegin(GL_POLYGON);
	glVertex3f( x + (SQUARESIZE/2), y + (SQUARESIZE/2), 0.0 );
	glVertex3f( x - (SQUARESIZE/2), y + (SQUARESIZE/2), 0.0 );
	glVertex3f( x - (SQUARESIZE/2), y - (SQUARESIZE/2), 0.0 );
	glVertex3f( x + (SQUARESIZE/2), y - (SQUARESIZE/2), 0.0 );
//	glNormal3i( 0, 0, 1 );
	glEnd();
	return(0);
}

int Map::run_test( void )
{
	SDL_Surface * main_screen;
	SDL_Event event;

#ifdef DEBUG_MAP
	printf("map.cpp - Map::run_test(): Starting test\n");
#endif
	
	// Abre a tela
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
		perror("Não foi possível iniciar SDL\n");

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

	glClearColor (0.1, 0.1, 0.1, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
//	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
/*	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);*/

/*	GLfloat mat_ambient[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// Luz no infinito, imitando o Sol
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	// Define as propriedades de material
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Habilita o teste de profundidade dos poligonos
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_FLAT); */
	
//	glLoadIdentity();
	//gluLookAt( 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 );
	(*this).draw();
	
/*	glBegin(GL_POLYGON);
	glColor3f( 255.0, 255.0, 255.0 );
	glVertex4f( 1.0, 1.0, 0.0, 0.0 );
	glVertex4f( -1.0, -1.0, 0.0, 0.0 );
	glVertex4f( -1.0, 1.0, 0.0, 0.0 );
	glVertex4f( 1.0, -1.0, 0.0, 0.0 );
//	glNormal3i( 0, 0, 1 );
	glEnd();*/

	glFlush();
	SDL_GL_SwapBuffers();
	fflush(NULL);

#ifdef DEBUG_MAP
	printf("map.cpp - Map::run_test(): Going into main loop\n");
#endif

	while ( SDL_WaitEvent(&event) >= 0 )
	{
		if( event.type == SDL_KEYDOWN )
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:
					return(0);
					break;
				case SDLK_r:
					glLoadIdentity();
					(*this).draw();
					glFlush();
					SDL_GL_SwapBuffers();
					fflush(NULL);

					break;
				default:
					printf("Tecla invalida pressionada.\n");
			}
	}
	return(0);
}

int Map::draw()
{
	Square * ref = center, * aux = center;
	GLfloat x = 0.0, y = 0.0;
#ifdef DEBUG_MAP
	printf("map.cpp - Map::draw(): Drawing map %s.\n", name);
#endif
	while( ref != NULL )
	{
		while( aux != NULL )
		{
			(*aux).draw( x, y );
			y += SQUARESIZE;
			aux = (*aux).up;
		}

		y = -SQUARESIZE;
		aux = (*ref).down;

		while( aux != NULL )
		{
			(*aux).draw( x, y );
			y -= SQUARESIZE;
			aux = (*aux).down;
		}
		ref = (*ref).left;
		aux = ref;
		x -= SQUARESIZE; y = 0.0;
	}

	ref = (*center).right;
	x = SQUARESIZE; y = 0.0;

	while( ref != NULL )
	{
		while( aux != NULL )
		{
			(*aux).draw( x, y );
			y += SQUARESIZE; 
			aux = (*aux).up;
		}

		y = 0.0;
		aux = ref;

		while( aux != NULL )
		{
			(*aux).draw( x, y );
			y -= SQUARESIZE;
			aux = (*aux).down;
		}
		ref = (*ref).right;
		aux = ref;
		x += SQUARESIZE; y = 0.0;
	}

	return(0);

}

/* Construidor */

Map::Map()
{
	center = NULL;
	name = NULL;
}

int Map::open()
{
	center = new(Square);
	(*center).up = new(Square);
	(*center).down = new(Square);
	(*center).left = new(Square);
	(*center).right = new(Square);
	name = "2014";
	return(0);
}

/* Destruidor */

Map::~Map()
{
}
