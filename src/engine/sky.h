#ifndef _sky_h
#define _sky_h

#include "../map/map.h"
#include <SDL/SDL_opengl.h>

/*! Sky Control Class */
class sky
{
   public:
      /*! Constructor */
      sky();
      /*! Destructor */
      ~sky();

      /*! Draw Sky Dome 
       * \param actualMap -> pointer to the opened Map 
       * \param time -> current time of day */
      void draw(Map* actualMap, GLfloat time);

   private:

      /*! Draw Sky Dome
       * \param lats -> number of latitudes
       * \param longs -> number of longitudes */
      void drawDome(int lats, int longs);

      GLuint skyMap;
      GLfloat solarTime;
      GLfloat color[3];

};


#endif

