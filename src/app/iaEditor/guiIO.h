#ifndef _guiIO_H
#define _guiIO_H

#include "../../gui/farso.h"

/* Return Constants */
#define GUI_IO_NOTHING              0
#define GUI_IO_NEW_POSITION         1
#define GUI_IO_NEW_STATE            2
#define GUI_IO_NEW_IAFILE           3
#define GUI_IO_OPEN_IAFILE          4
#define GUI_IO_SAVE_IAFILE          5
#define GUI_IO_EXIT                 6
#define GUI_IO_OTHER                7

#define TOOL_NONE            0

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

      /*! Get Tool selected by GUI IO.
       * \return actual selected Tool */
      int getTool();


      /*! Do the GUI IO */
      int doIO(int mouseX, int mouseY, Uint8 mButton, Uint8 *keys);

      /*! Draw The GUI */
      void draw(GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Redefine Camera Position */
      void camera();

      /*! Gets the FileName */
      string getFileName();

      /*! Show Message on Message Window */
      void showMessage(string msg);

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

      /*********************Message*Window*******************************/
      janela* messageWindow;              /**< Message Window */
      quadroTexto* messageText;           /**< Message Text */
      void openMessageWindow();

      /*********************Objects*Window*******************************/
      janela* objectsWindow;              /**< Message Window */
      tabButton* objectsTabButton;        /**< Objects Tab Button */
      oneTabButton* potentialButton;      /**< Potential Agent Button */
      oneTabButton* patternButton;        /**< Pattern Agent Button */
      oneTabButton* goalButton;           /**< Goal Button */
      oneTabButton* objectButton;         /**< Objects Button */
      void openObjectsWindow();

      /*********************PlayControl*Window***************************/
      janela* playControlWindow;              /**< Message Window */
      tabButton* playControlTabButton;        /**< Objects Tab Button */
      oneTabButton* playButton;               /**< Play Button */
      oneTabButton* stopButton;               /**< Stop Button */
      oneTabButton* pauseButton;              /**< Pause Button */
      void openPlayControlWindow();

};

#endif

