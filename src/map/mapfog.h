#ifndef _mapfog_h
#define _mapfog_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

class mapFog
{
   public:
      GLfloat color[4];
      GLfloat density;
      GLfloat start;
      GLfloat end;
      bool enabled;
      string fileName;

      mapFog();
      ~mapFog();
      void Load(string arq);
     
};

#endif

