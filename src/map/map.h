#ifndef _mapa_h
#define _mapa_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

/* Que zona este código!!! Rui escreve em inglês e em português ao mesmo tempo
 * e Farrer em Português!! Viva o Esperanto!*/

#include "mapobjeto.h"
//#include "../engine/glm.h" 


/* Flag masks */
#define PISAVEL 1
#define OCUPADO 2

/* Constraints */
#define SQUARESIZE 10
#define HALFSQUARESIZE 5
#define MAXOBJETOS 5
#define MUROALTURA 50
#define ALTURAMAXIMA 100

class Square
{

	public:

		/* Functions: */
		Square();
		~Square();
		int init( char * texture_fname );
		int draw( GLfloat x, GLfloat y );

		/* Vars: */
                int x1,z1,x2,z2;   // dimensoes do quadrado
                int posX, posZ;    // posicao no mapa
		Square * up, * down, * right, * left; //vizinhos
                int flags;   // bandeiras
                int visivel;
                int textura; // qual textura ele usa
		mapObjeto *objetos[MAXOBJETOS]; //objetos quele usa
                int objetosDesenha[MAXOBJETOS]; //desenha objeto n?
                int Xobjetos[MAXOBJETOS]; //x do objeto n
                int Zobjetos[MAXOBJETOS]; //z do objeto n
                Square* quadObjetos[MAXOBJETOS];

	private:
		char* floor_texture_fname;
		
};

typedef struct _texture {
   char* nome;                   /* Nome do arquivo ed Textura Correspondente */
   GLuint indice;                /* Indice da Textura internamente */
   GLuint w,h;
   struct _texture* proximo;  /* Proxima Textura na Lista */
}texture;

typedef struct _muro
{
   GLuint x1,z1,x2,z2;
   int textura;
   struct _muro* proximo;
}muro;

class Map
{
	
	public:
		
		/* Functions */
		
		Map();                     /* Construtor */
		~Map();                    /* Destruidor */
                /* Desenha o Mapa */
		int draw(GLfloat cameraX, GLfloat cameraY, 
                         GLfloat cameraZ, GLfloat matriz[6][4]);
                void drawMinimap(SDL_Surface* img);
		int open( char* arquivo ); /* Abre o mapa do arquivo */
                Square* quadradoRelativo(int x, int z);

		/* Vars */
                LmapObjeto* Objetos; 
                int numtexturas;
                texture* Texturas;
                int xInic,zInic;
                Square* first; /* Quadrado localizado no canto superior */
                Square* squareInic; /* quadrado inicial */
                int x,z;       /* Dimensões do mapa */

	private:
		char* name;    /* Nome do Arquivo de Mapa Carregado */
		muro* muros;   /* Muros presentes no mapa */
                

};

#endif
