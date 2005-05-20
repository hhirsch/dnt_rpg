/* DCC Nightmare is Public Domain - Do whatever you want with this code.
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

int Square::draw( GLfloat x, GLfloat z )
{
#ifdef DEBUG_MAP
	printf("map.cpp - Square::draw(): Drawing square in (%f,%f)\n", x, z);
#endif
//	glEnable(GL_TEXTURE_2D);
	glColor3f( 1.0, 1.0, 20.0 );
	glBegin(GL_POLYGON);
	glVertex3f( x + (SQUARESIZE/2), 0.0, z + (SQUARESIZE/2) );
	glVertex3f( x - (SQUARESIZE/2), 0.0, z + (SQUARESIZE/2) );
	glVertex3f( x - (SQUARESIZE/2), 0.0, z - (SQUARESIZE/2) );
	glVertex3f( x + (SQUARESIZE/2), 0.0, z - (SQUARESIZE/2) );
//	glNormal3i( 0, 0, 1 );
	glEnd();
	return(0);
}

int Map::draw()
{
	Square * ref = first;
        Square * aux = first;
	GLfloat x = 0.0, y = 0.0;
#ifdef DEBUG_MAP
	printf("map.cpp - Map::draw(): Drawing map %s.\n", name);
#endif
	while( aux != NULL )
	{
           (*aux).draw(x,y);
           aux = aux->right;
           if(aux == NULL)   //chegou ao fim da linha
           {
              aux = ref->down;
              ref = ref->down;
              y += SQUARESIZE;
              x = 0.0;
           } 
           else      //senao continua na linha
           {
              x += SQUARESIZE;
           } 
        }
	return(0);

}

/* Construidor */

Map::Map()
{
	first = NULL;
	name = NULL;
}

int Map::open(char* arquivo)
{
	/*first = new(Square);
	(*center).up = new(Square);
	(*center).down = new(Square);
	(*center).left = new(Square);
	(*center).right = new(Square);
	name = "2014";*/
	return(0);
}

/* Destruidor */

Map::~Map()
{
}
