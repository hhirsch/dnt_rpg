#ifndef _maplights_h
#define _maplights_h

#include <GL/gl.h>
#include <string>
using namespace std;


class mapLight
{
   public:
      GLfloat light_ambient[4];
      GLfloat light_diffuse[4];
      GLfloat light_specular[4];
      GLfloat light_position[4];
      GLfloat light_direction[4];
      
      bool enableLight;
      bool enableDiffuse;
      bool enableSpecular;
      bool enableAmbient;
      bool enableSpot;

      mapLight();
      ~mapLight();
};

class mapLights
{
   public:
      mapLight light[5];
      mapLights();
      ~mapLights();
      void Load(string arq);
};

#endif

