#ifndef _sun_h
#define _sun_h

/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <SDL/SDL_opengl.h>

/*! The class sun represents the global light simulating the SUN on game */
class sun
{
   public:
      /*! Constructor 
       * \param hour -> current hour of day 
       * \param farViewX -> far view X position
       * \param farViewZ -> far view Z position */
      sun(float hour, float farViewX, float farViewZ);
      /*! Destructor*/
      ~sun();

      /*! Actualize Sun Itensity, based on hour of the day 
       * \param hour -> current hour of day
       * \param posX -> character X position
       * \param posZ -> character Z position */
      void actualizeHourOfDay(float hour, float posX, float posZ);

      /*! Draw the Sun to its position */
      void drawSun();

      /*! Get The Sun rotation
       * \return sun rotation value */
      GLfloat getRotation();

      /*! Get The Sun Position
       * \param pos -> variable to get the sun position */
      void getPosition(GLfloat pos[4]);

      /*! Set the sun light to the current state */
      void setLight();

      /*! Multiply the Matrix by the currenct Shadow Matrix. */
      void mulShadowMatrix();

   private:
      GLfloat quadricAttenuation;   /**< Quadric Attenuation Light Value */
      GLfloat constantAttenuation;  /**< Constant Attenuation Light Value */
      GLfloat linearAttenuation;    /**< Linear Attenuation Light Value */

      GLfloat actualColor[4];       /**< Actual Sun Emiting Color */
      GLfloat where[4];             /**< Actual Sun Sky Position */
      GLfloat rotation;             /**< Actual Sun Spin to surface */

      GLfloat shadowMat[4][4];      /**< Actual Shadow Matrix */

      GLuint  sunTexture;           /**< Current Sun Texture */
      GLuint  moonTexture;          /**< Current Moon Texture */

      float curHour;                /**< Current Hour of Day to Sun */

      /*! Set the current Postion of the sun, based on hour of the day
       * \param posX -> character X position
       * \param posZ -> character Z position*/
      void positionOnHour(float posX, float posZ);

      /*! Set the current Color of the sun, based on the rotation */
      void colorOnHour();

      /*! Define the shadow matrix */
      void defineShadowMatrix();

      /*! Verifies if the sun is on visible time (born -> death)
       *  \return true if is in visible time, false otherwise.*/
      bool visibleTime();
};

#endif

