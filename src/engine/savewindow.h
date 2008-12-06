#ifndef _dnt_save_window_h
#define _dnt_save_window_h

#define DNT_SAVE_WINDOW_CANCEL   0
#define DNT_SAVE_WINDOW_CONFIRM  1

#include "../gui/farso.h"

#include "savefile.h"

/*! The save window class is used to display a load/save games window */
class saveWindow
{
   public:
      /*! Constructor
       * \param interf -> guiInterface used */
      saveWindow();
      /*! Destructor */
      ~saveWindow();

      /*! Run the Save/Load Window
       * \param load -> true if the window is a loading one
       * \param proj -> projection matrix
       * \param modl -> model view matrix
       * \param viewPort -> viewPort vector
       * \return -> DNT_SAVE_WINDOW_CANCEL or DNT_SAVE_WINDOW_CONFIRM */
      int run(bool load, GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Get the selected fileName
       * \return selected fileName (if one) */
      string getSelectedFileName();

   protected:

      /*! Open then window */
      void open();

      /*! Change the current displayed image and title */
      void changeInfo();

      string curFileName;       /**< Current FileName selected */

      window* fileWindow;       /**< The file window, used for load/save */
      fileSel* fileSelector;    /**< The file selector interface */
      textBox* fileTitle;       /**< The title of the save */
      picture* fileImage;       /**< The image representing the save */
      bool windowIsLoad;        /**< If is a load window or not */
      guiInterface* gui;        /**< Pointer to the current gui used */ 
};


#endif


