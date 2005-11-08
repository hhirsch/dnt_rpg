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
#define SQUARESIZE 64
#define HALFSQUARESIZE 32
#define SQUAREDIAGONALSIZE 90.509668
#define MAXOBJETOS 10
#define MAXMUROS 5
#define MUROALTURA 50
#define MEIOFIOALTURA 2
#define ALTURAMAXIMA 100

typedef struct _muro
{
   GLfloat x1,z1,x2,z2;
   int textura;
   struct _muro* proximo;
}muro;

typedef struct _texture
{
   char* nome;
   char* arqNome;
   GLuint indice;                /* Indice da Textura internamente */
   GLuint w,h;
   GLuint R,G,B;                 /* Cores da Textura para o MINIMAPA */
   struct _texture* proximo;
}texture;


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
                GLfloat h1,h2,h3,h4; //altura de cada vertice do quadrado
                int posX, posZ;    // posicao no mapa
		Square * up, * down, * right, * left; //vizinhos
                int flags;   // bandeiras
                int visivel;
                int textura; // qual textura ele usa
		mapObjeto *objetos[MAXOBJETOS]; //objetos quele usa
                int objetosDesenha[MAXOBJETOS]; //desenha objeto n?
		int quadXobjetos[MAXOBJETOS]; // X do quadrado do objeto 
		int quadZobjetos[MAXOBJETOS]; // Z do quadrado do objeto
                int orientacaoObjetos[MAXOBJETOS];
                float Xobjetos[MAXOBJETOS]; //x do objeto n
                float Zobjetos[MAXOBJETOS]; //z do objeto n
                muro* muros[MAXMUROS];
                GLuint R,G,B;            /* Cores do Quadrado para o MINIMAPA */
                Square* quadObjetos[MAXOBJETOS];

	private:
		char* floor_texture_fname;
		
};



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
                Square* quadradoRelativo(int xa, int za);
                int save( char* arquivo ); /* Salva o Mapa */
                void optimize();


		/* Vars */
                LmapObjeto* Objetos; 
                int numtexturas;
                texture* Texturas;
                int xInic,zInic;
                Square* first; /* Quadrado localizado no canto superior */
                Square* squareInic; /* quadrado inicial */
                int x,z;       /* Dimensões do mapa */
                muro* muros;  /* Muros Presentes no Mapa */
                muro* meiosFio; /* Meios-Fio presentes no mapa */

	private:
		char* name;    /* Nome do Arquivo de Mapa Carregado */
                

};

#endif
