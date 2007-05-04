#ifndef _mapfog_h
#define _mapfog_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/*! Map Fog Class */
class mapFog
{
   public:
      GLfloat color[4]; /**< Fog Color */
      GLfloat density;  /**< Fog Density */
      GLfloat start;    /**< Fog Distant Start */
      GLfloat end;      /**< Fog Distant End */
      bool enabled;     /**< Fog Enabled? */
      string fileName;  /**< File name of the fog */

      /*! Constructor */
      mapFog();
      /*! Destructor */
      ~mapFog();
      /*! Load Fog from File
       * \param arq -> fog filename */
      void Load(string arq);
      /*! Save fog to file */
      bool save();
     
};

#endif

