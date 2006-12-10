#ifndef _exclamation_h
#define _exclamation_h

#include "../../etc/glm.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

class excObject
{
   public:
      GLfloat posX,           /**< X Position */
              posY,           /**< Y Position */
              posZ;           /**< Z Position */

      excObject* next;        /**< Next Exclamation on List */
      excObject* previous;    /**< Previous Exclamation on List */
};

/*! Exclamation Alert Class (show an exclamation on 3D world */
class exclamation
{
   public:
      /*! Constructor */
      exclamation();
      /*! Destructor */
      ~exclamation();

      /*! Add an exclamation to the list 
       * \param x -> X Position of the new exclamation
       * \param z -> Z Position of the new Exclamation */
      void addExclamation(GLfloat x, GLfloat z);

      /*! Do Step on exclamations and draw then to screen */
      void doStep();

   private:

      /*! Remove an Exclamation from List
       * \param exc -> Exclamation to be removed */
      void removeExclamation(excObject* exc);

      /*! Search for a exclamation in position
       * \param x -> X position
       * \param z -> Z position
       * \return True if exclamation exists */
      bool find(GLfloat x, GLfloat z);
      
      GLMmodel* exclamationModel;   /**< 3D model of the exclamation */

      excObject list;               /**< Exclamation List Head Node */
      int totalExclamations;        /**< Total Number of Exclamation on list */
};

#endif
