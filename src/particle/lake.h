/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#ifndef _lake_h
#define _lake_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/*! The lake class */
class lake
{
   public:
      /*! Constructor */
      lake();
      /*! Destructor */
      ~lake();

      /*! Define Lake Position
       * \param x1 -> x first coordinate
       * \param z1 -> z first coordinate
       * \param x2 -> x last coordinate
       * \param z2 -> z last coordinate */
      void definePosition(GLfloat x1, GLfloat z1, GLfloat x2, GLfloat z2);     

      /*! Define Color
       * \param r -> red
       * \param g -> green 
       * \param b -> blue
       * \param a -> alpha */
      void defineColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
   
      /*! Draw the lake */
      void draw();

      lake* next;          /**< Next lake on the list */
      lake* previous;      /**< Previous lake on the list */

   protected:
      GLfloat pos[4];      /**< Lake position */
      GLfloat color[4];    /**< Lake Color */

      GLfloat *vertexBuffer;  /**< Lake Render Buffer */
      int numVertex;          /**< Number of vertexes on lake */

};

#endif

