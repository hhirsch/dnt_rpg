#ifndef _guiIO_H
#define _guiIO_H

#include "../../gui/farso.h"
#include "../../engine/camera.h"
#include "portal.h"
#include "terrain.h"

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
#define GUI_IO_OBJECT_INSERT       11

#define TOOL_NONE            0

/* State Constants */
#define GUI_IO_STATE_INITIAL   0
#define GUI_IO_STATE_TERRAIN   1
#define GUI_IO_STATE_WALL      2
#define GUI_IO_STATE_PORTAL    3
#define GUI_IO_STATE_OBJECTS   4
#define GUI_IO_STATE_PARTICLES 5

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


      /*! Do the GUI IO */
      int doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys);

      /*! Draw The GUI */
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Redefine Camera Position */
      void cameraPos();

      /*! Gets the FileName */
      string getFileName();

      /*! Gets the Texture File Name */
      string getTextureFileName();

      /*! Gets Object File Name */
      string getObjectFileName();

      /*! Show Message on Message Window */
      void showMessage(string msg);

      camera gameCamera;               /**< The Game Camera */

#if 0 
      GLfloat cameraX; /**< Camera X position */
      GLfloat cameraY; /**< Camera Y position */
      GLfloat cameraZ; /**< Camera Z position */

      GLfloat theta;   /**< Camera Theta */
      GLfloat phi;     /**< Camera Phi */
      GLfloat d;       /**< Camera Zoom */
      GLfloat deltaY;  /**< Camera Delta Y */

      GLfloat centerX; /**< Camera Center X Position */
      GLfloat centerY; /**< Camera Center Y Position */
      GLfloat centerZ; /**< Camera Center Z Position */
#endif

   private:

      int state;      /**< Internal State */
      int tool;       /**< Selected Tool */

      interface* gui; /**< Internal GUI */

      /*********************File*Window*******************************/
      janela* fileWindow;   /**< File Window */
      botao* openButton;    /**< Open Button */
      botao* saveButton;    /**< Save Button */
      botao* exitButton;    /**< Exit Button */
      botao* newButton;     /**< New Button */
      barraTexto* fileText; /**< FileName Text */
      void openFileWindow();


      /*********************Navigate*Window***************************/
      janela* navWindow;              /**< Navigate Window */
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
      janela* mainWindow;   /**< Main Window */
      botao* terrainButton; /**< Terrain Edition Button */
      botao* wallButton;    /**< Wall Edition Button */
      botao* portalButton;  /**< Portal Edition Button */
      botao* objectButton;  /**< Objects Edition Button */
      botao* npcButton;     /**< NPC Editon Button */
      botao* particleButton;/**< Particle Edit Button */
      botao* lightButton;   /**< Light Edit Button */
      botao* fogButton;     /**< Fog Button */
      botao* destroyButton; /**< Destroy Button */
      botao* optionsButton; /**< Options Button */
      botao* musicButton;   /**< Music Button */
      botao* sndFxButton;   /**< Sound Effects Button */
      void openMainWindow();

      /*********************Texture*Window*******************************/
      janela* textureWindow;
      botao* texturePreviousButton;
      botao* textureNextButton;
      botao* textureInsertButton;
      barraTexto* textureText;
      void openTextureWindow();

      /*********************Terrain*Window*******************************/
      janela* terrainWindow;               /**< Terrain Window */
      tabButton* terrainTabButton;         /**< Terrain TabButton */
      oneTabButton* terrainUpButton;       /**< Terrain Up Button */
      oneTabButton* terrainDownButton;     /**< Terrain Down Button */
      oneTabButton* terrainNivButton;      /**< Terrain Nivelation Button */
      oneTabButton* terrainWalkableButton; /**< Terrain Walkable Button */
      oneTabButton* terrainTextureButton;  /**< Terrain Texture Button */
      void openTerrainWindow();
      /*********************Wall*Window*******************************/
      janela* wallWindow;                 /**< Wall Window */
      tabButton* wallTabButton;           /**< Wall Tab Button */
      oneTabButton* wallXButton;          /**< X Wall Button */
      oneTabButton* wallZButton;          /**< Z Wall Button */
      oneTabButton* wallX2Button;         /**< X Meio Fio Button */
      oneTabButton* wallZ2Button;         /**< Z Meio Fio Button */
      oneTabButton* wallTextureButton;    /**< Wall Texture Button */
      oneTabButton* wallLessVerTexture;   /**< Texture Vertical Less Button */
      oneTabButton* wallMoreVerTexture;   /**< Texture Vertical More Button */
      oneTabButton* wallLessHorTexture;   /**< Texture Horizontal Less Button */
      oneTabButton* wallMoreHorTexture;   /**< Texture Horizontal More Button */
      void openWallWindow();
      /*********************Portal*Window*******************************/
      janela* portalWindow;               /**< Portal Window */
      tabButton* portalTabButton;         /**< Portal TabButton */
      oneTabButton* portalAddButton;      /**< Add Portal Button */
      oneTabButton* portalTagButton;      /**< Portal Tag Button */
      oneTabButton* portalDoorButton;     /**< Door Button */
      void openPortalWindow();
      /*********************Object*Window*******************************/
      janela* objectWindow;               /**< Object Window */
      barraTexto* objectText;             /**< Object Text Bar */
      botao* objectInsertButton;          /**< Object Insert Button */
      tabButton* objectTabButton;         /**< Object TabButton */
      oneTabButton* objectAddButton;      /**< Object Add Button */
      void openObjectWindow();      
      /*********************Particle*Window*******************************/
      janela* particleWindow;             /**< Particle Window */
      tabButton* particleTabButton;       /**< Particles Tab Button */
      oneTabButton* fireButton;           /**< Particle Fire Button */
      oneTabButton* smokeButton;          /**< Particle Smoke Button */
      void openParticleWindow();
      /*********************Message*Window*******************************/
      janela* messageWindow;              /**< Message Window */
      quadroTexto* messageText;           /**< Message Text */
      void openMessageWindow();

      

      
};

#endif

