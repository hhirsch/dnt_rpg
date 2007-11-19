#ifndef _editor_h
#define _editor_h

#include "guiIO.h"
#include "terrain.h"
#include "portal.h"
#include "wall.h"
#include "message.h"
#include "objects.h"
#include "particles.h"
#include "npcs.h"
#include "../../engine/sun.h"
#include "../../engine/sky.h"
#include "../../engine/character.h"
#include "../../particle/partSystem.h"
#include "../../map/map.h"
#include "../../engine/culling.h"
#include "../../engine/util.h"
#include "../../etc/modellist.h"
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

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

      GLdouble xReal,yReal,zReal;

      sun* gameSun;                  /**< The Sun on Game */
      sky* gameSky;                  /**< The Sky on Game */
      GLfloat hour;                  /**< Current Sun Hour */
      
      Map* map;                      /**< The active Map */
      lObject* objectsList;          /**< The objects List */
      bool mapOpened;                /**< True if Active Map is Opened */
      int sizeX,                     /**< Real X size of the map */
          sizeZ;                     /**< Real Z size of the map */

      characterList *NPCs;             /**< List of NPCs On Map */
      featsList* features;           /**< NPCs Geral Features */
      
      partSystem* particleSystem;    /**< Particles on Map */
      
      guiIO* gui;                    /**< The Editor's GUI */
      terrain* terrainEditor;        /**< The terrain Editor Module */
      portal* portalEditor;          /**< The portal Editor Module */
      wallController* wallEditor;    /**< The wall Editor Module */
      objects* objectEditor;         /**< The Object Editor Module */
      particles* particleEditor;     /**< The Particle Editor Module */
      modelList* models;             /**< The Objects Model List */
      npcs* npcController;           /**< The Npcs Controller */
      weaponTypes* wTypes;           /**< The list of types */

      GLuint actualTexture;          /**< Actual Texture */

      
      GLdouble proj[16];             /**< Projection Matrix */
      GLdouble modl[16];             /**< Model View Matrix */
      GLint viewPort[4];             /**< View Port Vector */
      GLfloat visibleMatrix[6][4];   /**< Visible Matrix */
      
};

#endif

