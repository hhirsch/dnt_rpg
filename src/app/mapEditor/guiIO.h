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

#ifndef _dnt_map_editor_gui_io_h
#define _dnt_map_editor_gui_io_h

#include "../../gui/farso.h"
#include "../../engine/camera.h"
#include "portal.h"
#include "terrain.h"
#include "listwindow.h"
#include "grasswindow.h"
#include "waterwindow.h"
#include "nodeeditor.h"

/* Return Constants */
#define GUI_IO_NOTHING              0
#define GUI_IO_NEW_POSITION         1
#define GUI_IO_NEW_STATE            2
#define GUI_IO_NEW_MAP              3
#define GUI_IO_OPEN_MAP             4
#define GUI_IO_SAVE_MAP             5
#define GUI_IO_EXIT                 6
#define GUI_IO_OTHER                7
#define GUI_IO_TEXTURE_PREVIOUS     8
#define GUI_IO_TEXTURE_NEXT         9
#define GUI_IO_TEXTURE_INSERT      10
#define GUI_IO_SOUNDS              11

#define TOOL_NONE            0

/* State Constants */
#define GUI_IO_STATE_INITIAL   0
#define GUI_IO_STATE_TERRAIN   1
#define GUI_IO_STATE_WALL      2
#define GUI_IO_STATE_PORTAL    3
#define GUI_IO_STATE_OBJECTS   4
#define GUI_IO_STATE_PARTICLES 5
#define GUI_IO_STATE_NPCS      6
#define GUI_IO_STATE_MUSIC     7
#define GUI_IO_STATE_SOUNDS    8 
#define GUI_IO_STATE_TILE_WALL 9

/**! The Super GUI IO Class, to control things on map Editor! */
class guiIO 
{
   public:

      guiIO();
      ~guiIO();

      /*! Gets State seted by GUI IO.
       * \return Internal State */
      int getState();
      /*! Clear the current state */
      void clearState();

      /*! Get Tool selected by GUI IO.
       * \return actual selected Tool */
      int getTool();

      /*! Set actual selected tool
       * \param toolID -> id of the new tool */
      void setTool(int toolID);

      /*! Update the mouse world coordinates displayed */
      void updateMouseWorldCoordinates(float mX, float mZ);

      /*! Do the GUI IO */
      int doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys,
               bool outdoor);

      /*! Draw The GUI */
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Redefine Camera Position
       * \acMap -> actual opened Map */
      void cameraPos(Map* acMap);

      /*! Gets the FileName */
      std::string getFileName();

      /*! Gets the Selected Text */
      std::string getSelectedText();

      /*! Gets the Texture File Name */
      std::string getTextureFileName();

      /*! Gets Object File Name */
      std::string getObjectFileName();

      /*! Show Message on Message Window */
      void showMessage(std::string msg);

      /*! Set the map fog */
      void setFog(mapFog* fog);

      /*! Apply the fog! */
      void applyFog();

      /*! Get the grass Window
       * \return pointer to the grass window */
      grassWindow* getGrassWindow();
      
      /*! Get the water Window
       * \return pointer to the water window */
      waterWindow* getWaterWindow();

      /*! Get if is to invert the multitexture display
       * \return true or false */
      bool getInvertMultiTexture();

      camera gameCamera;               /**< The Game Camera */

      Farso::GuiInterface* getGui(){return(gui);};

      nodeEditor* getNodeEditor(){return(nodeEdit);};

   private:
      
      grassWindow* grWindow;/**< the grass window */ 
      waterWindow* wtWindow;/**< the waterfall window */
      listWindow* ltWindow; /**< The List Window */

      mapFog* actualFog;    /**< The actual fog */

      int state;            /**< Internal State */
      int tool;             /**< Selected Tool */
      bool invertMultiTexture; /**< Invert multi texture display or not */
      std::string selectedText;  /**< Last selected text with list window */
      std::string curFileName;   /**< Current File Name */

      Farso::GuiInterface* gui;    /**< Internal GUI */

      /**********************File*Window*******************************/
      Farso::Window* fileWindow; /**< File Selector Window (for load/save) */
      Farso::FileSel* fileSelector;   /**< The file selector itself */
      bool fileLoading;        /**< If is loading or saving */
      void openFileWindow(bool load);


      /**********************Act*Window*******************************/
      Farso::Window* actWindow;     /**< Act Window */
      Farso::Button* openButton;    /**< Open Button */
      Farso::Button* saveButton;    /**< Save Button */
      Farso::Button* exitButton;    /**< Exit Button */
      Farso::Button* newButton;     /**< New Button */
      void openActWindow();


      /*********************Navigate*Window***************************/
      Farso::Window* navWindow;              /**< Navigate Window */
      Farso::TabButton* navTabButton;        /**< Navigation Tab Button */
      Farso::OneTabButton* moreZoomButton;   /**< Navigation More Zoom Button */
      Farso::OneTabButton* lessZoomButton;   /**< Navigation Less Zoom Button */
      Farso::OneTabButton* upButton;         /**< Navigation Up Button */
      Farso::OneTabButton* downButton;       /**< Navigation Down Button */
      Farso::OneTabButton* leftButton;       /**< Navigation Left Button */
      Farso::OneTabButton* rightButton;      /**< Navigation Right Button */
      Farso::OneTabButton* rotUpButton;   /**< Navigation Rotation Up Button */
      Farso::OneTabButton* rotDownButton;/**< Navigation Rotation Down Button */
      Farso::OneTabButton* rotLeftButton;/**< Navigation Rotation Left Button */
      Farso::OneTabButton* rotRightButton;/**< Navigation RotationRightButton */
      void openNavWindow();

      /*********************Main*Window*******************************/
      Farso::Window* mainWindow;   /**< Main Window */
      Farso::Button* terrainButton; /**< Terrain Edition Button */
      Farso::Button* wallButton;    /**< Wall Edition Button */
      Farso::Button* portalButton;  /**< Portal Edition Button */
      Farso::Button* objectButton;  /**< Objects Edition Button */
      Farso::Button* npcButton;     /**< NPC Editon Button */
      Farso::Button* particleButton;/**< Particle Edit Button */
      Farso::Button* lightButton;   /**< Light Edit Button */
      Farso::Button* fogButton;     /**< Fog Button */
      Farso::Button* destroyButton; /**< Destroy Button */
      Farso::Button* optionsButton; /**< Options Button */
      Farso::Button* musicButton;   /**< Music Button */
      Farso::Button* sndFxButton;   /**< Sound Effects Button */
      void openMainWindow();

      /*********************Texture*Window*******************************/
      Farso::Window* textureWindow;           /**< Texture Window */
      Farso::Button* texturePreviousButton;   /**< Previous Texture Button */
      Farso::Button* textureNextButton;       /**< Next Texture Button */
      Farso::Button* textureInsertButton;     /**< Insert Texture Button */
      void openTextureWindow();        /**< Open The texture Window */

      /*********************Terrain*Window*******************************/
      Farso::Window* terrainWindow;               /**< Terrain Window */
      Farso::TabButton* terrainTabButton;         /**< Terrain TabButton */
      Farso::OneTabButton* terrainUpButton;       /**< Terrain Up Button */
      Farso::OneTabButton* terrainDownButton;     /**< Terrain Down Button */
      Farso::OneTabButton* terrainNivButton;  /**< Terrain Nivelation Button */
      Farso::OneTabButton* terrainTextureButton;  /**< Terrain Texture Button */
      void openTerrainWindow();
      /*********************Wall*Window*******************************/
      Farso::Window* wallWindow;                 /**< Wall Window */
      Farso::TabButton* wallTabButton;           /**< Wall Tab Button */
      Farso::OneTabButton* wallAddButton;        /**< Add Wall Button */
      Farso::OneTabButton* wallEditButton;       /**< Edit Wall Button */
      Farso::OneTabButton* wallCutButton;        /**< X Meio Fio Button */
      Farso::OneTabButton* wallDestroyButton;    /**< Wall Destroy Button */
      Farso::OneTabButton* wallTextureButton;    /**< Wall Texture Button */
      Farso::OneTabButton* wallLessYTexture;     /**< Texture Y Less Button */
      Farso::OneTabButton* wallMoreYTexture;     /**< Texture Y More Button */
      Farso::OneTabButton* wallLessXTexture;     /**< Texture X Less Button */
      Farso::OneTabButton* wallMoreXTexture;     /**< Texture X More Button */
      Farso::OneTabButton* wallLessZTexture;     /**< Texture Z Less Button */
      Farso::OneTabButton* wallMoreZTexture;     /**< Texture Z More Button */
      void openWallWindow();
      /*********************Tile*Wall*Window**************************/
      Farso::Window* tileWallWindow;             /**< TileWall Window */
      Farso::TabButton* tileWallTabButton;       /**< TileWall Tab Button */
      Farso::OneTabButton* tileWallAddButton;    /**< Add tile walls Button */
      Farso::OneTabButton* tileWallRemoveButton; /**< Remove tile Button */
      Farso::OneTabButton* tileWallChangeButton; /**< Change tile Button */
      void openTileWallWindow();
      /*********************Portal*Window*******************************/
      Farso::Window* portalWindow;               /**< Portal Window */
      Farso::TabButton* portalTabButton;         /**< Portal TabButton */
      Farso::OneTabButton* portalAddButton;      /**< Add Portal Button */
      Farso::OneTabButton* portalTagButton;      /**< Portal Tag Button */
      Farso::OneTabButton* portalDoorButton;     /**< Door Button */
      void openPortalWindow();
      /*********************Object*Window*******************************/
      Farso::Window* objectWindow;               /**< Object Window */
      Farso::TabButton* objectTabButton;         /**< Object TabButton */
      Farso::OneTabButton* objectCommonButton;   /**< Object Common Button */
      Farso::OneTabButton* objectGunsButton;     /**< Object Guns Button */
      Farso::OneTabButton* objectBuildButton;    /**< Object Building Button */
      Farso::OneTabButton* objectCarsButton;     /**< Object Cars Button */
      Farso::OneTabButton* objectIcexButton;     /**< Object Icex Button */
      Farso::OneTabButton* objectNaturalButton;  /**< Object Natural Button */
      Farso::OneTabButton* objectCharButton;     /**< Object Character Button */
      Farso::OneTabButton* objectMacabreButton;  /**< Object Macabre Button */
      Farso::OneTabButton* objectBathButton;     /**< Object Bath Button */
      Farso::OneTabButton* objectStreetButton;   /**< Object Street Button */
      Farso::OneTabButton* objectJunkButton;     /**< Objects Junk Button */
      Farso::OneTabButton* objectSelectButton;   /**< Objects Select Button */
      void openObjectWindow();      
      /*********************Particle*Window*******************************/
      Farso::Window* particleWindow;             /**< Particle Window */
      Farso::TabButton* particleTabButton;       /**< Particles Tab Button */
      Farso::OneTabButton* fireButton;           /**< Particle Fire Button */
      Farso::OneTabButton* smokeButton;          /**< Particle Smoke Button */
      Farso::OneTabButton* snowButton;           /**< Particle Snow Button */
      Farso::OneTabButton* waterfallButton;   /**< Particle Waterfall Button */
      Farso::OneTabButton* waterSurfaceButton;/**< ParticleWaterSurfaceButton */
      Farso::OneTabButton* grassButton;       /**< Particle Grass Button */
      void openParticleWindow();
      /************************Sound*Window*******************************/
      Farso::Window* soundWindow;                /**< Sound Window */
      Farso::TabButton* soundTabButton;          /**< sound TabButton */
      Farso::OneTabButton* soundAddButton;       /**< Sound Add Button */
      Farso::OneTabButton* soundRemoveButton;    /**< Sound Remove Button */
      Farso::OneTabButton* soundEditButton;      /**< Sound Edit Button */
      void openSoundWindow();
      /*********************Message*Window*******************************/
      Farso::Window* messageWindow;              /**< Message Window */
      Farso::TextBox* messageText;               /**< Message Text */
      Farso::TextBox* mouseCoordText;            /**< Mouse Coordinate text */
      void openMessageWindow(); 
      /**************************Fog*Window******************************/
      Farso::Window* fogWindow;                  /**< Fog Window */
      Farso::CxSel* fogEnabled;                  /**< Fog enabled CxSel */
      Farso::TextBar* fogColor[4];            /**< Fog Color Texts */
      Farso::TextBar* fogDensity;             /**< Fog Density Text */
      Farso::TextBar* fogStart;               /**< Fog Start Text */
      Farso::TextBar* fogEnd;                 /**< Fog End Text */
      Farso::Button* fogApplyButton;              /**< Fog Apply Button */
      void openFogWindow();               /**< Open Fog Window */
     

      nodeEditor* nodeEdit;          /**< Scene node editor */
};

#endif

