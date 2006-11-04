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
       * \param heuristic -> heuristic to the goal */
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

   private:
      pointStar* first;       /**< First Point in the list */
      int totalNodes;         /**< Total Nodes in the list */
};

/*! A* implementation */
class aStar
{
   public:
      /*! Constructor */
      aStar();
      /*! Destructor */
      ~aStar();

      /*! Define the actual opened map 
       *  \param map -> current opened map. */
      void defineMap(Map* map);

      /*! A* to find path
       * \param actualX -> current x position
       * \param actualZ -> current z position 
       * \param x -> destiny x position
       * \param z -> desired z position
       * \return true if found path, false otherwise. */
      bool findPath(GLfloat actualX, GLfloat actualZ, GLfloat x, GLfloat z,
                    GLfloat stepSize, GLfloat orientation);

      bool getNewPosition(GLfloat& posX, GLfloat& posZ, GLfloat& ori);

      void drawPath();

   private:
      Map* actualMap;         /**< Pointer to opened Map */
      GLfloat destinyX,       /**< Destiny X position */
              destinyZ;       /**< Destiny Z position */
      pattAgent* patt;        /**< The Pattern Agent to Follow created path */
};


#endif

