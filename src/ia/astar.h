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
      pointStar* parent;   /**< Node parent to this one */
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
       * \param parent -> parent to the node */
      void insert(GLfloat x, GLfloat z, GLfloat gone, GLfloat heuristic,
                  pointStar* parent);
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
      bool isEmpty(){return(totalNodes > 0);};

   private:
      pointStar* first;       /**< First Point in the list */
      int totalNodes;         /**< Total Nodes in the list */
};

/*! A* implementation */
class aStar
{
   public:
      /*! Constructor
       * \param map -> current opened map. */
      aStar(Map* map);
      /*! Destructor */
      ~aStar();

      /*! A* to find path
       * \param actualX -> current x position
       * \param actualZ -> current z position 
       * \param x -> destiny x position
       * \param z -> desired z position
       * \return true if found path, false otherwise. */
      bool findPath(GLfloat actualX, GLfloat actualZ, GLfloat x, GLfloat z,
                    GLfloat stepSize, GLfloat orientation);

      bool getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori);

   private:
      Map* actualMap;         /**< Pointer to opened Map */
      GLfloat destinyX,       /**< Destiny X position */
              destinyZ;       /**< Destiny Z position */
      pattAgent* patt;        /**< The Pattern Agent to Follow created path */
};


#endif

