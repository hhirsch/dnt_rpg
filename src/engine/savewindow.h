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
      /*! Constructor */
      saveWindow();
      /*! Destructor */
      ~saveWindow();

      /*! Run the Save/Load Window
       * \param load -> true if the window is a loading one
       * \param tituloId -> the main screen texture
       * \param proj -> projection matrix
       * \param modl -> model view matrix
       * \param viewPort -> viewPort vector
       * \return -> DNT_SAVE_WINDOW_CANCEL or DNT_SAVE_WINDOW_CONFIRM */
      int run(bool load, GLuint tituloId, 
            GLdouble proj[16],GLdouble modl[16],GLint viewPort[4]);

      /*! Get the selected fileName
       * \return selected fileName (if one) */
      std::string getSelectedFileName();

   protected:

      /*! Open then window */
      void open();

      /*! Change the current displayed image and title */
      void changeInfo(int mouseX, int mouseY);

      std::string curFileName;       /**< Current FileName selected */

      Farso::Window* fileWindow;  /**< The file window, used for load/save */
      Farso::FileSel* fileSelector;    /**< The file selector interface */
      Farso::TextBox* fileTitle;       /**< The title of the save */
      Farso::Picture* fileImage;       /**< The image representing the save */
      bool windowIsLoad;        /**< If is a load window or not */
      Farso::GuiInterface* gui;        /**< Pointer to the current gui used */ 
};


#endif


