#ifndef _maplights_h
#define _maplights_h

#include <SDL/SDL_opengl.h>
#include <string>
using namespace std;

#define MAX_LIGHTS_PER_MAP       20

/*! Map Lights Implementation \todo not working for now. */
class mapLight
{
   public:
      GLfloat light_ambient[4];   /**< Ambient Color Light */
      GLfloat light_diffuse[4];   /**< Diffuse Color Light */
      GLfloat light_specular[4];  /**< Specular Color Light */
      GLfloat light_position[4];  /**< Light position */
      GLfloat light_direction[4]; /**< Light Direction */

      GLfloat linearAtenuation;   /**< Linear Atenuation */
      GLfloat quadricAtenuation;  /**< Quadric Atenuation */
      GLfloat constantAtenuation; /**< Constant Atenuation */

      GLfloat cutOff;             /**< Spot CutOff */

      bool enableLight;      /**< Enable Light */
      bool enableDiffuse;    /**< Enable Diffuse Light */
      bool enableSpecular;   /**< Enable Specular Light */
      bool enableAmbient;    /**< Enable Ambient Light */
      bool enableSpot;       /**< Enable Spot Light */
      bool enableAtenuation; /**< Enable Linear Atenuation */

      /*! Constructor */
      mapLight();
      /*! Destructor */
      ~mapLight();
};

/*! The light Distance Class */
class lightDist
{
   public:
      int lightNumber;  /**< Internal Number of the Light */
      GLfloat distance; /**< Current Distance */
};

/*! Group of lights to a map */
class mapLights
{
   public:
      /*! Constructor */
      mapLights(); 
      /*! Destructor */
      ~mapLights();
      /*! Load Map Lights from file
       * \param arq -> filename of lights file */
      void Load(string arq);
      /*! Actualize Map Lights Position
       * \param posX -> active character X position
       * \param posZ -> active character Z position */
      void actualize(GLfloat posX, GLfloat posZ);

      /*! Get the Lights File Name
       * \return name of the file */
      string getFileName();

   private:
      /*! Set the active lights as the near lights
       * \param posX -> X position
       * \param posZ -> Z position */
      void setNearLights(GLfloat posX, GLfloat posZ);

      int totalLights;                       /**< Total Lights on Map */
      mapLight light[MAX_LIGHTS_PER_MAP];    /**< lights on Map */
      string fileName;                       /**< FileName */
      lightDist activeLights[3];             /**< Current Active Lights */
};

#endif

