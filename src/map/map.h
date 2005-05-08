#ifndef _mapa_h
#define _mapa_h

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

/* TODO: 
 * Add support for objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL.h>

/* Flag masks */
#define PISAVEL 1
#define OCUPADO 2

class Square
{

	public:

		/* Functions: */
		Square();
		~Square();
		int init( char * texture_fname );
		int draw( int x, int y );
		int set_position( int newX, int newY);

		/* Vars: */
		Square * up, * down, * right, * left;
		int flags;
		/* Personagem * Ocupador; */
		/* Ojeto * Objeto; */

	private:
		char * floor_texture_fname;
		
};

class Map
{
	
	public:
		Map();
		~Map();
		int draw( SDL_Surface * screen );
		int run_test();

	private:
		Square * center;
};

#endif
