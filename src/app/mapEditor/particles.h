#ifndef _particles_h
#define _particles_h

#include "message.h"
#include "../../map/map.h"
#include "../../particle/partSystem.h"


class particles
{
   public:
      /*! Constructor
       * \param map -> poiter to opened map*/
      particles(Map* map);
      /*! Destructor */
      ~particles();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, Uint8* keys, int tool, partSystem* pS,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary(GLfloat matriz[6][4]);

      void deleteParticle();

   private:
      Map* actualMap;                  /**< actual Opened Map */
      particleSystem* actualParticle;  /**< actual Particle */
      int state;                       /**< actual State */
      int particleType;                /**< actual Particle Type */
      GLfloat height;                  /**< actual Particle Height */

      interPlane* actualPlane;         /**< actual Plane */
};

#endif

