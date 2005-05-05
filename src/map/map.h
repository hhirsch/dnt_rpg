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

class Hexagon
{

	public:

		Octagon();
		~Octagon();
		init( char * texture_fname );
		draw( SDLSurface * screen, int x, int y );
		set_position( int newX, int newY);

	private:
		char * floor_texture_fname;
		Hexagon * up, * down, * upright, * upleft, * downright, * downleft;
}

class Map
{
	
	public:
		Map();
		~Map();
		draw( SDL_Surface * screen );
		run_test();

	private:
		Hexagon * center;
}

#endif
