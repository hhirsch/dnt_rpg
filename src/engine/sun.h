/*  DccNiTghtmare is Public Domain, do anything with this code. */
#ifndef _sun_h
#define _sun_h

#include <SDL/SDL_opengl.h>

/*! The class sun represents the global light simulating the SUN on game */
class sun
{
   public:
      /*! Constructor 
       * \param hour -> current hour of day 
       * \param farViewX -> far view X position
       * \param farViewZ -> far view Z position */
      sun(float hour, int farViewX, int farViewZ);
      /*! Destructor*/
      ~sun();

      /*! Actualize Sun Itensity, based on hour of the day 
       * \param hour -> current hour of day */
      void actualizeHourOfDay(float hour);

   private:
      GLfloat quadricAttenuation;   /**< Quadric Attenuation Light Value */
      GLfloat constantAttenuation;  /**< Constant Attenuation Light Value */
      GLfloat linearAttenuation;    /**< Linear Attenuation Light Value */

      GLfloat actualColor[4];       /**< Actual Sun Emiting Color */
      GLfloat where[4];             /**< Actual Sun Sky Position */
      GLfloat rotation;             /**< Actual Sun Spin to surface */

      float curHour;                /**< Current Hour of Day to Sun */

      /*! Set the current Postion of the sun, based on hour of the day */
      void positionOnHour();

      /*! Set the current Color of the sun, based on the rotation */
      void colorOnHour();


      /*! Verifies if the sun is on visible time (born -> death)
       *  \return true if is in visible time, false otherwise.*/
      bool visibleTime();
};

#endif

