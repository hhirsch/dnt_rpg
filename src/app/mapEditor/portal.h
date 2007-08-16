#ifndef _portal_h
#define _portal_h

#include "../../map/map.h"
#include "area.h"
#include "message.h"
#include "../../map/mapobject.h"
#include "../../engine/util.h"


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

      /*! Verify if Point is inner the Square */
      bool inner(GLfloat ax, GLfloat az, GLfloat bx1, GLfloat bz1, 
                                        GLfloat bx2, GLfloat bz2);

      void defineDoor(mapObject* newDoor, string fileName);

      mapObject* getDoor();

      string getDoorFileName();


   private:
      Map* actualMap;         /**< Actual Internal Map */
      mapObject* actualDoor;  /**< Actual Door */
      string fileDoor;        /**< Actual Door File */
      int doorMode;           /**< Actual Door Mode */
      int doorOrientation;    /**< Actual Door Orientation */
      GLfloat doorX;          /**< Actual Door X position */
      GLfloat doorZ;          /**< Actual Door Z position */
      wall* doorWall;         /**< Actual Door Wall */
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
      /*! Add Portal To Map */
      void addPortal(int qx, int qz, string where);

      

};

#endif

