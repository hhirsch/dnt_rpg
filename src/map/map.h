#ifndef _mapa_h
#define _mapa_h

/*************************************************************************
 * DccNiTghtmare is Public Domain - Do whatever you want with this code. *
 *************************************************************************/

#include "mapobject.h"
#include "mapfog.h"
#include "maplights.h"
#include "maproad.h"
#include <string>
using namespace std;

/* Flag masks */
#define PISAVEL 1   /**< If a Square is walkable or not. */

/* Constraints */
#define OUTDOOR_SQUARE_SIZE      256      /**< Size of the Outdoor Square */
#define INDOOR_SQUARE_SIZE        64      /**< Size of the indoor square */


//#define HALF_SQUARE_SIZE SQUARE_SIZE / 2 /**< Half size of the square */
//#define QUARTER_SQUARE_SIZE SQUARE_SIZE / 4 /**< Quarter size of the square */
//#define SQUARE_DIAGONAL_SIZE SQUARE_SIZE * 1.4142136 /**< Diagonal squaresize */

#define MAX_WALLS             15       /**< Max number of walls per square */
#define WALL_HEIGHT           50       /**< Walls height */
#define CURB_HEIGHT            2       /**< Curbs height */
#define MAX_HEIGHT           150       /**< Max square height */

#define SQUARE_DIVISIONS_INC  256 /**< Difference heigh to inc the square divisions. NOT used anymore. */ 
#define TEXTURE_REPEATS (OUTDOOR_SQUARE_SIZE / 256) /**< Number of Repeats */ 
#define ALPHA_TEXTURE_INC       4 /**< Points per square on each alpha texture*/


/*!
 ****************************************************
 *               Map's Connections                  *
 ****************************************************/
class conection
{
   public:
      bool active;          /**< Active Conection */
      GLfloat x1,z1,x2,z2;  /**< Map Position */
      string mapName;       /**< Map filename */ 
};

/*!
 ****************************************************
 *                  Map's Walls                     *
 ****************************************************/
class wall
{
   public:
      GLfloat x1,   /**< X min coordinates */
              z1,   /**< Z min coordinates */
              x2,   /**< X max Coordinates */
              z2;   /**< Z max Coordinates */
      GLuint dX,    /**< Delta X of texture cycle */
             dY,    /**< Delta Y of texture cycle */
             dZ;    /**< Delta Z of texture cycle */
      int texture;  /**< Texture ID */
      wall* next; /**< Next on list */
};

/*!
 ****************************************************
 *               Map's Textures                     *
 ****************************************************/
class texture
{
   public:
      string name;              /**< Name */
      string fileName;          /**< File Name */
      GLuint index;             /**< Texture ID */
      GLuint w,h;               /**< Dimmensions */
      GLuint R,G,B;             /**< Colors to MINIMAP */
      GLuint alphaTexture;      /**< The Alpha Texture */
      int count;                /**< The count of the texture */
      float** alphaValues;      /**< The Alpha Values Matrix */
      bool definedAlpha;        /**< If the alpha is defined */
      texture* next;    /**< Next on List */
};

/*!
 ****************************************************
 *                  Map's Doors                     *
 ****************************************************/
class door
{
   public:
      object* obj;             /**< pointer to door object */
      GLfloat x,z;             /**< position on map */
      GLint status;            /**< actual status (opened, closed) */
      GLint orientation;       /**< orientation */
      door* next;      /**< pointer to next door on map */
};

/*!
 *****************************************************
 *                  Object at Square                 *
 *****************************************************/
class objSquare
{
   public:
      bool draw;           /**< If is to draw the object (true if
                                the object orign is in this square,
                                false if the object orign is on other 
                                square. This is used to avoid drawning
                                more than one time the object, if it is on
                                more than one square at a time).*/
      int squareX;         /**< The square X coordinate of the object */
      int squareZ;         /**< The square Z coordinate of the object */
      int orientation;     /**< The orientation angle of the object */
      float x;             /**< The X position of the object on the map */
      float z;             /**< The Z position of the object on the map */
      int status;          /**< The status of the object */
      bool colision;       /**< If the collision is enable to this object */
      object* obj;         /**< The pointer to the object used */

      objSquare* next;     /**< Next object on the list */
      objSquare* previous; /**< Previous object on the list */
};

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
      int visible;                      /**< Visible on active frame ? */
      int texture;                      /**< Actual Texture */
      wall* walls[MAX_WALLS];           /**< Square walls on */
      GLuint R,G,B;                     /**< Square Color to MINIMAP */
      conection mapConection;           /**< Conection to other map */
      int divisions;                    /**< Number of Divisions */

      /*! Set the Number of Divisions, based on Square Heights */
      void setDivisions();

      /*! Add object to the square */
      objSquare* addObject(bool draw, int squareX, int squareZ, int orientation,
                           float x, float z, bool colision, object* obj);

      /*! Remove Object from the square */
      void removeObject(objSquare* obj);
   
      /*! Get the first object on the list
       * \return pointer to the first objSquare on the list */
      objSquare* getFirstObject();

      /*! Get the number of objects on the list
       * \return number of objects on the list */
      int getTotalObjects();

   protected:
      objSquare* objList;     /**< List of Objects on the square */
      int totalObjects;       /**< Total objects on the list */

};

/*! Map Class */
class Map
{
   public:
      Map(lObject* lObjects);    /**< Construtor */
      ~Map();                    /**< Destruidor */


      /*!
       ***************************************************************
       * Gets the square size on the map 
       * \return-> the square size used on the map 
       ***************************************************************/
       int squareSize(){if(outdoor) return(OUTDOOR_SQUARE_SIZE);
                        else return(INDOOR_SQUARE_SIZE);};

      /*!
       *************************************************************** 
       * Draw on screen the visible map (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param perX -> active character X position
       * \param perZ -> active character Z position
       * \return 1 if success.
       ***************************************************************/
      int draw(GLfloat cameraX, GLfloat cameraY, 
               GLfloat cameraZ, GLfloat matriz[6][4],
               GLfloat perX, GLfloat perZ);
      /*!
       *************************************************************** 
       * Draw the floor on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \return 1 if success.
       ***************************************************************/
      void drawFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                     GLfloat matriz[6][4]);
      /*!
       *************************************************************** 
       * Draw the map objects on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param inverted -> usefull for reflections
       ***************************************************************/
      void drawObjects(GLfloat cameraX, GLfloat cameraY, 
                       GLfloat cameraZ, GLfloat matriz[6][4],
                       bool inverted);
      /*!
       *************************************************************** 
       * Draw the map walls on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param inverted -> usefull for reflections
       ***************************************************************/
      void drawWalls(GLfloat cameraX, GLfloat cameraY, 
                     GLfloat cameraZ, GLfloat matriz[6][4],
                     bool inverted);
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
       * \param wTypes -> weapons types
       * \return 1 on success.
       ***************************************************************/
      int open( string arquivo, modelList& mdlList, weaponTypes& wTypes ); 
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
        * \param divisions -> how many divisions
        *************************************************************/
       void drawSurfaceOnMap(GLuint image, GLfloat xa, GLfloat za, 
                             GLfloat xb, GLfloat zb, GLfloat sumY,
                             int divisions);

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
       * \param collision -> true if has collision with object, false otherwise
       **************************************************************/
      void insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                        object* obj, int qx, int qz, bool collision);
      void insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                        object* obj, bool collision);

      /*!
       ************************************************************* 
       * Insert object on map list
       * \param arquivo -> filename to load
       * \param mdlList -> modelList
       * \param wTypes -> list of weapons types
       * \return pointer to the map Object created 
       *************************************************************/
      object* insertObject(string arquivo, modelList& mdlList, 
                           weaponTypes& wTypes);

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
      void setInitialPosition(GLfloat iX, GLfloat iZ);

      /*!
       ***************************************************************
       * Verify if the map is outdoor
       * \return -> true if the map is outdoor
       ***************************************************************/
      bool isOutdoor();

      /*!
       ***************************************************************
       * Set if the map is outdoor or not  
       * \param val -> true if is outdoor, false if indoor
       ***************************************************************/
      void setOutdoor(bool val);

      /*!
       ***************************************************************
       * Create all the splats vertexes related to splatting, if map
       * is outdoor. If is indoor, only create splats to any of the
       * textures, without merging them with alpha things.
       ***************************************************************/
      void createSplats();

      void createAlpha(int x1, int z1);

      void actualizeAlphaTextures();

      int getTextureID(string textureName, GLuint* R, GLuint* G, GLuint* B);
      string getTextureName(GLuint ID);
      texture* getTexture(GLuint id);
      GLuint insertTexture(string arq, string name, 
                           GLuint R, GLuint G, GLuint B);
      void defineCommonTexture();

      mapFog fog;           /**< Map's Fog */
      mapLights lights;     /**< Map's Lights */
      //mapRoad* roads;     /**< Map's Roads */
      int numTextures;      /**< Number of distinct Textures on Map */
      texture* textures;    /**< List of textures on Map */
      Square* squareInic;   /**< Square where PCs starts */
      wall* walls;          /**< Map Walls */
      wall* curbs;          /**< Map Meio Fios (how translate this?) */
      door* doors;          /**< Map Doors */

      int SQUAREMINISIZE;   /**< Minimap square size */
      int SQUAREMINIDIV;    /**< MiniMap square division relation */

      protected:


      /*!
       *************************************************************** 
       * Draw the Outdoor floor on screen
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \return 1 if success.
       ***************************************************************/
      void drawFloorOutdoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                            GLfloat matriz[6][4]);

      /*!
       *************************************************************** 
       * Draw the indoor floor on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \return 1 if success.
       ***************************************************************/
      void drawFloorIndoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                           GLfloat matriz[6][4]);


         int x,                /**< Map X dimension (in squares) */
             z;                /**< Map Z Dimension (in squares) */
         bool outdoor;         /**< If it's an outdoor or indoor map */
         Square** MapSquares;  /**< Internal Map squares */
         lObject* objects;     /**< Map's objects list */
         string music;         /**< Map Music */
         string name;          /**< File name of loaded map */
         string particlesFileName; /**< File Name of Map Particles Systens */
         string npcFileName;   /**< Static NPC's on map filename */
         float xInic,            /**< X coordinate where PCs starts */
               zInic;            /**< Z Coordinate where PCs starts */

         float* vertexBuffer;
         float* uvBuffer;
         float* uvAlphaBuffer;
         int totalVertex;
         GLuint commonTexture;   /**< The most common texture on the map */
};

#endif
