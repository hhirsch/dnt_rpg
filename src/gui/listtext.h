/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
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

#ifndef _farso_listtext_h
#define _farso_listtext_h

#include "../etc/list.h"

#include "rolbar.h"
#include "tabbutton.h"

/*! A element of text on listText */
class textElement: public dntListElement
{
   public:
      string text;            /**< The text of the element */
};

/*! The listText class. Is a list of texts elements to select one */
class listText: public guiObject, public dntList
{
   public:
      /*! Constructor 
       * \param xa -> x1 position
       * \param ya -> y1 position
       * \param xb -> x2 position
       * \param yb -> y2 position
       * \param surface -> window surface to draw
       * \param list -> pointer to the GUI objects list */
      listText(int xa, int ya, int xb, int yb, SDL_Surface* surface,
               void* list);
      /*! Destructor */
      ~listText();

      /*! Clear the text list */
      void clear();

      /*! Insert Text Element on the list
       * \param text -> string to insert */
      void insertText(string text);

      /*! Insert Text element, with color on the list
       * \param text -> string to insert 
       * \param r -> red 
       * \param g -> green
       * \param b -> blue */
      void insertText(string text, int r, int g, int b);

      /*! Remove text from list
       * \param text -> string to remove */
      void removeText(string text);

      /*! Draw the listText to the surface */
      void draw();

      /*! Verify if some event is related to the listText
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \return -> true if event is related (and treated) by the listText */
      bool eventGot(int type, guiObject* object);

      /*! Get the selected text
       * \return selected text */
      string getSelectedText();

      /*! Get the selected text position on list
       * \return selected text position */
      int getSelectedPos();

      rolBar* roll;           /**< RolBar of the list */
      tabButton* table;       /**< TabButton of the list */

   protected:
      /*! Define tabButton buttons avaialabilility */
      void defineTabButton();

      /*! Free the textElement
       * \param obj -> pointer to the text element */
      void freeElement(dntListElement* dnt);

   private:
      void* intList;              /**< Internal GUI Objects List */
      oneTabButton** listButtons; /**< Buttons of the List*/
      string selectedText;        /**< Text selected */
      int selectedPos;            /**< Position on list of the selected */
      int maxButtons;             /**< Max number of buttons */
};

#endif

