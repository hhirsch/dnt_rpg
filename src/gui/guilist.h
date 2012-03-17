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

#ifndef _farso_guilist_h
#define _farso_guilist_h

#include <string>

#include <list>

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

enum
{
   LIST_TYPE_ADD_AT_BEGIN=0,
   LIST_TYPE_ADD_AT_END
};

/*! The guiList Class: a list for guiObjects. */
class guiList
{
   public:
      /*! Constructor
       * \param sWidth -> usefull width of the surface
       * \param sHeight -> usefull height of the surface (see menu)
       * \param surface -> pointer to the surface where list is
       * \param hasDecor -> if window has decor or not
       * \param t -> dntList type */
      guiList(int sWidth, int sHeight, SDL_Surface* surface, bool hasDecor,
            int t=LIST_TYPE_ADD_AT_BEGIN);
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
                           std::string text, bool oval);

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
      textBar* insertTextBar(int xa,int ya,int xb,int yb, std::string text, 
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
                             int frameType, std::string text);

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
      selText* insertSelText(int xa,int ya,int xb,int yb, std::string text0,
                             std::string text1, std::string text2,
                             std::string text3, std::string text4);

      /*! Insert a rolBar on the List
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param txt -> text on the rolbar
       * \return rolBar inserted */
      rolBar* insertRolBar(int xa,int ya,int xb,int yb, std::string txt);

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
      fileSel* insertFileSel(int xa, int ya, bool load, std::string dir, 
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

      /*! Clear and delete all elements from the list */
      void clearList();

      /*! Insert element on list, based on its type (end or head) */
      void insert(guiObject* obj);

      friend class guiInterface;

   protected:

      /*! Free Element
       * \param obj -> pointer to the object to free */
      void freeElement(guiObject* obj);
     
      std::list<guiObject*>list; /**< The list itself */

      SDL_Surface* wSurface;  /**< window surface used */
      int wWidth,             /**< Window width */
          wHeight;            /**< Window Height */
      int type;               /**< List type */
      bool wHasDecor;         /**< true if window has decor */
      
      guiObject* intMenu; /**< The internal Menu of the List */
      guiObject* tab;    /**< Inner tabBox */

};


#endif

