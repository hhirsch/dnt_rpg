#ifndef _guiIO_H
#define _guiIO_H

#include "../../gui/farso.h"
#include "../../engine/camera.h"
#include "portal.h"
#include "terrain.h"
#include "listwindow.h"
#include "grasswindow.h"
#include "waterwindow.h"

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

/**! The Super GUI IO Class, to control things on map Editor! */
class guiIO 
{
   public:

      guiIO();
      ~guiIO();

      /*! Gets State seted by GUI IO.
       * \return Internal State */
      int getState();

      /*! Get Tool selected by GUI IO.
       * \return actual selected Tool */
      int getTool();

      /*! Set actual selected tool
       * \param toolID -> id of the new tool */
      void setTool(int toolID);


      /*! Do the GUI IO */
      int doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys,
               bool outdoor);

      /*! Draw The GUI */
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Redefine Camera Position
       * \acMap -> actual opened Map */
      void cameraPos(Map* acMap);

      /*! Gets the FileName */
      string getFileName();

      /*! Gets the Selected Text */
      string getSelectedText();

      /*! Gets the Texture File Name */
      string getTextureFileName();

      /*! Gets Object File Name */
      string getObjectFileName();

      /*! Show Message on Message Window */
      void showMessage(string msg);

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

      camera gameCamera;               /**< The Game Camera */

      guiInterface* getGui(){return(gui);};

   private:
      
      grassWindow* grWindow;/**< the grass window */ 
      waterWindow* wtWindow;/**< the waterfall window */
      listWindow* ltWindow; /**< The List Window */

      mapFog* actualFog;    /**< The actual fog */

      int state;            /**< Internal State */
      int tool;             /**< Selected Tool */
      string selectedText;  /**< Last selected text with list window */
      string curFileName;   /**< Current File Name */

      guiInterface* gui;    /**< Internal GUI */

      /**********************File*Window*******************************/
      window* fileWindow;      /**< File Selector Window (for load/save) */
      fileSel* fileSelector;   /**< The file selector itself */
      bool fileLoading;        /**< If is loading or saving */
      void openFileWindow(bool load);


      /**********************Act*Window*******************************/
      window* actWindow;     /**< Act Window */
      button* openButton;    /**< Open Button */
      button* saveButton;    /**< Save Button */
      button* exitButton;    /**< Exit Button */
      button* newButton;     /**< New Button */
      void openActWindow();


      /*********************Navigate*Window***************************/
      window* navWindow;              /**< Navigate Window */
      tabButton* navTabButton;        /**< Navigation Tab Button */
      oneTabButton* moreZoomButton;   /**< Navigation More Zoom Button */
      oneTabButton* lessZoomButton;   /**< Navigation Less Zoom Button */
      oneTabButton* upButton;         /**< Navigation Up Button */
      oneTabButton* downButton;       /**< Navigation Down Button */
      oneTabButton* leftButton;       /**< Navigation Left Button */
      oneTabButton* rightButton;      /**< Navigation Right Button */
      oneTabButton* rotUpButton;      /**< Navigation Rotation Up Button */
      oneTabButton* rotDownButton;    /**< Navigation Rotation Down Button */
      oneTabButton* rotLeftButton;    /**< Navigation Rotation Left Button */
      oneTabButton* rotRightButton;   /**< Navigation Rotation Right Button */
      void openNavWindow();

      /*********************Main*Window*******************************/
      window* mainWindow;   /**< Main Window */
      button* terrainButton; /**< Terrain Edition Button */
      button* wallButton;    /**< Wall Edition Button */
      button* portalButton;  /**< Portal Edition Button */
      button* objectButton;  /**< Objects Edition Button */
      button* npcButton;     /**< NPC Editon Button */
      button* particleButton;/**< Particle Edit Button */
      button* lightButton;   /**< Light Edit Button */
      button* fogButton;     /**< Fog Button */
      button* destroyButton; /**< Destroy Button */
      button* optionsButton; /**< Options Button */
      button* musicButton;   /**< Music Button */
      button* sndFxButton;   /**< Sound Effects Button */
      void openMainWindow();

      /*********************Texture*Window*******************************/
      window* textureWindow;           /**< Texture Window */
      button* texturePreviousButton;   /**< Previous Texture Button */
      button* textureNextButton;       /**< Next Texture Button */
      button* textureInsertButton;     /**< Insert Texture Button */
      void openTextureWindow();        /**< Open The texture Window */

      /*********************Terrain*Window*******************************/
      window* terrainWindow;               /**< Terrain Window */
      tabButton* terrainTabButton;         /**< Terrain TabButton */
      oneTabButton* terrainUpButton;       /**< Terrain Up Button */
      oneTabButton* terrainDownButton;     /**< Terrain Down Button */
      oneTabButton* terrainNivButton;      /**< Terrain Nivelation Button */
      oneTabButton* terrainWalkableButton; /**< Terrain Walkable Button */
      oneTabButton* terrainTextureButton;  /**< Terrain Texture Button */
      void openTerrainWindow();
      /*********************Wall*Window*******************************/
      window* wallWindow;                 /**< Wall Window */
      tabButton* wallTabButton;           /**< Wall Tab Button */
      oneTabButton* wallXButton;          /**< X Wall Button */
      oneTabButton* wallZButton;          /**< Z Wall Button */
      oneTabButton* wallX2Button;         /**< X Meio Fio Button */
      oneTabButton* wallZ2Button;         /**< Z Meio Fio Button */
      oneTabButton* wallPreviousButton;   /**< Wall Previous button */
      oneTabButton* wallDestroyButton;    /**< Wall Destroy Button */
      oneTabButton* wallNextButton;       /**< Wall Next Button */
      oneTabButton* wallTextureButton;    /**< Wall Texture Button */
      oneTabButton* wallLessYTexture;     /**< Texture Y Less Button */
      oneTabButton* wallMoreYTexture;     /**< Texture Y More Button */
      oneTabButton* wallLessXTexture;     /**< Texture X Less Button */
      oneTabButton* wallMoreXTexture;     /**< Texture X More Button */
      oneTabButton* wallLessZTexture;     /**< Texture Z Less Button */
      oneTabButton* wallMoreZTexture;     /**< Texture Z More Button */
      void openWallWindow();
      /*********************Portal*Window*******************************/
      window* portalWindow;               /**< Portal Window */
      tabButton* portalTabButton;         /**< Portal TabButton */
      oneTabButton* portalAddButton;      /**< Add Portal Button */
      oneTabButton* portalTagButton;      /**< Portal Tag Button */
      oneTabButton* portalDoorButton;     /**< Door Button */
      void openPortalWindow();
      /*********************Object*Window*******************************/
      window* objectWindow;               /**< Object Window */
      tabButton* objectTabButton;         /**< Object TabButton */
      oneTabButton* objectCommonButton;   /**< Object Common Button */
      oneTabButton* objectGunsButton;     /**< Object Guns Button */
      oneTabButton* objectBuildButton;    /**< Object Building Button */
      oneTabButton* objectCarsButton;     /**< Object Cars Button */
      oneTabButton* objectIcexButton;     /**< Object Icex Button */
      oneTabButton* objectNaturalButton;  /**< Object Natural Button */
      oneTabButton* objectCharButton;     /**< Object Character Button */
      oneTabButton* objectMacabreButton;  /**< Object Macabre Button */
      oneTabButton* objectBathButton;     /**< Object Bath Button */
      oneTabButton* objectStreetButton;   /**< Object Street Button */
      void openObjectWindow();      
      /*********************Particle*Window*******************************/
      window* particleWindow;             /**< Particle Window */
      tabButton* particleTabButton;       /**< Particles Tab Button */
      oneTabButton* fireButton;           /**< Particle Fire Button */
      oneTabButton* smokeButton;          /**< Particle Smoke Button */
      oneTabButton* snowButton;           /**< Particle Snow Button */
      oneTabButton* waterfallButton;      /**< Particle Waterfall Button */
      oneTabButton* waterSurfaceButton;   /**< Particle WaterSurface Button */
      oneTabButton* grassButton;          /**< Particle Grass Button */
      void openParticleWindow();
      /*********************Message*Window*******************************/
      window* messageWindow;              /**< Message Window */
      textBox* messageText;               /**< Message Text */
      void openMessageWindow(); 
      /**************************Fog*Window******************************/
      window* fogWindow;                  /**< Fog Window */
      cxSel* fogEnabled;                  /**< Fog enabled CxSel */
      textBar* fogColor[4];            /**< Fog Color Texts */
      textBar* fogDensity;             /**< Fog Density Text */
      textBar* fogStart;               /**< Fog Start Text */
      textBar* fogEnd;                 /**< Fog End Text */
      button* fogApplyButton;              /**< Fog Apply Button */
      void openFogWindow();               /**< Open Fog Window */
      
};

#endif

