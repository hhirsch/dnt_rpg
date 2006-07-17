#ifndef _maplights_h
#define _maplights_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

/**> Map Lights Implementation \todo not working for now. */
class mapLight
{
   public:
      GLfloat light_ambient[4];   /**< Ambient Color Light */
      GLfloat light_diffuse[4];   /**< Diffuse Color Light */
      GLfloat light_specular[4];  /**< Specular Color Light */
      GLfloat light_position[4];  /**< Light position */
      GLfloat light_direction[4]; /**< Light Direction */
      
      bool enableLight;   /**< Enable Light */
      bool enableDiffuse; /**< Enable Diffuse Light */
      bool enableSpecular;/**< Enable Specular Light */
      bool enableAmbient; /**< Enable Ambient Light */
      bool enableSpot;    /**< Enable Spot Light */

      /*! Constructor */
      mapLight();
      /*! Destructor */
      ~mapLight();
};

/*! Conjunt of lights to a map */
class mapLights
{
   public:
      mapLight light[5];    /**< Total number of lights */

      /*! Constructor */
      mapLights(); 
      /*! Destructor */
      ~mapLights();
      /*! Load Map Lights from file
       * \param arq -> filename of lights file */
      void Load(string arq);
};

#endif

