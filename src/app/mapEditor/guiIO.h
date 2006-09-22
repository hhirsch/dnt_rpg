#ifndef _guiIO_H
#define _guiIO_H

#include "../../gui/farso.h"

/* Return Constants */
#define GUI_IO_NOTHING       0
#define GUI_IO_NEW_POSITION  1
#define GUI_IO_NEW_STATE     2
#define GUI_IO_NEW_MAP       3
#define GUI_IO_OPEN_MAP      4
#define GUI_IO_SAVE_MAP      5
#define GUI_IO_EXIT          6

/* State Constants */
#define GUI_IO_STATE_INITIAL 0

/**! The Super GUI IO Class, to control things on map Editor! */
class guiIO
{
   public:

      guiIO();
      ~guiIO();

      /*! Gets State seted by GUI IO.
       * \return Internal State */
      int getState();

      /*! Do the GUI IO */
      int doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys);

      /*! Draw The GUI */
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Redefine Camera Position */
      void camera();

      /*! Gets the FileName */
      string getFileName();

      GLfloat cameraX; 
      GLfloat cameraY;
      GLfloat cameraZ;

      GLfloat theta;
      GLfloat phi;
      GLfloat d;
      GLfloat deltaY;

      GLfloat centerX;
      GLfloat centerY;
      GLfloat centerZ; 

   private:

      int state;      /**< Internal State */

      interface* gui; /**< Internal GUI */

      /*********************File*Window*******************************/
      janela* fileWindow;   /**< File Window */
      botao* openButton;    /**< Open Button */
      botao* saveButton;    /**< Save Button */
      botao* exitButton;    /**< Exit Button */
      botao* newButton;     /**< New Button */
      barraTexto* fileText; /**< FileName Text */

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

      /*********************Main*Window*******************************/
      janela* mainWindow;   /**< Main Window */
      botao* floorButton;   /**< Floor Edition Button */
      botao* wallButton;    /**< Wall Edition Button */
      botao* portalButton;  /**< Portal Edition Button */
      botao* objectButton;  /**< Objects Edition Button */
      botao* npcButton;     /**< NPC Editon Button */
      botao* particleButton;/**< Particle Edit Button */
      botao* lightButton;   /**< Light Edit Button */
      botao* fogButton;     /**< Fog Button */
      botao* destroyButton; /**< Destroy Button */
      botao* optionsButton; /**< Options Button */

      /*********************Texture*Window*******************************/
      janela* textureWindow;

      /*********************Floor*Window*******************************/
      janela* floorWindow;  /**< Floor Window */
      /*********************Wall*Window*******************************/
      janela* wallWindow;   /**< Wall Window */
      /*********************Portal*Window*******************************/
      janela* portalWindow; /**< Portal Window */
      /*********************Object*Window*******************************/
      janela* objectWindow; /**< Object Window */
      

      
};

#endif

