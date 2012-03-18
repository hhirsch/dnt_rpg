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
      aspectWindow(character* dude, Farso::GuiInterface* inter);
      /*! Destructor */
      ~aspectWindow();

      /*! Treat Events on Window. 
       * \param inter -> pointer to GUI interface
       * \param object -> last GUI object
       * \param eventInfo -> last GUI Event
       * \return 0 on close, 1 otherwise */
      int treat(Farso::GuiObject* object, int eventInfo, 
            Farso::GuiInterface* inter);

   private:
      /*! Load All Portrait Images */
      void loadImages();

      character* usedCharacter;    /**< Pointer to the used character */

      Farso::Button* buttonConfirm;         /**< Confirm Button */
      Farso::Button* buttonCancel;          /**< Cancel Button */
      Farso::Button* buttonNextImage;       /**< Next Image Button */
      Farso::Button* buttonPreviousImage;   /**< Previous Image Button */
      Farso::TextBar* textName;            /**< Character Name Text */
      Farso::TextBar* textAge;             /**< Character Age Text */
      Farso::CxSel* cxSelSexM;             /**< Feminine Sex SelBox */
      Farso::CxSel* cxSelSexF;             /**< Masculine Sex SelBox */
      Farso::CxSel* cxSelSexO;             /**< Other Sex SelBox */
      Farso::Picture* figurePortrait;      /**< Portrait Figure */
      Farso::Window* intWindow;           /**< Internal pointer to the window */

      int totalImages;              /**< Number of total images */
      int curImage;                 /**< Nuber of the Current Image */
      portraitImage* images;        /**< All Portrait Images */
};

#endif

