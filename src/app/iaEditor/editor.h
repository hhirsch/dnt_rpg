#ifndef _editor_h
#define _editor_h

#include "guiIO.h"
#include "message.h"
#include "agents.h"
#include "../../map/map.h"
#include "../../engine/culling.h"
#include "../../engine/util.h"
#include "../../engine/sun.h"
#include "../../particle/partSystem.h"
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
      /*! Open Active Map */
      void openMap();
      /*! New Active IA file */
      void newIAFile();
      /*! Save Active IA File */
      void saveIAFile();
      /*! Open Active IA File */
      void openIAFile();
      /*! Verify if Camera Position is Valid, seting it to a valid if not. */
      void verifyPosition();
      /*! Draw Editor Screen */
      void draw();
      /*! Verifies The IO Events on Editor */
      void verifyIO();
      /*! Do the IO on Editor Map */
      void doEditorIO();

      SDL_Surface *screen;           /**< SDL Screen */
      Uint8* keys;                   /**< Keyboard State */
      Uint8 mButton;                 /**< Mouse Button State */
      int mouseX,                    /**< Mouse X position */
          mouseY;                    /**< Mouse Y position */
      bool quit;                     /**< Quit Editor Flag */

      sun* gameSun;                  /**< The Sun on Game */
      GLfloat hour;                  /**< Current Sun Hour */
      
      Map* map;                      /**< The active Map */
      mapObjeto* actualObject;       /**< The actual Map Object */
      bool mapOpened;                /**< True if Active Map is Opened */

      bool simulationStarted;        /**< True if simulation started */

      //Lpersonagem *NPCs;             /**< List of NPCs On Map */
      //featsList* features;           /**< NPCs Geral Features */
      
      partSystem* particleSystem;    /**< Particles on Map */
      bool enableParticles;          /**< Enable Particles on Editor */
      
      guiIO* gui;                    /**< The Editor's GUI */

      GLuint actualTexture;          /**< Actual Texture */

      GLdouble proj[16];             /**< Projection Matrix */
      GLdouble modl[16];             /**< Model View Matrix */
      GLint viewPort[4];             /**< View Port Vector */
      GLfloat visibleMatrix[6][4];   /**< Visible Matrix */

      agents* agentsSimulation;
      
};

#endif

