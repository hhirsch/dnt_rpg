/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_h
#define _dnt_map_h

#include "mapfog.h"
#include "maplights.h"
#include "maproad.h"
#include "mapsound.h"

#include "../particle/lake.h"
#include "../etc/texrenderer.h"
#include "../classes/object.h"
#include "../classes/weapon.h"

#include <string>
using namespace std;

/* Flag masks */
#define SQUARE_CAN_WALK 1   /**< If a Square is walkable or not. */

/* Constraints */
#define OUTDOOR_SQUARE_SIZE      256      /**< Size of the Outdoor Square */
#define INDOOR_SQUARE_SIZE        64      /**< Size of the indoor square */


#define MAX_WALLS             15       /**< Max number of walls per square */
#define WALL_HEIGHT           50       /**< Walls height */
#define CURB_HEIGHT            2       /**< Curbs height */
#define MAX_HEIGHT           150       /**< Max square height */

#define TEXTURE_REPEATS (OUTDOOR_SQUARE_SIZE / 256) /**< Number of Repeats */ 
#define ALPHA_TEXTURE_INC       2 /**< Points per square on each alpha texture*/

#define DOOR_STATUS_CLOSED      0      /**< If door is closed */
#define DOOR_STATUS_OPENED      1      /**< If door is opened */


/*! Map's Connections is a clickable area on the map that changes 
 * the current map to the one the connection defines. */
class conection
{
   public:
      bool active;          /**< Active Conection */
      GLfloat x1,z1,x2,z2;  /**< Map Position */
      GLfloat angle;        /**< Connection Angle */
      string mapName;       /**< Map filename */ 
};

/*! Texture information to be used by an wall */
class wallTexture
{
   public:
      /*! Constructor */
      wallTexture();

      /*! Equals two textures */
      void operator=(wallTexture* t);

      /*! Get texture ID
       * \return Id of the texture */
      int getTextureId();
      /*! Set the texture ID 
       * \param id -> id of the texture */
      void setTextureId(int id);

      /*! Set the texture name
       * \param name -> texture name */
      void setTextureName(string name);

      /*! Get the texture name
       * \return -> string with texture name */
      string getTextureName();

      /*! Get Texture Deltas
       * \param x -> deltaX
       * \param y -> deltaY
       * \param z -> deltaZ */
      void getDelta(GLuint& x, GLuint& y, GLuint& z); 
       /*! Set Texture Deltas
       * \param x -> deltaX
       * \param y -> deltaY
       * \param z -> deltaZ */
      void setDelta(GLuint x, GLuint y, GLuint z); 

   protected:

      int textureId;       /**< Texture ID Used*/
      string textureName;  /**< Texture Name */
      GLuint dX,           /**< Delta X of texture cycle */
             dY,           /**< Delta Y of texture cycle */
             dZ;           /**< Delta Z of texture cycle */ 
};

/*! A map Wall. Just a wall usually used in indoor maps */
class wall: public dntListElement
{
   public:
      GLfloat x1,          /**< X min coordinates */
              z1,          /**< Z min coordinates */
              x2,          /**< X max Coordinates */
              z2;          /**< Z max Coordinates */
      
      wallTexture frontTexture;    /**< Front Texture ID */
      wallTexture backTexture;     /**< Back Texture ID */
      wallTexture rightTexture;    /**< Right Texture ID */
      wallTexture leftTexture;     /**< Left Texture ID */
};

/*! The list of walls */
class wallList: public dntList
{
   public:
      /*! Constructor */
      wallList();
      /*! Destructor */
      ~wallList();

   protected:
      /*! Free the wall memory */
      void freeElement(dntListElement* obj);
};

/*! Definition of a texture used for the map */
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
      texture* next;            /**< Next on List */
      texture* previous;        /**< Previous on List */
};

/*! A door is an object that, obviously, opens and close passages */
class door
{
   public:
      object* obj;             /**< pointer to door object */
      GLfloat x,z;             /**< position on map */
      GLint status;            /**< actual status (opened, closed) */
      GLint orientation;       /**< orientation */
      GLfloat delta;           /**< current orientation delta
                                    (is inc or dec by the current status,
                                     from 0 to 90) */
      door* next;      /**< pointer to next door on map */
};

/*! An object that is inner a map's square definition */
class objSquare : public dntListElement
{
   public:
      bool draw;           /**< If is to draw the object (true if
                                the object orign is in this square,
                                false if the object orign is on other 
                                square. This is used to avoid drawning
                                more than one time the object, if it is on
                                more than one square at a time).*/
      
      GLfloat x;           /**< X Position (used to simplify) */
      GLfloat y;           /**< Y position (used to simplify) */
      GLfloat z;           /**< Z positon (used to simplify) */
      GLfloat orientation; /**< Orientation (used to simplify) */
      
      int status;          /**< The status of the object */
      bool colision;       /**< If the collision is enable to this object */
      object* obj;         /**< The pointer to the object used */
};

/*! The square definition. A square is the atom of the map, with
 * its texture, objects, etc. Also its a list of objSquares too.
 * \note -> an object can be om more than one square at the same time 
 * \note -> the square's act like the map's BSP */
class Square: public dntList
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

      /*! Add object to the square 
       * \param draw -> true if the draw is at this square
       * \param collision -> if collision is enabled for this object 
       * \param obj -> pointer to the object that is in the square
       * \return -> pointer to the created objectSquare */
      objSquare* addObject(bool draw, GLfloat x, GLfloat y, GLfloat z,
                           GLfloat orientaton, bool colision, object* obj);

      /*! Remove Object from the square
       * \param obj -> pointer to the objectSquare to remove
       * \note -> it will free the objSquare, but not the object* */
      void removeObject(objSquare* obj);
   
      /*! Get the first object on the list
       * \return pointer to the first objSquare on the list */
      objSquare* getFirstObject();

      /*! Get the number of objects on the list
       * \return number of objects on the list */
      int getTotalObjects();

   protected:
      /*! Free the objSquare memory
       * \param obj -> pointer to the objSquare to delete */
      void freeElement(dntListElement* obj);

};

/*! Map Class */
class Map
{
   public:
      Map();       /**< Construtor */
      ~Map();      /**< Destruidor */


      /*! Gets the square size on the map 
       * \return-> the square size used on the map */ 
      int squareSize();
      /*! Get the minimap square size
       * \return -> size of each square on the minimap */
      float getSquareMiniSize();

      /*! Gets coordinate relative square
       * \param xa -> x coordinate of square
       * \param za -> z coordinate of square
       * \return the coordinate relative square. */
      Square* relativeSquare(int xa, int za);
      /*! Get the initial square (where character begins)
       * \return -> pointer to the initial square */
      Square* getInitialSquare();

      /*! Render an invisible surface only to the unproject */
      void renderInvisibleSurface();

      /*! Render on screen the visible map (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param perX -> active character X position
       * \param perZ -> active character Z position
       * \return 1 if success. */
      int render(GLfloat cameraX, GLfloat cameraY, 
                 GLfloat cameraZ, GLfloat** matriz,
                 GLfloat perX, GLfloat perZ);
      /*! Render the floor on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param selectionRender -> true if renderng at SELECTION mode
       * \return 1 if success. */
      void renderFloor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                       GLfloat** matriz, bool selectionRender=false);
      /*! Render the map objects on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param inverted -> usefull for reflections */
      void renderObjects(GLfloat cameraX, GLfloat cameraY, 
                         GLfloat cameraZ, GLfloat** matriz,
                       bool inverted);
      /*! Render the map walls on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param inverted -> usefull for reflections */
      void renderWalls(GLfloat cameraX, GLfloat cameraY, 
                       GLfloat cameraZ, GLfloat** matriz,
                       bool inverted);
      /*! Render a texture (surface) over map terrain (floor)
        * \param image -> openGL texture ID
        * \param xa -> first x position
        * \param za -> first z position
        * \param xb -> last x position
        * \param zb -> last z position
        * \param sumY -> value to sum at Y height of the map
        * \param divisions -> how many divisions */
       void renderSurfaceOnMap(GLuint image, GLfloat xa, GLfloat za, 
                               GLfloat xb, GLfloat zb, GLfloat sumY,
                               int divisions);


      /*! Draw Minimap, relative to current map and character position. */
      void drawMiniMap();

      /*! Get the minimap surface
       * \return -> pointer to the rendered minimap surface
       * \note -> you must call drawMinimap at last one time before calling 
       *          this function */
      SDL_Surface* getMiniMap();

      /*! Create a new empty map, with new structs.
       * \param X -> number of squares on X coordinate
       * \param Z -> number of squares on Z coordinate
       * \note -> usually called at dntMapEditor */
      void newMap(int X,int Z);
      /*! Opens map from file
       * \param arquivo ->  Name of file to be opened
       * \return 1 on success. */
      int open(string arquivo); 
      /*! Save map to file
       * \param arquivo -> file name to be saved
       * \return -> 1 on success. */
      int save( string arquivo ); 


      /*! Optimize Map Structs */
      void optimize();


      /*! Get Height Value for Map position
       * \param nx -> map X position
       * \param nz -> map Z position
       * \return height of the map on nx,nz position */
      GLfloat getHeight(GLfloat nx, GLfloat nz);
      /*! Get Height Value for Map position
       * \param nx -> map X position
       * \param nz -> map Z position
       * \param saux -> the relative square to get the height from
       * \return height of the map on nx,nz position */
      GLfloat getHeight(GLfloat nx, GLfloat nz, Square* saux);
      /*! Define Thing new Height, based on Thing position
       * \param c -> pointer to the thing
       * \param nx -> thing new X position
       * \param nz -> thing new Z position
       * \return -> true if can define new heigh, false if can't move. */
      bool defineThingHeight(thing* c, GLfloat nx, GLfloat nz);


      /*! Remove Object from Map
       * \param obj -> pointer to map Object
       * \note -> this function will not delete the object pointer.
       *          so if you want it deleted, just do yourself */
      void removeObject(object* obj);
      /*! Insert Object on Map
       * \param xReal -> xPosition to insert
       * \param yReal -> yPosition to insert
       * \param zReal -> zPosition to insert
       * \param orObj -> object orientation value
       * \param obj -> pointer to mapObject
       * \param qx -> square internal X (in Squares)
       * \param qz -> square internal Z (in squares)
       * \param collision -> true if has collision with object */
      void insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, 
                        GLfloat orObj,
                        object* obj, int qx, int qz, bool collision);
      /*! Insert Object on Map
       * \param xReal -> xPosition to insert
       * \param yReal -> yPosition to insert
       * \param zReal -> zPosition to insert
       * \param orObj -> object orientation value
       * \param obj -> pointer to mapObject
       * \param collision -> true if has collision with object */
      void insertObject(GLfloat xReal, GLfloat yReal, GLfloat zReal, 
                        GLfloat orObj, object* obj, bool collision);

      /*! Get Map File Name
       * \return map fileName */
      string getFileName();
      /*! Set Map File Name
       * \param fName -> New fileName */
      void setFileName(string fName);
      /*! Get the Map Name
       * \return -> string with the map's name */
      string getName();

      /*! Get NPC File Name
       * \return NPC fileName */
      string getNpcFileName();
      /*! Set NPC File Name
       * \param fName -> New fileName */
      void setNpcFileName(string fName);

      /*! Get Music File Name
       * \return music fileName */
      string getMusicFileName();
      /*! Set Music File Name
       * \param fName -> New fileName */
      void setMusicFileName(string fName);

      /*! Get Sounds File Name
       * \return sounds fileName */
      string getSoundsFileName();
      /*! Set Sounds File Name
       * \param fName -> New fileName */
      void setSoundsFileName(string fName);

      /*! Get Particles File Name
       * \return particles fileName */
      string getParticlesFileName();
      /*! Set Particles File Name
       * \param fName -> New fileName */
      void setParticlesFileName(string fName);


      /*! Get Map Size X
       * \return Map X size on squares */
      int getSizeX();
      /*! Set Map Size X
       * \param sizeX -> Map X size on squares */
      void setSizeX(int sizeX);
      /*! Get Map Size X
       * \return Map X size on squares */
      int getSizeZ();
      /*! Set Map Size Z
       * \param sizeZ -> Map Z size on squares */
      void setSizeZ(int sizeZ);
      
      /*! Get Initial Position
       * \param iX -> initial X position
       * \param iZ -> initial Z position
       * \param iAngle -> initial character orientation */
      void getInitialPosition(float& iX, float& iZ, float& iAngle);
      /*! Set the initial position
       * \param iX -> initial X position
       * \param iZ -> initial Z position */
      void setInitialPosition(GLfloat iX, GLfloat iZ);


      /*! Verify if the map is outdoor
       * \return -> true if the map is outdoor */
      bool isOutdoor();
      /*! Set if the map is outdoor or not  
       * \param val -> true if is outdoor, false if indoor */
      void setOutdoor(bool val);



      /*! Flush all map sounds to the sound controller */
      void flushSounds();

      /*! Create all the splats vertexes related to splatting, if map
       * is outdoor. If is indoor, only create splats to any of the
       * textures, without merging them with alpha things. */
      void createSplats();
      /*! Create the alpha for texture splatting
       * \param x1 -> X size of texture
       * \param z1 -> Z size of texture  */
      void createAlpha(int x1, int z1);
      /*! Update the Alpha Textures for texture splatting */
      void updateAlphaTextures();
      /*! Define the common texture for the texture splatting */
      void defineCommonTexture();


      /*! Get the integer ID value of the texture
       * \param textureName -> Name of the texture
       * \param R -> the texture Red Color got
       * \param G -> the texture Green Color got
       * \param B -> the texture Blue Color got
       * \return integer with the texture ID, or -1 if not found. */
      int getTextureID(string textureName, GLuint& R, GLuint& G, GLuint& B);
      /*! Get the texture name
       * \param ID -> integer ID of the texture
       * \return texture Name */
      string getTextureName(GLuint ID);
      /*! Get the texture
       * \param id -> integer ID of the texture
       * \return texture pointer */
      texture* getTexture(GLuint id);
      /*! Insert the texture on the map
       * \param arq -> fileName of the texture
       * \param name -> Name of the texture
       * \param R -> Red Color of the texture
       * \param G -> Green Color of the texture
       * \param B -> Blue Color of the texture */
      GLuint insertTexture(string arq, string name, 
                           GLuint R, GLuint G, GLuint B);
      /*! Remove all unused textures from the list */
      void removeUnusedTextures();


      /*! Add Wall to the map
       * \param x1 -> wall X1
       * \param z1 -> wall Z1
       * \param x2 -> wall X2
       * \param z2 -> wall Z2
       * \return pointer to the added wall */
      wall* addWall(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2); 
      /*! Get the first wall on the map
       * \return pointer to the first wall on map */
      wall* getFirstWall();
      /*! Remove an wall from the map
       * \param w -> pointer to wall to remove */
      void removeWall(wall* w);
      /*! Get total Number of walls on map
       * \return total number of walls on map */
      int getTotalWalls();
      
      /*! Get the first door on the map's list
       * \return -> pointer to the first door or NULL */
      door* getFirstDoor();
      /*! Get the total number of doors on list */
      int getTotalDoors();
      /*! Insert a door on door's list
       * \param newDoor -> pointer to the door to insert */
      void insertDoor(door* newDoor);

      /*! Get the current mapFog
       * \return -> mapFog information */
      mapFog getFog();

      
      /*! Add a lake to the map
       * \param x1 -> X first coordinate 
       * \param z1 -> Z first coordinate
       * \param x2 -> X last coordinate 
       * \param z2 -> Z last coordinate 
       * \return -> pointer to the lake added */
      lake* addLake(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2);

      /*! Set editor as friend, one time it MUST have acces to some
       * internal structures (that aren't needed to be exposed for 
       * other things) */
      friend class editor;

   protected:

      /*! Alloc all needed map structures (buffers, squares, etc.)
       * \note -> map size must be defined before call this function
       * \note -> only call this function one time per map */
      void alloc();
      /*! Fill all buffers needed to render (doing culling)
       * \param matriz -> current view frustum matrix */
      void createBuffers(GLfloat** matriz);

      /*! Draw the Outdoor floor on screen
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz
       * \param selectionRender -> true if renderng at SELECTION mode
       * \return 1 if success. */
      void renderFloorOutdoor(GLfloat cameraX,GLfloat cameraY,GLfloat cameraZ, 
                              GLfloat** matriz, bool selectionRender=false);
      /*! Draw the indoor floor on screen (using view culling)  
       * \param cameraX -> X coordinate of camera
       * \param cameraY -> Y coordinate of camera
       * \param cameraZ -> Z coordinate of camera
       * \param matriz  -> view frustum matriz 
       * \param selectionRender -> true if renderng at SELECTION mode
       * \param outdoorCompatible -> true if is rendering an outdoor map
       *                             at compatible mode (without multitextures)
       * \return 1 if success. */
      void renderFloorIndoor(GLfloat cameraX, GLfloat cameraY, GLfloat cameraZ, 
                             GLfloat** matriz, bool selectionRender=false,
                             bool outdoorCompatible=false);

      /*! Delete all remaining objects on the map */
      void deleteObjects();


      SDL_Surface* miniMap;    /**< The rendered minimap surface */

      mapFog fog;           /**< Map's Fog */
      mapLights lights;     /**< Map's Lights */
      //mapRoad* roads;     /**< Map's Roads */
      int numTextures;      /**< Number of distinct Textures on Map */
      texture* textures;    /**< List of textures on Map */
      Square* squareInic;   /**< Square where PCs starts */
      door* doors;          /**< Map Doors */
      int totalDoors;       /**< Total Number of doors on the map */
      lake* lakes;          /**< Chain list of lakes on map */
      int totalLakes;       /**< Total lakes on map */

      float squareMiniSize; /**< Minimap square size */
      float squareMiniDiv;  /**< MiniMap square division relation */

      mapSound* sounds;      /**< Map Sound Effects */
      string soundsFileName; /**< Sound Info File Name */

      wallList walls;        /**< Map Walls */

      int x,                /**< Map X dimension (in squares) */
          z;                /**< Map Z Dimension (in squares) */
      bool outdoor;         /**< If it's an outdoor or indoor map */
      Square** MapSquares;  /**< Internal Map squares */
      string music;         /**< Map Music */
      string fileName;      /**< File name of loaded map */
      string name;          /**< The name of the map */

      string particlesFileName; /**< File Name of Map Particles Systens */
      string npcFileName;       /**< Static NPC's on map filename */
      float xInic,              /**< X coordinate where PCs starts */
            zInic;              /**< Z Coordinate where PCs starts */
      float angleInic;          /**< Angle for PC at initial position */

      float* vertexBuffer;     /**< The vertex array buffer used */
      float* uvBuffer;         /**< The UV-map array buffer used */
      float* uvAlphaBuffer;    /**< The alpha UV-map buffer for splatting */
      int totalVertex;         /**< Current total vertex at vertexBuffer */

      texRenderer* wallRenderer;  /**< The texture Renderer for walls */

      GLuint commonTexture;   /**< The most common texture on the map */

};

#endif
