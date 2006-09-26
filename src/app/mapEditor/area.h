#ifndef _area_h
#define _area_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! Simple Definition of Map Area */
class area
{
   public:
      GLfloat x1, y1, x2, y2; /**< Coordinates */
      string whereToGo;       /**< Destiny */
      area* next;             /**< Next Node */
};

/*! List of Map Areas */
class areaList
{
   public:
      /*! Constructor */
      areaList();
      /*! Destructor */
      ~areaList();

      /*! Search and Return an Aread on the list */
      area* getArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
      /*! Add an area to the list */
      void addArea(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, 
                   string whereToGo);

      /*! Draw on screen all portals */
      void drawAll();

   private:
      area* first; /**< First Node of the list */
      int total;   /**< Total Nodes of the list */
};


#endif

