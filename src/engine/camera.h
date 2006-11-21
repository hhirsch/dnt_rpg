#ifndef _camera_h
#define _camera_h

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

/*! The Camera Manipulation Class */
class camera
{
   public:
      /*! Constructor */
      camera();
      /*! Destructor */
      ~camera();

      /*! Do the IO of the Camera */
      bool doIO(Uint8 *keys, Uint8 mBotao, int x, int y, GLfloat varCamera);

      GLfloat getCameraX();
      GLfloat getCameraY();
      GLfloat getCameraZ();

      void sumTheta(GLfloat f);
      void sumPhi(GLfloat f);
      void sumD(GLfloat f);

      GLfloat getPhi();


      GLfloat centerX,                  /**< Camera Central X Position */
              centerY,                  /**< Camera Central Y Position */
              centerZ;                  /**< Camera Central Z Position */

      void lookAt();


   private:
      GLfloat theta;                    /**< Rotation Up*/
      GLfloat phi;                      /**< Rotation to Sides*/
      GLfloat d;                        /**< Zoom */
      GLfloat cameraX,                  /**< Camera X Position */
             cameraY,                   /**< Camera Y Position */
             cameraZ;                   /**< Camera Z Position */
      GLfloat deltaY;                   /**< Camera DeltaY */

      bool middleMouse;                 /**< Middle Mouse Camera Control */  
      int prevX,                        /**< Previous Mouse X position */
          prevY;                        /**< Previous Mouse Z position */
};

#endif

