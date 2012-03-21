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

#ifndef _farso_rolbar_h
#define _farso_rolbar_h

#include <string>

#include "guiobject.h"
#include "color.h"
#include "draw.h"
#include "mouse.h"
#include "button.h"
#include "textbox.h"

namespace Farso
{

class GuiList;

/*! Rolling Text Bar Definition */
class RolBar: public GuiObject
{
   public:
      /*! Constructor
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param txt -> text on the rolbar
       * \param list -> pointer to the guiList
       * \param surface -> pointer to the screen surface */
      RolBar(int xa, int ya, int xb, int yb, std::string txt, GuiList* list, 
             SDL_Surface* surface);
      /*! Destructor */
      ~RolBar();

      /*! Verify if the rolBar is the owner of a guiObject
       * \param obj -> pointer to the desired guiObject
       * \return -> true if obj is from the rolBar (a button, for example) */
      bool isOwner(GuiObject* obj);

      /*! Verify if some event is related to the rolBar
       * \param type -> Type of the occurred event
       * \param object -> object of the event 
       * \param mouseY -> mouse Y coordinate
       * \return -> true if event is related (and threated) by the rolBar */
      bool eventGot(int type, GuiObject* object, int y);

      /*! Set The Text of the Bar, putting them on top
       * \param txt -> desired text */
      void setText(std::string txt);

      /*! Add text to bar 
       * \param txt -> text to add
       * \param info-> some related info for the text added */
      void addText(std::string txt, void* info=NULL);

      /*! Add the string to the rolBar
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use  
       * \param info-> some related info for the text added */
      void addText(std::string txt, std::string font, int size, int style,
                   int align, void* info=NULL);

      /*! Add the string to the rolBar
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font 
       * \param info-> some related info for the text added */
      void addText(std::string txt, std::string font, int size, int style,
                   int align, int R, int G, int B, void* info=NULL);

      /*! Set the first displayed line of the rolBar
       * \param line -> line number (from 0 to totalLines) */
      void setFirstLine(int line);

      /*! Get the current first displayed line on the rolBar
       * \return -> first displayed line */
      int getFirstLine();

      /*! Redraw the bar when scrolling text */
      void redraw();

      /*! Draw the RolBar (from guiObject) */
      void draw();

      /*! Render only the active text
       * \param i -> text number to render */
      void draw(int i);

      /*! Get last line text on the rolbar
       * /param -> text of the last line */
      std::string getLastLine();

      /*! Get related line info
       * \param line -> line number
       * \return related line info (if any) */
      void* getRelatedInfo(int line);

      /*! Get the related info of line under (x,y) position
       * \note (x,y) are at window coordinate system, not global. */
      void* getRelatedInfo(int x, int y);

   private:
      Button* up;              /**< Up Button */
      Button* down;            /**< Down Button */
      Button* actualPressed;   /**< Actual Pressed Button */
      TextBox* scrollText;     /**< Text of The Scroll */
      TextBox* contorn;        /**< Contorn of the Scroll */
      Button* position;        /**< Contorn Text Position */

      Uint32 lastUpdated;     /**< Last Time when updated */
      int maxHeight;          /**< Max Height */
      int style;              /**< Current Style */
      float deltaY;           /**< Current deltaY of the position button */
      int sizeY;              /**< The size of position button */
      int lastMouseY;         /**< Last mouse Y */
      
      GuiList* intList;       /**< Internal GUI Objects List */
};

}

#endif

