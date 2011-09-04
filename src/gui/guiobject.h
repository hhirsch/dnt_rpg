/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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

#ifndef _farso_guiobject_h
#define _farso_guiobject_h

#include <SDL/SDL.h>
#include <iostream>
#include <string>
using namespace std;

#include "../etc/list.h"

/*! The farso guiObject types */
enum
{
   FARSO_OBJECT_WINDOW,
   FARSO_OBJECT_BUTTON,
   FARSO_OBJECT_TEXT_BAR,
   FARSO_OBJECT_SEL_BOX,
   FARSO_OBJECT_PICTURE,
   FARSO_OBJECT_SEL_TEXT,
   FARSO_OBJECT_TEXT_BOX,
   FARSO_OBJECT_MENU_ITEM,
   FARSO_OBJECT_TAB_BUTTON,
   FARSO_OBJECT_ROL_BAR,
   FARSO_OBJECT_LIST_TEXT,
   FARSO_OBJECT_FILE_SEL,
   FARSO_OBJECT_HEALTH_BAR,
   FARSO_OBJECT_TAB_BOX
};

/*! The Class guiObject is a generic interface to all GUI related objects. */
class guiObject : public dntListElement
{
   public:
      int type;            /**< Object Type */ 

      /*! Constructor 
       * \param surface -> SDL_Surface to draw to */
      guiObject(SDL_Surface* surface)
      {
         available=true;
         text="";
         mouseHint = "";
         wSurface=surface;
      };

      /*! Virtual Destructor */
      virtual ~guiObject(){};

      /*! Virtual Draw Function */
      virtual void draw()=0;

      /*! Verify if the mouse is in the object or not
       * \param mouseX -> mouse X Coordinate
       * \param mouseY -> mouse Y Coordinate */
      bool isMouseIn(int mouseX, int mouseY)
      {
         return( ( (mouseX>=x1) && (mouseX<=x2) ) && 
                 ( (mouseY>=y1) && (mouseY<=y2) ) );
      };

      /*! Gets the object text
       * \return object text */
      string getText(){return(text);};

      /*! Set the object text
       * \param txt -> new text */
      void setText(string txt){text = txt;};

      /*! Gets the X1 Coordinate
       * \return x1 coordinate */
      int getX1(){return(x1);};

      /*! Gets the X2 Coordinate
       * \return x2 coordinate */
      int getX2(){return(x2);};

      /*! Gets the y1 Coordinate
       * \return y1 coordinate */
      int getY1(){return(y1);};

      /*! Gets the Y2 Coordinate
       * \return y2 coordinate */
      int getY2(){return(y2);};

      /*! Set Object Coordinate
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate */
      void setCoordinate(int xa, int ya, int xb, int yb)
      {
         x1 = xa; x2 = xb; y1 = ya; y2 = yb;
      };

      /*! Get Object Coordinate
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate */
      void getCoordinate(int& xa, int& ya, int& xb, int& yb)
      {
         xa = x1; xb = x2; ya = y1; yb = y2;
      };

      /*! Verify if the object is available
       * return true if is avaible */
      bool isAvailable() {return(available);};

      /*! Set if the item is available or not
       * \param av -> true if is available, false otherwise */
      void setAvailable(bool av){available = av;};

      /*! Verify if the draw state changes */
      bool changed(){bool prev = hadChanged; hadChanged = false;return(prev);};

      /*! Set the window as changed */
      void setChanged(){hadChanged = true;};

      /*! Set the surface used */
      void setSurface(SDL_Surface* surface){wSurface = surface;};

      /*! Set Text to display when mouse is over the object */
      void setMouseHint(string txt){mouseHint=txt;};
      /*! Get current mouse hint */
      string getMouseHint(){return(mouseHint);};

   protected:
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      bool available;      /**< Available? */
      string text;         /**< Text Display */
      string mouseHint;    /**< Hint when mouse over */
      SDL_Surface* wSurface; /**< Screen surface */
   
   private:
      bool hadChanged;     /**< Flag if the had changed its draw state */
};



#endif

