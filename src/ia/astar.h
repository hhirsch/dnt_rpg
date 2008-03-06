#ifndef _astar_h
#define _astar_h

#include "../map/map.h"
#include "pattAgent.h"
#include <SDL/SDL_opengl.h>

/*! The A* Point Class */
class pointStar
{
   public:
      GLfloat x,           /**< Way Point X Coordinate */
              z;           /**< Way Point Z Coordinate */
      GLfloat heuristic;   /**< Calue of the heuristic to this point */
      GLfloat gone;        /**< How much percorred to this point */
      GLfloat parentX,     /**< Node parent X position */
              parentZ;     /**< Node parent Z position */
      pointStar* next;     /**< Next Point */
      pointStar* previous; /**< Previous Point */
};

/*! The A* Point List Class */
class listStar
{
   public:
      /*! Constructor */
      listStar();
      /*! Destructor */
      ~listStar();

      /*! Insert Node on List 
       * \param x -> x map position
       * \param z -> z map position
       * \param gone -> distance travelled
       * \param heuristic -> heuristic to the goal 
       * \param parentX -> X position of the parent
       * \param parentZ -> Z position of the parent */
      pointStar*  insert(GLfloat x, GLfloat z, GLfloat gone, GLfloat heuristic,
                         GLfloat parentX, GLfloat parentZ);
      /*! Remove Node from list 
       * \param node -> node to be removed */
      void remove(pointStar* node);
      /*! Find a specific node on list
       * \param x -> node X position
       * \param z -> node Z position
       * \return pointer to the founded node. NULL if not found */
      pointStar* find(GLfloat x, GLfloat z);
      /*! Find the node with lowest value
       * \return pointer to the founded node. NULL if not found */
      pointStar* findLowest();
      /*! Verify if the list is empty
       * \return true if empty, false otherwise */
      bool isEmpty(){return(totalNodes <= 0);};
      /*! return the size of the List */
      int size(){return(totalNodes);};

   private:
      pointStar* first;       /**< First Point in the list */
      int totalNodes;         /**< Total Nodes in the list */
};

#define ASTAR_STATE_OTHER     0  /**< A* Other State */
#define ASTAR_STATE_RUNNING   1  /**< A* Running State (searching for path) */
#define ASTAR_STATE_FOUND     2  /**< A* Found Path State */   
#define ASTAR_STATE_NOT_FOUND 3  /**< A* Not Found Path State */

/*! A* implementation. The A* will be search in paralel when called with 
 *  findPath. When running, state is RUNNING. When end running, state is
 *  FOUND or NOT_FOUND, based on the result of the search. */
class aStar
{
   public:
      /*! Constructor */
      aStar();
      /*! Destructor */
      ~aStar();

      /*! Define the actual opened map 
       *  \param acMap -> current opened map. */
      void defineMap(Map* acMap);

      /*! Calls A* thread to find path
       * \param actualX -> current x position
       * \param actualZ -> current z position 
       * \param x -> destiny x position
       * \param z -> desired z position
       * \param stepSize -> size of the Step
       * \param orientation -> character orientation
       * \param perX1 -> per X initial Bounding position
       * \param perY1 -> per Y initial Bounding position
       * \param perZ1 -> per Z initial Bounding position
       * \param perX2 -> per X final Bounding position
       * \param perY2 -> per Y final Bounding position
       * \param perZ2 -> per Z final Bounding position */
      /*void findPath(GLfloat actualX, GLfloat actualZ, GLfloat x, GLfloat z,
                    GLfloat stepSize, GLfloat orientation,
                    GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                    GLfloat perX2, GLfloat perY2, GLfloat perZ2);*/
      void findPath(void* actor, GLfloat x, GLfloat z, GLfloat stepSize,
                    void* NPCs, void* PCs);

      /*! A* to find path INTERNAL
       * \param actualX -> current x position
       * \param actualZ -> current z position 
       * \param x -> destiny x position
       * \param z -> desired z position
       * \param stepSize -> size of the Step
       * \param orientation -> character orientation
       * \param perX1 -> per X initial Bounding position
       * \param perY1 -> per Y initial Bounding position
       * \param perZ1 -> per Z initial Bounding position
       * \param perX2 -> per X final Bounding position
       * \param perY2 -> per Y final Bounding position
       * \param perZ2 -> per Z final Bounding position
       * \return true if found path, false otherwise. */
      /*bool findPathInternal(GLfloat actualX, GLfloat actualZ, 
                            GLfloat x, GLfloat z,
                            GLfloat stepSize, GLfloat orientation,
                            GLfloat perX1, GLfloat perY1, GLfloat perZ1, 
                            GLfloat perX2, GLfloat perY2, GLfloat perZ2);*/
      bool findPathInternal(void* actor, GLfloat x, GLfloat z, GLfloat stepSize,
                            void* NPCs, void* PCs);

      /*! Get the New Character Position, based on Path Found previously 
       * \param posX -> new X position
       * \param posZ -> new Z position
       * \param ori -> new orientation
       * \return true if can actualize, false otherwise */
      bool getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori);

      /*! Gets the Destiny of The Character
       * \param destX -> X destiny Position
       * \param destZ -> Z destiny Position */
      void getDestiny(GLfloat& destX, GLfloat& destZ);

      /*! Draws the founded path */
      void drawPath();

      /*! Get Actual State of the aStar and put it on OTHER mode
       * \return state value if the state is nor RUNNING. */
      int getState();

      /*! Set character orientation value
       * \param ori -> new orientation value */
      void setOrientation(GLfloat ori);

      /*! Clear the a* state */
      void clear();

      /*! Toggle the a* to force the next call */
      void forceNextCall();

   private:
      Map* actualMap;           /**< Pointer to opened Map */
      GLfloat destinyX,         /**< Destiny X position */
              destinyZ;         /**< Destiny Z position */
      pattAgent* patt;          /**< The Pattern Agent to Follow created path */
      int state;                /**< Internal State of the aStar */
      SDL_Thread* searchThread; /**< The Search Paralel Thread */
      SDL_mutex* searchMutex;   /**< The Search Mutex */
      GLuint lastCallTime;      /**< Last Time when Called Search */


      bool abort;               /**< Flag to abort the execution of the a* */

      /*! lock Mutex */
      void lock();
      /*! unLock Mutex */
      void unLock();


};


#endif

