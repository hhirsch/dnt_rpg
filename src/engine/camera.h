#ifndef _camera_h
#define _camera_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#define CAMERA_TYPE_NORMAL 1  /**< Normal Camera Mode */
#define CAMERA_TYPE_DRIVE  2  /**< Drive Camera Mode */

/*! The Camera Manipulation Class */
class camera
{
   public:
      /*! Constructor */
      camera();
      /*! Destructor */
      ~camera();

      /*! Do the IO of the Camera 
       * \param keys -> pointer to the SDL Keys
       * \param mBotao -> mouse Button state
       * \param x -> mouse X Screen position
       * \param y -> mouse Y Screen position
       * \param varCamera -> how much the camera moves
       * \return TRUE if camera modified, false otherwise */
      bool doIO(Uint8 *keys, Uint8 mBotao, int x, int y, GLfloat varCamera);

      /*! Get The Camera X Postion
       * \return Camera X position */
      GLfloat getCameraX();
      /*! Get The Camera Y Postion
       * \return Camera Y position */
      GLfloat getCameraY();
      /*! Get The Camera Z Postion
       * \return Camera Z position */
      GLfloat getCameraZ();

      /*! Sum Value to Current Camera Theta
       * \param f -> value to sum */
      void sumTheta(GLfloat f);
      /*! Sum Value to Current Camera Phi
       * \param f -> value to sum */
      void sumPhi(GLfloat f);
      /*! Sum Value to Current Camera D (Zoom)
       * \param f -> value to sum */
      void sumD(GLfloat f);

      /*! Get The Camera Phi
       * \return Camera Phi */
      GLfloat getPhi();
      /*! Get The Camera Theta
       * \return Camera Theta */
      GLfloat getTheta();
      /*! Get The Camera DeltaY
       * \return Camera DeltaY */
      GLfloat getDeltaY();
      /*! Get The Camera D (Zoom)
       * \return Camera D (Zoom) */
      GLfloat getD();
      
      /*! Get The Camera Center X Postion
       * \return Camera Center X position */
      GLfloat getCenterX();
      /*! Get The Camera Center Y Postion
       * \return Camera Center Y position */
      GLfloat getCenterY();
      /*! Get The Camera Center Z Postion
       * \return Camera Center Z position */
      GLfloat getCenterZ();

      /*! Do the camera Look At */
      void lookAt();

      /*! Define new Camera Type
       * \param value -> value of the new camera type */
      void defineCameraType(int value);

      /*! Actualize Camera Position
       * \param characterX -> character X position 
       * \param characterY -> character Y position
       * \param characterZ -> character Z position
       * \param characterOrientation -> character Orientation */
      void actualizeCamera(GLfloat characterX, GLfloat characterY, 
                           GLfloat characterZ, GLfloat characterOrientation);


   private:
      GLfloat theta;                    /**< Rotation Up*/
      GLfloat phi;                      /**< Rotation to Sides*/
      GLfloat d;                        /**< Zoom */
      GLfloat cameraX,                  /**< Camera X Position */
              cameraY,                  /**< Camera Y Position */
              cameraZ;                  /**< Camera Z Position */
      GLfloat deltaY;                   /**< Camera DeltaY */
      GLfloat centerX,                  /**< Camera Central X Position */
              centerY,                  /**< Camera Central Y Position */
              centerZ;                  /**< Camera Central Z Position */

      bool middleMouse;                 /**< Middle Mouse Camera Control */  
      int prevX,                        /**< Previous Mouse X position */
          prevY;                        /**< Previous Mouse Z position */

      int type;                         /**< Internal Camera Type */

      GLfloat zoomAc;                   /**< Zoom Acceleration Value */
      GLfloat phiAc;                    /**< Phi Acceleration Value */
      GLfloat thetaAc;                  /**< Theta Acceleration Value */
};

#endif

