#ifndef _mapa_h
#define _mapa_h

/*   DCC Nightmare is Public Domain - Do whatever you want with this code.
 */

#include "mapobjeto.h"
#include "mapfog.h"
#include "maplights.h"
#include <string>
using namespace std;

/* Flag masks */
#define PISAVEL 1

/* Constraints */
#define SQUARESIZE             64
#define HALFSQUARESIZE         32
#define SQUAREDIAGONALSIZE     90.509668
#define MAXOBJETOS             10
#define MAXMUROS                8
#define MUROALTURA             50
#define MEIOFIOALTURA           2
#define ALTURAMAXIMA          100

/****************************************************
 *               Map's Connections                  *
 ****************************************************/
typedef struct _conection
{
   bool active;          /* Active Conection */
   GLfloat x1,z1,x2,z2;  /* Map Position */
   string mapName;       /* Map filename */ 
}conection;

/****************************************************
 *                  Map's Walls                     *
 ****************************************************/
typedef struct _muro
{
   GLfloat x1,z1,x2,z2;   /* Coordenates */
   GLuint dX,dY,dZ;       /* Delta X, Y & Z of texture cycle */
   int textura;           /* Texture ID */
   struct _muro* proximo; /* Next on list */
}muro;

/****************************************************
 *               Map's Textures                     *
 ****************************************************/
typedef struct _texture
{
   string nome;              /* Name */
   string arqNome;           /* File Name */
   GLuint indice;            /* Texture ID */
   GLuint w,h;               /* Dimmensions */
   GLuint R,G,B;             /* Colors to MINIMAP */
   struct _texture* proximo; /* Next on List */
}texture;

/****************************************************
 *                  Map's Doors                     *
 ****************************************************/
typedef struct _door
{
  mapObjeto* objeto;       /* pointer to door map object */
  GLfloat x,z;             /* position on map */
  GLint status;            /* actual status (opened, closed) */
  GLint orientacao;        /* orientation */
  struct _door* proximo;   /* pointer to next door on map */
}door;


/****************************************************
 *                  Map's Square                    *
 ****************************************************/
class Square
{
   public:
      /* Functions: */
      Square();         /* Constructor */
      ~Square();        /* Destructor */

      /* Vars: */
      int x1,z1,x2,z2;                  /* Coordinates */
      GLfloat h1,h2,h3,h4;              /* Vertice's Height */
      int posX, posZ;                   /* Map positions */
      int flags;                        /* Condition flag */
      int visivel;                      /* Visible on active frame ? */
      int textura;                      /* Actual Texture */
      mapObjeto *objetos[MAXOBJETOS];   /* Objects on Square */
      int objetosDesenha[MAXOBJETOS];   /* Draw object on active frame ? */
      int quadXobjetos[MAXOBJETOS];     /* Object Square X coordinate */ 
      int quadZobjetos[MAXOBJETOS];     /* Object Square Z coordinate */
      int orientacaoObjetos[MAXOBJETOS];/* Object Orientation */
      float Xobjetos[MAXOBJETOS];       /* Object X coordinate */
      float Zobjetos[MAXOBJETOS];       /* Object Z coordinate */
      muro* muros[MAXMUROS];            /* Square walls on */
      GLuint R,G,B;                     /* Square Color to MINIMAP */
      Square* quadObjetos[MAXOBJETOS];  /* Orign object from square: */
      conection mapConection;           /* Conection to other map */
      int statusObj[MAXOBJETOS];        /* Current Status of Object */
      int pisavelObj[MAXOBJETOS];       /* Object collision or not */
};



class Map
{
   public:
      /* Functions */
      Map();                     /* Construtor */
      ~Map();                    /* Destruidor */

      /*************************************************************** 
       * Reason: Draw on screen the visible map (using view culling)  
       * Param:
       *        cameraX -> X coordinate of camera
       *        cameraY -> Y coordinate of camera
       *        cameraZ -> Z coordinate of camera
       *        matriz  -> view frustum matriz
       ***************************************************************/
      int draw(GLfloat cameraX, GLfloat cameraY, 
               GLfloat cameraZ, GLfloat matriz[6][4]);
      int drawFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
              GLfloat matriz[6][4]);
      /*************************************************************** 
       * Reason: Draw Minimap, relative to current map and character
       *         position.
       * Param:
       *        img -> Surface where minimap will be draw. Usualy a
       *               valid window surface.
       ***************************************************************/
      void drawMinimap(SDL_Surface* img);
      /*************************************************************** 
       * Reason: Create a new empty map, with new structs.
       * Param:
       *         X -> number of squares on X coordinate
       *         Z -> number of squares on Z coordinate
       ***************************************************************/
      void newMap(int X,int Z);
      /*************************************************************** 
       * Reason: Opens map from file
       * Param:
       *        arquivo ->  Name of file to be opened
       ***************************************************************/
      int open( string arquivo ); 
      /*************************************************************** 
       * Reason: Returns coordinate relative square
       * Param:
       *         xa, za -> coordinate of square
       ***************************************************************/
      Square* quadradoRelativo(int xa, int za);
      /*************************************************************** 
       * Reason: Save map to file
       * Param:
       *         arquivo -> file name to be saved
       ***************************************************************/
      int save( string arquivo ); 
      /*************************************************************** 
       * Reason: Optimize Map Structs
       * Param:
       *        NULL
       ***************************************************************/
      void optimize();

      /* Vars */
      LmapObjeto* Objetos;  /* Map's objects list */
      mapFog fog;           /* Map's Fog */
      mapLights lights;     /* Map's Lights */
      int numtexturas;      /* Number of distint Textures on Map */
      texture* Texturas;    /* List of textures on Map */
      int xInic,zInic;      /* Coordinate where PCs starts */
      Square* squareInic;   /* Square where PCs starts */
      int x,z;              /* Map Dimensions */
      muro* muros;          /* Map Walls */
      muro* meiosFio;       /* Map Meio Fios (how translate this?) */
      door* portas;         /* Map Doors */
      string music;         /* Map Music */
      string name;          /* File name of loaded map */
      string npcFileName;   /* Static NPC's on map filename */

      private:
         Square*** MapSquares; /* Internal squares of Map */
};

#endif
