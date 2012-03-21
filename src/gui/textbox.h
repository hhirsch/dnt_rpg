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

#ifndef _farso_textbox_h
#define _farso_textbox_h

#include <list>

#include "color.h"
#include "draw.h"
#include "dntfont.h"
#include "guiobject.h"

#include <string>

namespace Farso
{


/*! The textLine represents each line of text in the
 * textBox, with his own font parameters and colors */
class TextLine
{  
   public:
      std::string text;    /**< Text of the line */
      int R,               /**< Red Color */
          G,               /**< Green Color */
          B;               /**< Blue Color */
      std::string fontName;/**< Name of the Font Used */
      int fontSize;        /**< Size of the font */
      int fontAlign;       /**< Align of the font */
      int fontStyle;       /**< Style of the font */
      int height;          /**< Height of the line */
      void* relatedInfo;   /**< Pointer to some related info */
};

/*! Class of text box representation. */
class TextBox: public GuiObject
{
   public:

      /*! The TextBox styles */
      enum TextBoxStyleEnum
      {
         STYLE_NOT_FRAMED=0,         /**< Text without frames */
         STYLE_FRAMED_FILLED,        /**< Text with frames and BG color */
         STYLE_FRAMED_TRANSPARENT,   /**< Text with frames and transparent */
         STYLE_NOT_FRAMED_FILLED     /**< Without frames, but filled */
      };

      /*! Constructor 
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate
       * \param framedType -> type of the frame. 0 for no frames.
       * \param screen -> surface to draw to */
      TextBox(int xa, int ya, int xb, int yb, int framedType,
              SDL_Surface *screen);
      /*! Destructor */
      ~TextBox();

      /*! Get total lines on current the text box
       * \return -> totalLines on the textBox */
      int getTotalLines();

      /*! Draw in the Surface the text bar
       * \return -> number of the last writed line */
      int draw2();

      /*! Draw only the line number 'i'
       * \param i -> number of the line to draw */
      void draw(int i);

      /*! Draw the textbar */
      void draw();
      
      /*! Define the Font
       * \param name -> font file name
       * \param align -> font aligment
       * \param size -> font size */
      void setFont(std::string name, int size, int align);

      /*! Define the Font
       * \param name -> font file name
       * \param align -> font aligment
       * \param size -> font size
       * \param style -> style constant */
      void setFont(std::string name, int size, int align, int style);

      /*! Set the text color
       * \param R -> red color
       * \param G -> green color
       * \param B -> blue color */
      void setColor(int R, int G, int B);

      /*! Set the Back color
       * \param R -> red color
       * \param G -> green color
       * \param B -> blue color */
      void setBackColor(int R, int G, int B);

      /*! Get the text from the line number
       * \param line -> number of line to get text from 
       * \note -> first line is 0. */
      std::string getLineText(int line);

      /*! Set the object text
       * \param txt -> new text */
      void setText(std::string txt);

      /*! Get the number of the last drawable line
       * \return -> number of the last drawable line */
      int lastDrawableLine();

      /*! Set the first displayed line of the textBox
       * \param line -> line number (from 0 to totalLines) */
      void setFirstLine(int line);

      /*! Set the last displayed line of the textbox, showing as many lines
       * as possible
       * \param line -> number of the line to be the last one */
      void setLastLine(int line);

      /*! Get the current first displayed line on the textBox
       * \return -> first displayed line */
      int getFirstLine();

      /*! Add the string to the textBox
       * \param txt -> text screen to add 
       * \param info-> some related info for the text added */
      void addText(std::string txt, void* info=NULL);

      /*! Add the string to the textBox
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use  
       * \param info-> some related info for the text added */
      void addText(std::string txt, std::string font, int size,
                   int align, int style, void* info=NULL);

      /*! Add the string to the textBox
       * \param txt -> text screen to add
       * \param font -> font file to use
       * \param size -> size of the font to use
       * \param align -> alignment of the font to use
       * \param style -> style of the font to use
       * \param R -> red color of the font
       * \param G -> green color of the font
       * \param B -> blue color of the font 
       * \param info-> some related info for the text added */
      void addText(std::string txt, std::string font, int size,
                   int align, int style, int R, int G, int B, void* info=NULL);

      /*! Verify if the text box receives events or not
       * \return true if receive events*/
      bool receiveEvents();

      /*! Set if the text box receive events
       * \param b -> true to receive events*/
      void setReceiveEvents(bool b);

      /*! Set the textBox to SolidMode (allowing, for example, correct
       * render on empty - with full alpha - surfaces) */
      void setSolid();

      /*! Get related line info
       * \param line -> line number
       * \return related line info (if any) */
      void* getRelatedInfo(int line);

      /*! Get the related info of line under (x,y) position
       * \note (x,y) are at window coordinate system, not global. */
      void* getRelatedInfo(int x, int y);
   
   private:

      /*! Clear the lines list */
      void clear();

      /*! Create the text lines
       * \param txt -> string base to create the lines */
      void createLines(std::string txt, std::string font, int size,
                       int align, int style, int R, int G, int B, void* info);

      /*! Insert textLine to the list
       * \param line -> line to insert to the list */
      void insertLine(TextLine* line);

      /*! Get the related text line */
      TextLine* getTextLine(int line);

      SDL_Surface* wSurface; /**< Window Surface */
      
      int firstLine;       /**< Actual Init Line Draw */

      bool recEvents;      /**< If receive events or not */

      int framed;          /**< Type of the frame. 0 for none */
      std::string fontName;/**< Font FileName */
      int fontAlign;       /**< Font Align */
      int fontSize;        /**< Font Size */
      int fontStyle;       /**< Font Style */
      bool solid;          /**< Solid Mode */
      Colors colors;       /**< Colors */

      std::list<TextLine*>lines; /**< Text line list */
};

}

#endif

