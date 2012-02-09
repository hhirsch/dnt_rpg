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

#include "textbox.h"
using namespace std;

/*******************************************************
 *                       Constructor                   *
 *******************************************************/
textBox::textBox(int xa, int ya, int xb, int yb, int frameType,
                 SDL_Surface *screen): 
                 guiObject(screen),dntList(DNT_LIST_TYPE_ADD_AT_END)
{
   wSurface = screen;
   type = FARSO_OBJECT_TEXT_BOX;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   framed = frameType;
   fontAlign = DNT_FONT_ALIGN_LEFT;
   fontName = DNT_FONT_ARIAL;
   fontStyle = DNT_FONT_STYLE_NORMAL;
   fontSize = 10;
   firstLine = 0;
   recEvents = false;
}

/*******************************************************
 *                        Destructor                   *
 *******************************************************/
textBox::~textBox()
{
   clear();
}

/*******************************************************
 *                        clear                        *
 *******************************************************/
void textBox::clear()
{
   clearList();
}

/*******************************************************
 *                     freeElement                     *
 *******************************************************/
void textBox::freeElement(dntListElement* obj)
{
   textLine* t = (textLine*)obj;
   delete(t);
}

/*******************************************************
 *                      getTextLine                    *
 *******************************************************/
string textBox::getTextLine(int line)
{
   int i;
   if(line < total)
   {
      /* Get the line */
      textLine* tmp = (textLine*)first;
      for(i = 0; i < line; i++)
      {
         tmp = (textLine*)tmp->getNext();
      }
      /* Get the text from line */
      return(tmp->text);
   }

   return("");
}

/*******************************************************
 *                        setText                      *
 *******************************************************/
void textBox::setText(string txt)
{
   if(text != txt)
   {
      clear();
      text = txt;
      createLines(txt, fontName, fontSize, fontAlign, fontStyle,
                  Colors.colorText.R, Colors.colorText.G,
                  Colors.colorText.B);
      draw();
   }
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt, string font, int size,
                      int align, int style, int R, int G, int B)
{
   text += txt;
   createLines(txt, font, size, align, style, R, G, B);
   draw();
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt, string font, int size,
                      int align, int style)
{
   addText(txt, font, size, align, style,
               Colors.colorText.R, Colors.colorText.G,
               Colors.colorText.B);
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void textBox::addText(string txt)
{
   addText(txt, fontName, fontSize, fontAlign, fontStyle, 
               Colors.colorText.R, Colors.colorText.G,
               Colors.colorText.B);
}

/*******************************************************
 *                  lastDrawableLines                  *
 *******************************************************/
int textBox::lastDrawableLine()
{
   int i;
   textLine* line;
   int lastLine = 0;
   int height = 0;

   /* Verify if exist lines */
   if(total <= 0)
   {
      return(0);
   }

   /* Get first text line */
   line = (textLine*)first;
   for(i = 0; i < firstLine; i++)
   {
      height = line->height;
      line = (textLine*)line->getNext();
   }
   
   int y = y1+2;
   /* Draw the text lines */
   for(i = firstLine; ( ( (y+height) < y2) && (i < total) ) ; i++)
   {
      lastLine = i;
      if( i < total)
      {
         y += line->height;
         height = line->height;
         line = (textLine*)line->getNext();
      }
   }
   return(lastLine);
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
void textBox::draw()
{
   draw2();
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
void textBox::draw(int i)
{
   int l;
   textLine* line;
   dntFont fnt;

   /* No need to draw if not before first line or hidden */
   if( (i < firstLine) || (!isVisible()) )
   {
      return;
   }

   if(total > 0)
   {
      /* Get desired text line */
      int y = y1+2;
      line = (textLine*) first;
      for(l = 0; ((l < i) && (line != NULL)); l++)
      {
         if(l >= firstLine)
         {
            y += line->height;
         }
         line = (textLine*)line->getNext();
      }
  
      /* Draw the desired line */
      if(line)
      {
         color_Set(0,0,0,255);
         fnt.defineFont(line->fontName, line->fontSize);
         fnt.defineFontAlign(line->fontAlign);
         fnt.defineFontStyle(line->fontStyle);
         fnt.writeSingleLine(wSurface, x1+2, y, line->text, x1+2, y, x2, y2);

         setChanged();
      }
   }
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
int textBox::draw2()
{
   int i;
   textLine* line;
   dntFont fnt;
   int lastLine = 0;
   int height = 0;
   
   /* No draw when hidden */
   if(!isVisible())
   {
      return(lastLine);
   }

   /* Draw Background */
   if(framed)
   {
      if( (framed == 1) || (framed == 3))
      {
         color_Set(Colors.colorButton.R,
                   Colors.colorButton.G,
                   Colors.colorButton.B,
                   Colors.colorButton.A);
         rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
      }
      
      if(framed != 3)
      {
         color_Set(Colors.colorCont[0].R, Colors.colorCont[0].G,
                   Colors.colorCont[0].B, Colors.colorCont[0].A);
         rectangle_2Colors(wSurface,x1,y1,x2,y2,
                          Colors.colorCont[1].R, Colors.colorCont[1].G,
                          Colors.colorCont[1].B, Colors.colorCont[1].A);
      }
   }

   if(total > 0)
   {
      /* Get first text line */
      line = (textLine*)first;
      for(i = 0; i < firstLine; i++)
      {
         line = (textLine*)line->getNext();
      }
   
      int y = y1+2;
      /* Draw the text lines */
      for(i = firstLine; ( (y+height < y2) && (i < total) ) ; i++)
      {
         lastLine = i;
         color_Set(line->R,line->G,line->B,255);
         fnt.defineFont(line->fontName, line->fontSize);
         fnt.defineFontAlign(line->fontAlign);
         fnt.defineFontStyle(line->fontStyle);
         fnt.writeSingleLine(wSurface, x1+2, y, line->text, x1+2, y, x2, y2);
         y += line->height;
         height = line->height;
         line = (textLine*)line->getNext();
      }
      setChanged();
   }
   return(lastLine);
}

/*******************************************************
 *                     getTotalLines                   *
 *******************************************************/
int textBox::getTotalLines()
{
   return(total);
}

/*******************************************************
 *                      getFirstLine                   *
 *******************************************************/
int textBox::getFirstLine()
{
   return(firstLine);
}

/*******************************************************
 *                      setFirstLine                   *
 *******************************************************/
void textBox::setFirstLine(int line)
{
   firstLine = line;
}

/*******************************************************
 *                      setLastLine                   *
 *******************************************************/
void textBox::setLastLine(int line)
{
   int cur = line;
   
   /* Find the lesser first line that the last one is
    * still the desired. */
   setFirstLine(line);
   while( (lastDrawableLine() >= line) && (cur > 0) )
   {
      cur--;
      setFirstLine(cur);
   }

   if( (cur > 0) && (cur < line)) //to avoid can't write the last!
   {
      /* So, it's the previous one  */
      cur++;
      setFirstLine(cur);
   }
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align)
{
   setFont(name, size, align, DNT_FONT_STYLE_NORMAL);
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void textBox::setFont(string name, int size, int align, int style)
{
   fontName = name;
   fontAlign = align;
   fontSize = size;
   fontStyle = style;

   /* Change all current Lines */
   int i;
   textLine* line = (textLine*)first;
   for(i = 0; i < total; i++)
   {
      line->fontName = fontName;
      line->fontAlign = fontAlign;
      line->fontSize = fontSize;
      line->fontStyle = fontStyle;
      line = (textLine*)line->getNext();
   }
}

/*******************************************************
 *                       setColor                      *
 *******************************************************/
void textBox::setColor(int R, int G, int B)
{
   Colors.colorText.R = R;
   Colors.colorText.G = G;
   Colors.colorText.B = B;

   /* Change all current Lines */
   int i;
   textLine* line = (textLine*)first;
   for(i = 0; i < total; i++)
   {
      line->R = R;
      line->G = G;
      line->B = B;
      line = (textLine*)line->getNext();
   }
}

/*******************************************************
 *                     setBackColor                    *
 *******************************************************/
void textBox::setBackColor(int R, int G, int B)
{
   Colors.colorButton.R = R;
   Colors.colorButton.G = G;
   Colors.colorButton.B = B;
}

/*******************************************************
 *                    receiveEvents                    *
 *******************************************************/
bool textBox::receiveEvents()
{
   return(recEvents);
}

/*******************************************************
 *                    setReceiveEvents                 *
 *******************************************************/
void textBox::setReceiveEvents(bool b)
{
   recEvents = b;
}

/*******************************************************
 *                     insertLine                      *
 *******************************************************/
void textBox::insertLine(textLine* line)
{
   insert(line);
}

/*******************************************************
 *                     createLines                     *
 *******************************************************/
void textBox::createLines(string txt, string font, int size,
                          int align, int style, int R, int G, int B)
{
   dntFont fnt;
   fnt.defineFont(font, size);
   fnt.defineFontAlign(align);
   fnt.defineFontStyle(style);

   textLine* line = NULL;
   int lastPos = 0;

   while(lastPos < (int)txt.length())
   {
      line = new textLine();
      line->R = R; 
      line->G = G;
      line->B = B;
      line->fontName = font;
      line->fontSize = size;
      line->fontAlign = align;
      line->fontStyle = style;
      line->text = fnt.getNextLine(txt, lastPos, x2-x1-2);
      line->height = fnt.getHeight();
      insertLine(line);
   }
}

