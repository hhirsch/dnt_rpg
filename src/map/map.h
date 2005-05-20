#ifndef _mapa_h
#define _mapa_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
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

/* Constraints */
#define SQUARESIZE 0.4

class Square
{

	public:

		/* Functions: */
		Square();
		~Square();
		int init( char * texture_fname );
		int draw( GLfloat x, GLfloat y );

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
		
		/* Functions */
		
		Map();
		~Map();
		int draw( );
		int open( char* arquivo );

		/* Vars */

	private:
		char * name;
		Square * first;
};

#endif
