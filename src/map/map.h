#ifndef _mapa_h
#define _mapa_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "mapobjeto.h"
#include "mapfog.h"
#include "maplights.h"
#include "maproad.h"
#include <string>
using namespace std;

/* Flag masks */
#define PISAVEL 1   /**< If a Square is walkable or not. */

/* Constraints */
#define SQUARESIZE             64 /**< Size of the Square */
#define HALFSQUARESIZE         32 /**< Half size of the square */
#define QUARTERSQUARESIZE      16 /**< Quarter size of the square */
#define SQUAREDIAGONALSIZE     90.509668 /**< Diagonal size of the square */
#define MAXOBJETOS             10 /**< Max number of objects per square */
#define MAXMUROS                8 /**< Max number of walls per square */
#define MUROALTURA             50 /**< Walls height */
#define MEIOFIOALTURA           2 /**< Meio-fios height */
#define ALTURAMAXIMA          100 /**< Max square height */

#define SQUARE_DIVISIONS        8

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
  mapObject* object;       /**< pointer to door map object */
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

      GLfloat x1,z1,x2,z2;              /**< Coordinates */
      GLfloat h1,h2,h3,h4;              /**< Vertice's Height */
      int posX, posZ;                   /**< Map positions */
      int flags;                        /**< Condition flag */
      int visivel;                      /**< Visible on active frame ? */
      int textura;                      /**< Actual Texture */
      mapObject *objects[MAXOBJETOS];   /**< Objects on Square */
      int objectsDesenha[MAXOBJETOS];   /**< Draw object on active frame ? */
      int quadXobjects[MAXOBJETOS];     /**< Object Square X coordinate */ 
      int quadZobjects[MAXOBJETOS];     /**< Object Square Z coordinate */
      int objectsOrientation[MAXOBJETOS];/**< Object Orientation */
      float Xobjects[MAXOBJETOS];       /**< Object X coordinate */
      float Zobjects[MAXOBJETOS];       /**< Object Z coordinate */
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
       * \param mdlList -> Game models list
       * \return 1 on success.
       ***************************************************************/
      int open( string arquivo, modelList& mdlList ); 
      /*!
       *************************************************************** 
       * Gets coordinate relative square
       * \param xa -> x coordinate of square
       * \param za -> z coordinate of square
       * \return the coordinate relative square.
       ***************************************************************/
      Square* relativeSquare(int xa, int za);
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

      /*!
       ***************************************************************
       * Get Height Value for Map position
       * \param nx -> map X position
       * \param nz -> map Z position
       * \return height of the map on nx,nz position
       ***************************************************************/
       GLfloat getHeight(GLfloat nx, GLfloat nz);
       GLfloat getHeight(GLfloat nx, GLfloat nz, Square* saux);

       /*!
        *************************************************************
        * Draw an texture (surface) on map
        * \param image -> openGL texture ID
        * \param xa -> first x position
        * \param za -> first z position
        * \param xb -> last x position
        * \param zb -> last z position
        * \param sumY -> value to sum at Y height of the map
        *************************************************************/
       void drawSurfaceOnMap(GLuint image, GLfloat xa, GLfloat za, 
                             GLfloat xb, GLfloat zb, GLfloat sumY);

      /*!
       ***************************************************************
       *  Remove Object from Map
       *  \param xObj -> x Object Coordinate
       *  \param zObj -> z Object Coordinate
       *  \param obj -> pointer to map Object
       ***************************************************************/
      void removeObject(GLfloat xObj, GLfloat zObj, object* obj);
      /*!
       ***************************************************************
       *  Remove Object from Map
       *  \param xObj -> x Object Coordinate
       *  \param zObj -> z Object Coordinate
       *  \param fileName -> fileName of the object to remove
       ***************************************************************/
      void removeObject(GLfloat xObj, GLfloat zObj, string fileName);

      /*! 
       **************************************************************
       * Insert Object on Map
       * \param xReal -> xPosition to insert
       * \param zReal -> zPosition to insert
       * \param orObj -> object orientation value
       * \param obj -> pointer to mapObject
       * \param qx -> square internal X (in Squares)
       * \param qz -> square internal Z (in squares)
       **************************************************************/
      void insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                   mapObject* obj, int qx, int qz);

      /*!
       ************************************************************* 
       * Insert object on map list
       * \param arquivo -> filename to load
       * \param mdlList -> modelList
       * \return pointer to the map Object created 
       *************************************************************/
      mapObject* insertMapObject(string arquivo, modelList& mdlList);

      /*! 
       **************************************************************
       * Get Map File Name
       * \return map fileName
       **************************************************************/
      string getFileName();

      /*! 
       **************************************************************
       * Set Map File Name
       * \param fileName -> New fileName
       **************************************************************/
      void setFileName(string fileName);

      /*! 
       **************************************************************
       * Set NPC File Name
       * \return NPC fileName
       **************************************************************/
      string getNpcFileName();

      /*! 
       **************************************************************
       * Set NPC File Name
       * \param fileName -> New fileName
       **************************************************************/
      void setNpcFileName(string fileName);

      /*! 
       **************************************************************
       * Get Music File Name
       * \return music fileName
       **************************************************************/
      string getMusicFileName();

      /*! 
       **************************************************************
       * Set Music File Name
       * \param fileName -> New fileName
       **************************************************************/
      void setMusicFileName(string fileName);

      /*! 
       **************************************************************
       * Get Particles File Name
       * \return particles fileName
       **************************************************************/
      string getParticlesFileName();

      /*! 
       **************************************************************
       * Set Particles File Name
       * \param fileName -> New fileName
       **************************************************************/
      void setParticlesFileName(string fileName);

      /*! 
       **************************************************************
       * Get Map Size X
       * \return Map X size on squares
       ***************************************************************/
      int getSizeX();
      /*! 
       **************************************************************
       * Set Map Size X
       * \param sizeX -> Map X size on squares
       ***************************************************************/
      void setSizeX(int sizeX);
      /*! 
       **************************************************************
       * Get Map Size X
       * \return Map X size on squares
       ***************************************************************/
      int getSizeZ();
      /*! 
       **************************************************************
       * Set Map Size Z
       * \param sizeZ -> Map Z size on squares
       ***************************************************************/
      void setSizeZ(int sizeZ);
      /*! 
       **************************************************************
       * get Initial Position
       * \param iX -> initial X position
       * \param iZ -> initial Z position
       ***************************************************************/
      void getInitialPosition(float& iX, float& iZ);


      mapFog fog;           /**< Map's Fog */
      mapLights lights;     /**< Map's Lights */
      mapRoad* roads;       /**< Map's Roads */
      int numtexturas;      /**< Number of distinct Textures on Map */
      texture* Texturas;    /**< List of textures on Map */
      Square* squareInic;   /**< Square where PCs starts */
      muro* muros;          /**< Map Walls */
      muro* meiosFio;       /**< Map Meio Fios (how translate this?) */
      door* portas;         /**< Map Doors */

      int SQUAREMINISIZE;   /**< Minimap square size */
      int SQUAREMINIDIV;    /**< MiniMap square division relation */

      protected:
         int x,                /**< Map X dimension (in squares) */
             z;                /**< Map Z Dimension (in squares) */
         Square*** MapSquares; /**< Internal Map squares */
         lMapObject* objects;  /**< Map's objects list */
         string music;         /**< Map Music */
         string name;          /**< File name of loaded map */
         string particlesFileName; /**< File Name of Map Particles Systens */
         string npcFileName;   /**< Static NPC's on map filename */
         int xInic,            /**< X coordinate where PCs starts */
             zInic;            /**< Z Coordinate where PCs starts */
};

#endif
