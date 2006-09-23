#ifndef _terrain_h
#define _terrain_h

#include "../../map/map.h"

#define TOOL_TERRAIN_UP       11
#define TOOL_TERRAIN_NIVELATE 12
#define TOOL_TERRAIN_DOWN     13
#define TOOL_TERRAIN_TEXTURE  14

class terrain
{
   public:
      /*! Constructor
       * \param map -> poiter to opened map */
      terrain(Map* map);
      /*! Destructor */
      ~terrain();

      /*! Verify and do action by mouse and state 
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool
       * \param actualTexture -> current Texture */
      void verifyAction(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                        Uint8 mButton, int tool, GLuint actualTexture);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

   private:
      Map* actualMap;   /**< Actual Internal Map */
      int actualTool;   /**< Actual Tool */
      int state;        /**< Actual Internal State */

      int quadX;        /**< Mouse Mouse Square X Coordinate */
      int quadZ;        /**< Mouse Mouse Square Z Coordinate */

      void doUpDown(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                    Uint8 mButton);
      void doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                      Uint8 mButton);
      void doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
                     Uint8 mButton, GLuint actualTexture );
};


#endif

