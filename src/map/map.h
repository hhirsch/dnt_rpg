#ifndef _mapa_h
#define _mapa_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

/* Que zona este código!!! Rui escreve em inglês e em português ao mesmo tempo
 * e Farrer em Português!! Viva o Esperanto!*/

/* TODO: 
 * Add support for objects
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "mapobjeto.h"
//#include "../engine/glm.h" 


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
		

	private:
		char * floor_texture_fname;
		
};

class Map
{
	
	public:
		
		/* Functions */
		
		Map();                     /* Construtor */
		~Map();                    /* Destruidor */
		int draw( );               /* Desenha o mapa */
		int open( char* arquivo ); /* Abre o mapa do arquivo */

		/* Vars */
                LmapObjeto* Objetos; 
                int numtexturas;
                GLMtexture* Texturas;

	private:
		char * name;       /* Nome do Arquivo de Mapa Carregado */
		Square * first;    /* Quadrado localizado no canto superior */
		int x,z;           /* Dimensões do mapa */
};

#endif
