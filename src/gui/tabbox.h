/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "guiobject.h"
#include "guilist.h"

#include <string>
using namespace std;

/*! tabObj is used to keep information about a single option at the tabBox */
class tabObj
{
   public:
      /*! Constructor */
      tabObj();
      /*! Destructor */
      ~tabObj();

      guiList* list;      /**< The guiList itself */
      string title;       /**< The title */

      tabObj* next;     /**< Pointer to the next */
      tabObj* previous; /**< Pointer to the previous */
};

/*! The tabBox class is a tabWidget for FarSo. It have a 
 * gui objects list for each tab option, only displaying one
 * per time (that that is selected). 
 * \note each guiList can have only one tabBox */
class tabBox: public guiObject
{
   public:
      /*! Constructor 
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param screen -> pointer to the window surface */
      tabBox(int xa, int ya, int xb, int yb, SDL_Surface* screen);

      /*! Destructor */
      ~tabBox();

      /*! Get the active Element List
       * \return -> guiList relative to the active option or NULL */
      guiList* getActiveList();

      /*! Get the active element title
       * \return -> title of the active element (or "") */
      string getActiveTitle();

      /*! Get list relative to the option
       * \param opt -> option number to get guiList from [0,total)
       * \return -> guiList relative to the option or NULL */
      guiList* getList(int opt);

      /*! Get list relative to the option
       * \param title -> option title to get guiList from
       * \return -> guiList relative to the option or NULL */
      guiList* getList(string title);

      /*! Get the total number of options at the tabBox
       * \return -> total number of options */
      int getTotal();

      /*! Insert a new option at the tabBox
       * \param title -> unique title for the option 
       * \return -> pointer to th object guiList */
      guiList* insertOption(string title);

      /*! Draw the tabBox
       * \param screen -> pointer to the surface to draw to */
      void draw(SDL_Surface* screen);

      /*! Verify changes to the active option
       * \param mouseX -> current mouseX position relative to window 
       * \param mouseY -> current mouseY position relative to window
       * \return -> true if got a change here, false otherwise */
      bool verifyChanges(int mouseX, int mouseY);

   protected:

      /*! Get the tabObj relative to the number
       * \param opt -> tabObj index
       * \return -> pointer to the found tabObj or NULL */
      tabObj* getObject(int opt);

      int total;       /**< Total Objects at list */
      tabObj* first;   /**< tabObj List */
      tabObj* active;  /**< The active tabObj */
      SDL_Surface* wSurface; /**< Pointer to the window surface */

};

#endif

