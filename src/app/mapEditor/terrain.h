#ifndef _terrain_h
#define _terrain_h

#include "../../map/map.h"
#include "message.h"


class terrain
{
   public:
      /*! Constructor
       * \param acMap -> pointer to opened map */
      terrain(Map* acMap);
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

      int quadInitX;    /**< X quad value at init of some state */
      int quadInitZ;    /**< Y quad value at init of some state */

      GLfloat height;      /**< Current Map Height On Mouse */
      GLfloat nivelHeight; /**< Map Height at init of Nivelate */

      GLfloat mX, mY, mZ;  /**< Mouse Positions */
 
      GLfloat initmX, initmY, initmZ; /**< Mouse Positions On Init of State */

      /*! Do up Down on Terrain */
      void doUpDown(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                    Uint8 mButton);
      /*! Do Nivelate on Terrain */
      void doNivelate(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ, 
                      Uint8 mButton);
      /*! Do Change Texture on Terrain */
      void doTexture(GLfloat mouseX, GLfloat mouseY, GLfloat mouseZ,
                     Uint8 mButton, GLuint actualTexture );
      /*! Verifies if point is in Square */
      bool pointInSquare(GLfloat x, GLfloat y, 
                            GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
};


#endif

