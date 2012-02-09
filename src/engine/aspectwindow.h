/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#ifndef _dnt_aspectwindow_h
#define _dnt_aspectwindow_h

#include "../gui/farso.h"
#include "../etc/dirs.h"
#include "character.h"

#define ASPECTW_CANCEL  0 /**< Cancel return from Aspect Window */
#define ASPECTW_CONFIRM 1 /**< Confirm return from Aspect window */
#define ASPECTW_OTHER   2 /**< Other return from Aspect Window */

/*! Image to portrait */
class portraitImage
{
   public:
      portraitImage(){image = NULL;};
      ~portraitImage(){if(image){SDL_FreeSurface(image);}};
      SDL_Surface* image;         /**< Image to portrait */
      std::string imageFile;      /**< FileName of the Image */
};

/*! The Aspect Window Class */
class aspectWindow
{
   public:
      /*! Constructor
       * \param dude -> pointer to the edited character
       * \param inter -> pointer to the GUI used interface */
      aspectWindow(character* dude, guiInterface* inter);
      /*! Destructor */
      ~aspectWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(guiObject* object, int eventInfo, guiInterface* inter);

   private:
      /*! Load All Portrait Images */
      void loadImages();

      character* usedCharacter;    /**< Pointer to the used character */

      button* buttonConfirm;         /**< Confirm Button */
      button* buttonCancel;          /**< Cancel Button */
      button* buttonNextImage;       /**< Next Image Button */
      button* buttonPreviousImage;   /**< Previous Image Button */
      textBar* textName;            /**< Character Name Text */
      textBar* textAge;             /**< Character Age Text */
      cxSel* cxSelSexM;             /**< Feminine Sex SelBox */
      cxSel* cxSelSexF;             /**< Masculine Sex SelBox */
      cxSel* cxSelSexO;             /**< Other Sex SelBox */
      picture* figurePortrait;      /**< Portrait Figure */
      window* intWindow;            /**< Internal pointer to the window */

      int totalImages;              /**< Number of total images */
      int curImage;                 /**< Nuber of the Current Image */
      portraitImage* images;        /**< All Portrait Images */
};

#endif

