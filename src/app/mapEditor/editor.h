#ifndef _editor_h
#define _editor_h

#include "guiIO.h"
#include "terrain.h"
#include "portal.h"
#include "wall.h"
#include "message.h"
#include "objects.h"
#include "particles.h"
#include "../../engine/sun.h"
#include "../../engine/personagens.h"
#include "../../particle/partSystem.h"
#include "../../map/map.h"
#include "../../engine/culling.h"
#include "../../engine/util.h"
#include "../../etc/glm.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ACTUALIZATION_RATE 20
#define WALK_ACTUALIZATION 0.02

/*! The Editor Class */
class editor
{
   public:
      /*! Editor Constructor */
      editor();
      /*! Editor Destructor */
      ~editor();
      /*! Run Editor Main Loop */
      void run();
   private:
      /*! Remensionate Window */
      void redmensionateWindow();
      /*! Init Editor Graphs */
      void init();
      /*! Open Active Editor Map */
      void openMap();
      /*! Save Active Editor Map */
      void saveMap();
      /*! Make a new Active Editor Map */
      void newMap();
      /*! Verify if Camera Position is Valid, seting it to a valid if not. */
      void verifyPosition();
      /*! Draw Editor Screen */
      void draw();
      /*! Verifies The IO Events on Editor */
      void verifyIO();
      /*! Do the IO on Editor Map */
      void doEditorIO();
      /*! Gets Previous Texture on List */
      int previousTexture();
      /*! Gets Next Texture on Map List */
      int nextTexture();
      /*! Insert Texture on Map List */
      int insertTexture();

      SDL_Surface *screen;           /**< SDL Screen */
      Uint8* keys;                   /**< Keyboard State */
      Uint8 mButton;                 /**< Mouse Button State */
      int mouseX,                    /**< Mouse X position */
          mouseY;                    /**< Mouse Y position */
      bool quit;                     /**< Quit Editor Flag */

      sun* gameSun;                  /**< The Sun on Game */
      GLfloat hour;                  /**< Current Sun Hour */
      
      Map* map;                      /**< The active Map */
      bool mapOpened;                /**< True if Active Map is Opened */

      Lpersonagem *NPCs;             /**< List of NPCs On Map */
      featsList* features;           /**< NPCs Geral Features */
      
      partSystem* particleSystem;    /**< Particles on Map */
      
      guiIO* gui;                    /**< The Editor's GUI */
      terrain* terrainEditor;        /**< The terrain Editor Module */
      portal* portalEditor;          /**< The portal Editor Module */
      wall* wallEditor;              /**< The wall Editor Module */
      objects* objectEditor;         /**< The Object Editor Module */
      particles* particleEditor;     /**< The Particle Editor Module */

      GLuint actualTexture;          /**< Actual Texture */

      
      GLdouble proj[16];             /**< Projection Matrix */
      GLdouble modl[16];             /**< Model View Matrix */
      GLint viewPort[4];             /**< View Port Vector */
      GLfloat visibleMatrix[6][4];   /**< Visible Matrix */
      
};

#endif

