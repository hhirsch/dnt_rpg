#ifndef _portal_h
#define _portal_h

#include "../../map/map.h"
#include "area.h"

#define TOOL_PORTAL_ADD  31
#define TOOL_PORTAL_TAG  32

class portal
{
   public:
      /*! Constructor
       * \param map -> poiter to opened map */
      portal(Map* map);
      /*! Destructor */
      ~portal();

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
      Map* actualMap;         /**< Actual Internal Map */
      int actualTool;         /**< Actual Tool */
      int state;              /**< Actual Internal State */

      GLfloat mX, mY, mZ;     /**< Mouse Coordinates */

      Uint8 mB;               /**< Mouse Buttons */

      GLfloat initmX, initmZ; /**< Mouse Initial Positions on some States */

      areaList* portalList;   /**< Portal List */

      /*! Internal Add Portal */
      void doAddPortal();
      /*! Internal Tag Portal */
      void doTagPortal(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);
      

};

#endif

