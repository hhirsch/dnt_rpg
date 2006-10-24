#ifndef _briefcase_h
#define _briefcase_h

#include <SDL/SDL_opengl.h>
#include "../../etc/glm.h"


#define MAX_BRIEFCASES 30

class briefCase
{
   public:
      GLfloat x;
      GLfloat z;
      GLfloat angle;
      int value;
      bool owned;
};

class briefCases
{
   public:
      briefCases();
      ~briefCases();

      /*! Returns the most expensive briefcase in area
       * \returns briefcase if has almost one, NULL otherwise. */
      briefCase* briefCaseInArea(GLfloat x1,GLfloat z1,GLfloat x2,GLfloat z2);

      /*! Insert briefcase on list */
      void insertBriefCase(GLfloat x, GLfloat z, GLfloat angle, int value);

      /*! Draw All BriefCases */
      void draw();

      /*! Draw a BriefCase on Position */
      void drawAt(GLfloat x, GLfloat z, GLfloat orientation);

      int geTotal(){return(totalCases);};

   private:
      briefCase cases[MAX_BRIEFCASES];
      int totalCases;
      GLMmodel* briefModel;
};

#endif

