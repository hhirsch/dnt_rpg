#ifndef _shadow_h
#define _shadow_h

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <string>
using namespace std;

#include "camera.h"

/*! The Shadow Controller Class */
class shadow
{
   public:
      /*! Constructor */
      shadow();
      /*! Destructor */
      ~shadow();

      /*! Verify if the Shadows is Avaible
       * \return true -> if is avaible */
      bool isEnable();

      /*! Enable or not the shadows
       * \param en -> enable shadows (will be AND with avaible) */
      void setEnable(bool en);

      /*! Init the Shadow Internal Structs */
      void init();

      /*! Define the Light View */
      void defineLightView(GLfloat pX, GLfloat pY, GLfloat pZ);

      /*! Define the Camera View */
      void defineCameraView(camera& cam, GLdouble proj[16],GLdouble modl[16]);

      /*! Save the shadow map to the texture */
      void saveShadowMap();

   private:
      bool avaible;   /**< Flag if the shadow is avaible or not */
      bool enable;    /**< Enable the shadows or not */

      GLuint shadowMapTexture;  /**< The ShadowMap Texture */

};


#endif

