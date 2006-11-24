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
                        Uint8 mButton, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

   private:
      Map* actualMap;         /**< actual Opened Map */
};

#endif

