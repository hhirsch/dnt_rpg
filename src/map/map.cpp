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
