#ifndef _objects_h
#define _objects_h

#include "../../map/map.h"
#include "../../map/mapobject.h"
#include "../../etc/modellist.h"
#include "../../engine/util.h"
#include "message.h"

/*! The objects Map Controller Class */
class objects
{
   public:
      /*! Constructor
       * \param acMap -> poiter to opened map*/
      objects(Map* acMap, modelList* usedModels);
      /*! Destructor */
      ~objects();

      /*! Verify and do action by mouse and state 
       * \param keys -> current keyboard state
       * \param mouseX -> mouse X coordinate
       * \param mouseY -> mouse Y coordinate 
       * \param mouseZ -> mouse Z coordinate 
       * \param mButton -> mouse button state
       * \param tool -> current Tool*/
      void verifyAction(Uint8* keys, GLfloat mouseX, GLfloat mouseY, 
                        GLfloat mouseZ, 
                        Uint8 mButton, int mouseXw, int mouseYw, int tool,
                        GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! If have some temporary things to draw, draw it! */
      void drawTemporary();

      void defineActualObject(object* obj, string fileName);

      string getObjectFileName();

    private:
      modelList* models;          /**< Used Model List */
      Map* actualMap;             /**< Actual Internal Map */
      object* actualObstacle;     /**< Actual Obstacle */
      string objectFile;          /**< Actual Obstacle Filename */
      int actualTool;             /**< Actual Tool */
      int state;                  /**< Actual Internal State */

      GLfloat mX, mY, mZ;         /**< Mouse Coordinates */

      Uint8 mB;                   /**< Mouse Buttons */

      GLfloat obstacleX,          /**< X position Obstacle */
              obstacleY,          /**< Y position Obstacle */
              obstacleZ;          /**< Z position Obstacle */
      GLint obstacleOrientation;  /**< Orientation of the Obstacle */

      void insertObject(GLfloat xReal, GLfloat zReal, int orObj,
                        Map* acMap, object* obj, int qx, int qz);

};

#endif

