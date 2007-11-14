#ifndef _wind_h
#define _wind_h

#include <SDL/SDL_opengl.h>

/*! The wind controller class  */
class wind
{
   public:
      /*! The wind constructor */
      wind();
      /*! The window destructor */
      ~wind();

      /*! Set the wind velocity
       * \param vX -> wind X velocity 
       * \param vY -> wind Y velocity
       * \param vZ -> wind Z velocity */
      void setVelocity(GLfloat vX, GLfloat vY, GLfloat vZ);

      /*! Get the wind velocity
       * \param vX -> wind X velocity 
       * \param vY -> wind Y velocity
       * \param vZ -> wind Z velocity */
      void getVelocity(GLfloat& vX, GLfloat& vY, GLfloat& vZ);

   protected:
      GLfloat velX,           /**< Current wind X velocity */
              velY,           /**< Current wind Y velocity */
              velZ;           /**< Current wind Z velocity */
};

#endif

