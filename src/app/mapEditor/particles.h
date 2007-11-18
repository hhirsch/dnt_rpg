#ifndef _particles_h
#define _particles_h

#include "grasswindow.h"
#include "waterwindow.h"
#include "message.h"
#include "guiIO.h"
#include "../../map/map.h"
#include "../../particle/partSystem.h"


class particles
{
   public:
      /*! Constructor
       * \param acMap -> poiter to opened map*/
      particles(Map* acMap);
      /*! Destructor */
      ~particles();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param grWindow -> the grassWindow Used*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, Uint8* keys, guiIO* gui, partSystem* pS,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4],
                        string selectedText, grassWindow* grWindow,
                        waterWindow* wtWindow);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary(GLfloat matriz[6][4]);

      void deleteParticle();

   private:
      Map* actualMap;                  /**< actual Opened Map */
      particleSystem* actualParticle;  /**< actual Particle */
      int state;                       /**< actual State */
      int particleType;                /**< actual Particle Type */
      GLfloat height;                  /**< actual Particle Height */

      string previousText;             /**< Previous selectedText */

      GLfloat x1,                      /**< X1 coordinate for grass */
              z1,                      /**< Z1 Coordinate for grass */
              x2,                      /**< X2 coordinate for grass */
              z2;                      /**< Z2 coordinate for grass */

      interPlane* actualPlane;         /**< actual Plane */
};

#endif

