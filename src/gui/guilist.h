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

#ifndef _farso_guilist_h
#define _farso_guilist_h

#include <string>
using namespace std;

#include "../etc/list.h"

#include "guiobject.h"
#include "button.h"
#include "cxsel.h"
#include "picture.h"
#include "textbox.h"
#include "seltext.h"
#include "tabbutton.h"
#include "textbar.h"
#include "rolbar.h"
#include "listtext.h"
#include "filesel.h"
#include "healthBar.h"

/*! The guiList Class: a list for guiObjects. */
class guiList: public dntList
{
   public:
      /*! Constructor
       * \param surface -> pointer to the surface where list is
       * \param t -> dntList type */
      guiList(SDL_Surface* surface, int t=DNT_LIST_TYPE_ADD_AT_BEGIN);
      /*! Destructor */
      ~guiList();

      /*! Verify if some object at the list changed its draw status
       * \return -> true if something changed, false if not */
      bool changed();

      /*! Draw All objects at the list */
      void draw();

      /*! Insert a Button on List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate 
       * \param text -> button text
       * \param oval -> if the button is oval or rectangular
       * \return pointer to the button inserted */
      button* insertButton(int xa,int ya,int xb,int yb,
                           string text, bool oval);

      /*! Insert a new cxSel on the List
       * \param xa -> X coordinate
       * \param ya -> Y coordinate
       * \param selecionada -> if selected or not */
      cxSel* insertCxSel(int xa, int ya, bool selecionada);

      /*! Insert a new picture on the list
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param arquivo -> fileName to opwn or NULL to not open a file
       * \return pointer to the picture inserted */
      picture* insertPicture(int x,int y,int w,int h, const char* arquivo);

      /*! Insert a new tabButton on the list
       * \param x -> x initial coordinate
       * \param y -> y initial coordinate
       * \param w -> width of the picture
       * \param h -> height of the picture
       * \param arquivo -> fileName to opwn or NULL to not open a file 
       * \return tabbutton inserted */
      tabButton* insertTabButton(int x,int y,int w,int h, const char* arquivo);

      /*! Insert a textBar on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate 
       * \param yb -> y2 coordinate
       * \param text -> textBar text
       * \param cript -> if is crypt, or not
       * \return textBar inserted */
      textBar* insertTextBar(int xa,int ya,int xb,int yb, string text, 
                             int cript);

      /*! Insert a textBox on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param frameType -> type of the frame. 0 for no frames
       * \param text -> text of the textBox
       * \return textBox inserted */
      textBox* insertTextBox(int xa,int ya,int xb,int yb,
                             int frameType, string text);

      /*! Insert a selText on the list
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param text0 -> option 0 text
       * \param text1 -> option 1 text
       * \param text2 -> option 2 text
       * \param text3 -> option 3 text
       * \param text4 -> option 4 text
       * \return selText inserted */
      selText* insertSelText(int xa,int ya,int xb,int yb,string text0,
                             string text1,string text2,string text3, 
                             string text4);

      /*! Insert a rolBar on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param txt -> text on the rolbar
       * \return rolBar inserted */
      rolBar* insertRolBar(int xa,int ya,int xb,int yb,string txt);

      /*! Insert a listText on the List
       * \param xa -> x1 position
       * \param ya -> y1 position
       * \param xb -> x2 position
       * \param yb -> y2 position
       * \return listText inserted */
      listText* insertListText(int xa,int ya,int xb,int yb);

      /*! Insert a fileSel on the list
       * \param xa -> x coordinate
       * \param ya -> y coordinate
       * \param load -> true if the selector is a loading one, false if it
       *                is a saving one.
       * \param dir -> current dir to browse
       * \param nav -> if show dirs to navigate or not
       * \return -> pointer to the fileSel inserted */
      fileSel* insertFileSel(int xa, int ya, bool load, string dir, 
                             bool nav=true);

      /*! Insert a healthBar (a progress bar) on the list
       * \param xa -> x1 coordinate 
       * \param ya -> y1 coordinate 
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param max -> max health value */
      healthBar* insertHealthBar(int xa, int ya, int xb, int yb, int max);

      /*! Define a tabBox for the window
       * \param x1 -> x first coordinate
       * \param y1 -> y first coordinate
       * \param x2 -> x last coordinate
       * \param y2 -> y last coordinate
       * \return -> pointer to the defined tabBox */
      guiObject* defineTabBox(int x1, int y1, int x2, int y2);

      /*! Get the current tabBox 
       * \return -> pointer to the current tabBox */
      guiObject* getTabBox();
  
      /*! Get the objects list relative to the tabBox (if one)
       * \return guiList of objects active at the tabBox */
      guiList* getActiveTabBoxList();

      /*! Add a internal Menu to the list
       * \return menu Inserted */
      guiObject* addMenu();

      /*! Get the Internal Menu of The List
       * \return internal menu */
      guiObject* getMenu();

      /*! Remove Internal Menu, if exists one. */
      void removeMenu();

   protected:

      /*! Free Element
       * \param obj -> pointer to the object to free */
      void freeElement(dntListElement* obj);



      SDL_Surface* wSurface;  /**< window surface used */
      
      guiObject* intMenu; /**< The internal Menu of the List */
      guiObject* tab;    /**< Inner tabBox */

};

#endif

