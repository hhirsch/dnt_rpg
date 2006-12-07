#ifndef _briefcase_h
#define _briefcase_h

#include <SDL/SDL_opengl.h>
#include "../../etc/glm.h"


#define MAX_BRIEFCASES 30  /**< Max number of briefcases in the list */
#define MAX_VALUE 10000    /**< Max Value of briefCase */
#define SUM_VALUE 200      /**< Sum Value */

/*! the briefCase class */
class briefCase
{
   public:
      GLfloat x;        /**< x coordinate of the center of the briefcase */
      GLfloat z;        /**< z coordinate of the center of the briefcase */
      GLfloat angle;    /**< briefCase orientation angle */
      int value;        /**< value of the briefCase */
      bool owned;       /**< if someone owns the case */
      bool delivered;   /**< if the case is already delived(to group or police)*/
};


/*! the list of briefCases Class */
class briefCases
{
   public:
      /*! Constructor */
      briefCases();
      /*! Destructor */
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

      /* Returns the total number of briefcases in list */
      int getTotal(){return(totalCases);};

      void removeAll(){totalCases=0;};

      briefCase* getBriefCase(int briefNumber);

   private:
      briefCase cases[MAX_BRIEFCASES];  /**< vector of briefCases */
      int totalCases;                   /**< total number of briefcases */
      GLMmodel* briefModel;             /**< briefCase 3D model */
};

#endif

