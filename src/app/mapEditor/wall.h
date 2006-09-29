#ifndef _wall_h
#define _wall_h

#include "../../map/map.h"
#include "message.h"


class wall
{
   public:
      /*! Constructor 
       * \param map -> pointer to opened map */
      wall(Map* map);
      /*! Destructor */
      ~wall();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool
       * \param actualTexture -> current Texture */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, Uint8* keys, int tool, 
                        GLuint actualTexture);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

   private:
      Map* actualMap;         /**< Actual Internal Map */
      int actualTool;         /**< Actual Tool */
      int state;              /**< Actual Internal State */
      muro* actualWall;       /**< Actual Selected Wall */
      bool limitSquare;       /**< Limit Wall to square's vertices*/

      GLuint texture;         /**< Current Texture */

      GLfloat mX, mY, mZ;     /**< Mouse Coordinates */

      Uint8 mB;               /**< Mouse Buttons */

      GLfloat initmX, initmZ; /**< Mouse Initial Positions on some States */

      /*! Gets wall on actual Mouse Position */
      muro* getWall();
      /*! Do the modifications in Horizontal Vertical Texture repeat */
      void doModifyVerHorTexture();
      /*! Do the modification on texture */
      void doTexture();
      /*! Do The Wall Construction */
      void doWall(bool X, bool Z, bool full);


};

#endif

