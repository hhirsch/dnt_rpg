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

#ifndef _farso_tab_box_h
#define _farso_tab_box_h

#include <list>
#include <string>

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "guiobject.h"
#include "guilist.h"

#ifdef FARSO_USE_DNT_SOUND
   #include "../sound/sound.h"
#endif

namespace Farso
{

/*! tabObj is used to keep information about a single option at the tabBox */
class TabObj
{
   public:
      /*! Constructor */
      TabObj(int w, int h, SDL_Surface* surface);
      /*! Destructor */
      ~TabObj();

      GuiList* list;      /**< The guiList itself */
      std::string title;  /**< The title */
};

/*! The TabBox class is a tabWidget for FarSo. It have a 
 * gui objects list for each tab option, only displaying one
 * per time (that that is selected). 
 * \note each guiList can have only one tabBox */
class TabBox: public GuiObject
{
   public:
      /*! Constructor 
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param screen -> pointer to the window surface */
      TabBox(int xa, int ya, int xb, int yb, SDL_Surface* screen);

      /*! Destructor */
      ~TabBox();

      /*! Get the active Element List
       * \return -> guiList relative to the active option or NULL */
      GuiList* getActiveList();

      /*! Get the active element title
       * \return -> title of the active element (or "") */
      std::string getActiveTitle();

      /*! Get list relative to the option
       * \param opt -> option number to get guiList from [0,total)
       * \return -> guiList relative to the option or NULL */
      GuiList* getList(int opt);

      /*! Get list relative to the option
       * \param title -> option title to get guiList from
       * \return -> guiList relative to the option or NULL */
      GuiList* getList(std::string title);

      /*! Insert a new option at the tabBox
       * \param title -> unique title for the option 
       * \return -> pointer to th object guiList */
      GuiList* insertOption(std::string title);

      /*! Draw the tabBox */
      void draw();

      /*! Verify changes to the active option
       * \param mouseX -> current mouseX position relative to window 
       * \param mouseY -> current mouseY position relative to window
       * \return -> true if got a change here, false otherwise */
      bool verifyChanges(int mouseX, int mouseY);

   protected:

      /*! Get the tabObj relative to the number
       * \param opt -> tabObj index
       * \return -> pointer to the found tabObj or NULL */
      TabObj* getObject(int opt);

      std::list<TabObj*> tabs;

      TabObj* active;  /**< The active tabObj */
#ifdef FARSO_USE_DNT_SOUND
      sound snd;   /**< Sound controller */
#endif
};

}

#endif

