/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
  This file is part of DccNiTghtmare.
 
  DccNiTghtmare is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DccNiTghtmare is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DccNiTghtmare.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _dnt_map_editor_editor_h
#define _dnt_map_editor_editor_h

#include "guiIO.h"
#include "terrain.h"
#include "portal.h"
#include "wall.h"
#include "message.h"
#include "objects.h"
#include "particles.h"
#include "npcs.h"
#include "nodeeditor.h"

#include "../../etc/scene.h"
#include "../../engine/sun.h"
#include "../../engine/cursor.h"
#include "../../engine/sky.h"
#include "../../engine/character.h"
#include "../../particle/partcontroller.h"
#include "../../map/map.h"
#include "../../engine/culling.h"
#include "../../engine/util.h"
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
      /*! Cloase Active Editor Map */
      void closeMap();
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
      int insertTexture(std::string textureFile);
      /*! Render All Static Scenery Objects */
      void renderSceneryObjects();
      /*! Update the mouse projection on axys XZ */
      void updateMouseFloorPos();

      SDL_Surface *screen;           /**< SDL Screen */
      Uint8* keys;                   /**< Keyboard State */
      Uint8 mButton;                 /**< Mouse Button State */
      cursor curs;                   /**< The Mouse Cursor */
      int mouseX,                    /**< Mouse X position */
          mouseY;                    /**< Mouse Y position */
      bool quit;                     /**< Quit Editor Flag */

      GLdouble xReal,yReal,zReal;
      GLfloat xFloor, zFloor; 

      sun* gameSun;                  /**< The Sun on Game */
      sky* gameSky;                  /**< The Sky on Game */
      GLfloat hour;                  /**< Current Sun Hour */
      
      Map* map;                      /**< The active Map */
      bool mapOpened;                /**< True if Active Map is Opened */
      int sizeX,                     /**< Real X size of the map */
          sizeZ;                     /**< Real Z size of the map */

      characterList *NPCs;             /**< List of NPCs On Map */
      featsList* features;           /**< NPCs Geral Features */
      
      partController* particleSystem;    /**< Particles on Map */
      
      guiIO* gui;                    /**< The Editor's GUI */
      terrain* terrainEditor;        /**< The terrain Editor Module */
      portal* portalEditor;          /**< The portal Editor Module */
      wallController* wallEditor;    /**< The wall Editor Module */
      objects* objectEditor;         /**< The Object Editor Module */
      particles* particleEditor;     /**< The Particle Editor Module */
      scene curScene;                /**< The Scene */
      npcs* npcController;           /**< The Npcs Controller */
      weaponTypes wTypes;            /**< The list of types */

      GLuint curTexture;             /**< Current Texture */
      std::string curTextureName;    /**< Current Texture Name */
      
      GLdouble proj[16];             /**< Projection Matrix */
      GLdouble modl[16];             /**< Model View Matrix */
      GLint viewPort[4];             /**< View Port Vector */
      GLfloat** visibleMatrix;       /**< Visible Matrix */
};

#endif

