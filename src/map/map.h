#ifndef _mapa_h
#define _mapa_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "mapobjeto.h"
#include "mapfog.h"
#include "maplights.h"
#include <string>
using namespace std;

/* Flag masks */
#define PISAVEL 1   /**< If a Square is walkable or not. */

/* Constraints */
#define SQUARESIZE             64 /**< Size of the Square */
#define HALFSQUARESIZE         32 /**< Half size of the square */
#define SQUAREDIAGONALSIZE     90.509668 /**< Diagonal size of the square */
#define MAXOBJETOS             10 /**< Max number of objects per square */
#define MAXMUROS                8 /**< Max number of walls per square */
#define MUROALTURA             50 /**< Walls height */
#define MEIOFIOALTURA           2 /**< Meio-fios height */
#define ALTURAMAXIMA          100 /**< Max square height */

/*!
 ****************************************************
 *               Map's Connections                  *
 ****************************************************/
typedef struct _conection
{
   bool active;          /**< Active Conection */
   GLfloat x1,z1,x2,z2;  /**< Map Position */
   string mapName;       /**< Map filename */ 
}conection;

/*!
 ****************************************************
 *                  Map's Walls                     *
 ****************************************************/
typedef struct _muro
{
   GLfloat x1,   /**< X min coordinates */
           z1,   /**< Z min coordinates */
           x2,   /**< X max Coordinates */
           z2;   /**< Z max Coordinates */
   GLuint dX,    /**< Delta X of texture cycle */
          dY,    /**< Delta Y of texture cycle */
          dZ;    /**< Delta Z of texture cycle */
   int textura;  /**< Texture ID */
   struct _muro* proximo; /**< Next on list */
}muro;

/*!
 ****************************************************
 *               Map's Textures                     *
 ****************************************************/
typedef struct _texture
{
   string nome;              /**< Name */
   string arqNome;           /**< File Name */
   GLuint indice;            /**< Texture ID */
   GLuint w,h;               /**< Dimmensions */
   GLuint R,G,B;             /**< Colors to MINIMAP */
   struct _texture* proximo; /**< Next on List */
}texture;

/*!
 ****************************************************
 *                  Map's Doors                     *
 ****************************************************/
typedef struct _door
{
  mapObjeto* objeto;       /**< pointer to door map object */
  GLfloat x,z;             /**< position on map */
  GLint status;            /**< actual status (opened, closed) */
  GLint orientacao;        /**< orientation */
  struct _door* proximo;   /**< pointer to next door on map */
}door;


/*!
 ****************************************************
 *                  Map's Square                    *
 ****************************************************/
class Square
{
   public:
      Square();         /**< Constructor */
      ~Square();        /**< Destructor */

      int x1,z1,x2,z2;                  /**< Coordinates */
      GLfloat h1,h2,h3,h4;              /**< Vertice's Height */
      int posX, posZ;                   /**< Map positions */
      int flags;                        /**< Condition flag */
      int visivel;                      /**< Visible on active frame ? */
      int textura;                      /**< Actual Texture */
      mapObjeto *objetos[MAXOBJETOS];   /**< Objects on Square */
      int objetosDesenha[MAXOBJETOS];   /**< Draw object on active frame ? */
      int quadXobjetos[MAXOBJETOS];     /**< Object Square X coordinate */ 
      int quadZobjetos[MAXOBJETOS];     /**< Object Square Z coordinate */
      int orientacaoObjetos[MAXOBJETOS];/**< Object Orientation */
      float Xobjetos[MAXOBJETOS];       /**< Object X coordinate */
      float Zobjetos[MAXOBJETOS];       /**< Object Z coordinate */
      muro* muros[MAXMUROS];            /**< Square walls on */
      GLuint R,G,B;                     /**< Square Color to MINIMAP */
      Square* quadObjetos[MAXOBJETOS];  /**< Orign object from square: */
      conection mapConection;           /**< Conection to other map */
      int statusObj[MAXOBJETOS];        /**< Current Status of Object */
      int pisavelObj[MAXOBJETOS];       /**< Object collision or not */
};


/*! Map Class */
class Map
{
   public:
      Map();                     /**< Construtor */
      ~Map();                    /**< Destruidor */

      /*!
       *************************************************************** 
       * Draw on screen the visible map (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \return 1 if success.
       ***************************************************************/
      int draw(GLfloat cameraX, GLfloat cameraY, 
               GLfloat cameraZ, GLfloat matriz[6][4]);
      /*!
       *************************************************************** 
       * Draw the floor on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \return 1 if success.
       ***************************************************************/
      int drawFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
              GLfloat matriz[6][4]);
      /*!
       *************************************************************** 
       * Draw Minimap, relative to current map and character position.
       * \param img -> Surface where minimap will be draw. Usualy a
       *               valid window surface.
       ***************************************************************/
      void drawMinimap(SDL_Surface* img);
      /*!
       *************************************************************** 
       * Create a new empty map, with new structs.
       * \param X -> number of squares on X coordinate
       * \param Z -> number of squares on Z coordinate
       ***************************************************************/
      void newMap(int X,int Z);
      /*!
       *************************************************************** 
       * Opens map from file
       * \param arquivo ->  Name of file to be opened
       * \return 1 on success.
       ***************************************************************/
      int open( string arquivo ); 
      /*!
       *************************************************************** 
       * Gets coordinate relative square
       * \param xa -> x coordinate of square
       * \param za -> z coordinate of square
       * \return the coordinate relative square.
       ***************************************************************/
      Square* quadradoRelativo(int xa, int za);
      /*!
       *************************************************************** 
       * Save map to file
       * \param arquivo -> file name to be saved
       * \return -> 1 on success.
       ***************************************************************/
      int save( string arquivo ); 
      /*!
       *************************************************************** 
       * Optimize Map Structs
       ***************************************************************/
      void optimize();

      LmapObjeto* Objetos;  /**< Map's objects list */
      mapFog fog;           /**< Map's Fog */
      mapLights lights;     /**< Map's Lights */
      int numtexturas;      /**< Number of distint Textures on Map */
      texture* Texturas;    /**< List of textures on Map */
      int xInic,            /**< X coordinate where PCs starts */
          zInic;            /**< Z Coordinate where PCs starts */
      Square* squareInic;   /**< Square where PCs starts */
      int x,                /**< Map X dimension */
          z;                /**< Map Z Dimension */
      muro* muros;          /**< Map Walls */
      muro* meiosFio;       /**< Map Meio Fios (how translate this?) */
      door* portas;         /**< Map Doors */
      string music;         /**< Map Music */
      string name;          /**< File name of loaded map */
      string particlesFileName; /**< File Name of Map Particles Systens */
      string npcFileName;   /**< Static NPC's on map filename */

      int SQUAREMINISIZE;   /**< Minimap square size */
      int SQUAREMINIDIV;    /**< MiniMap square division relation */


      private:
         Square*** MapSquares; /**< Internal Map squares */
};

#endif
