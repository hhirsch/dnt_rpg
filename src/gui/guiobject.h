/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_guiobject_h
#define _farso_guiobject_h

#include <SDL/SDL.h>
#include <iostream>
#include <string>

namespace Farso
{

/*! The farso guiObject types */
enum FarsoObjectEnum
{
   OBJECT_WINDOW,
   OBJECT_BUTTON,
   OBJECT_TEXT_BAR,
   OBJECT_SEL_BOX,
   OBJECT_PICTURE,
   OBJECT_SEL_TEXT,
   OBJECT_TEXT_BOX,
   OBJECT_MENU_ITEM,
   OBJECT_TAB_BUTTON,
   OBJECT_ROL_BAR,
   OBJECT_LIST_TEXT,
   OBJECT_FILE_SEL,
   OBJECT_HEALTH_BAR,
   OBJECT_TAB_BOX
};

/*! The Class guiObject is a generic interface to all GUI related objects. */
class GuiObject
{
   public:
      int type;            /**< Object Type */ 

      /*! Constructor 
       * \param surface -> SDL_Surface to draw to */
      GuiObject(SDL_Surface* surface)
      {
         available=true;
         visible = true;
         text="";
         mouseHint = "";
         wSurface=surface;
      };

      /*! Virtual Destructor */
      virtual ~GuiObject(){};

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
      std::string getText(){return(text);};

      /*! Set the object text
       * \param txt -> new text */
      void setText(std::string txt){text = txt;};

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
      void setMouseHint(std::string txt){mouseHint=txt;};
      /*! Get current mouse hint */
      std::string getMouseHint(){return(mouseHint);};
   
      /*! Hide the object */
      void hide(){visible=false;available=false;hadChanged=true;};
      /*! Show the object */
      void show(){visible=true;available=true;hadChanged=true;};
      /*! Get visibility status */
      bool isVisible(){return(visible);};

   protected:
      int x1,              /**< Coordinate on Window */
          y1,              /**< Coordinate on Window */
          x2,              /**< Coordinate on Window */
          y2;              /**< Coordinate on Window */
      bool available;      /**< Available? */
      std::string text;      /**< Text Display */
      std::string mouseHint; /**< Hint when mouse over */
      SDL_Surface* wSurface; /**< Screen surface */
      bool visible;        /**< Toggle object visibility */
   
   private:
      bool hadChanged;     /**< Flag if the had changed its draw state */
};

}

#endif

