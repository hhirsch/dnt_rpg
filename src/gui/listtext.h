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

#ifndef _farso_listtext_h
#define _farso_listtext_h

#include <list>

#include "rolbar.h"
#include "tabbutton.h"

namespace Farso
{

class GuiList;

/*! The listText class. Is a list of texts elements to select one */
class ListText: public GuiObject
{
   public:
      /*! Constructor 
       * \param xa -> x1 position
       * \param ya -> y1 position
       * \param xb -> x2 position
       * \param yb -> y2 position
       * \param surface -> window surface to draw
       * \param list -> pointer to the GUI objects list */
      ListText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
               GuiList* list);
      /*! Destructor */
      ~ListText();

      /*! Clear the text list */
      void clear();

      /*! Insert Text Element on the list
       * \param text -> string to insert */
      void insertText(std::string text);

      /*! Insert Text element, with color on the list
       * \param text -> string to insert 
       * \param r -> red 
       * \param g -> green
       * \param b -> blue */
      void insertText(std::string text, int r, int g, int b);

      /*! Remove text from list
       * \param text -> string to remove */
      void removeText(std::string text);

      /*! Draw the listText to the surface */
      void draw();

      /*! Verify if some event is related to the listText
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the listText */
      bool eventGot(int type, GuiObject* object);

      /*! Get the selected text
       * \return selected text */
      std::string getSelectedText();

      /*! Get the selected text position on list
       * \return selected text position */
      int getSelectedPos();

      RolBar* roll;           /**< RolBar of the list */
      TabButton* table;       /**< TabButton of the list */

   protected:
      /*! Define tabButton buttons avaialabilility */
      void defineTabButton();

   private:
      GuiList* intList;           /**< Internal GUI Objects List */
      OneTabButton** listButtons; /**< Buttons of the List*/
      std::string selectedText;   /**< Text selected */
      int selectedPos;            /**< Position on list of the selected */
      int maxButtons;             /**< Max number of buttons */
      std::list<std::string>textList; /**< List of text elements */
};

}

#endif

