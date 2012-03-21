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
#include "farsoopts.h"
using namespace std;
using namespace Farso;

/*******************************************************
 *                       Constructor                   *
 *******************************************************/
TextBox::TextBox(int xa, int ya, int xb, int yb, int frameType,
                 SDL_Surface *screen): 
                 GuiObject(screen)
{
   Options opt;

   wSurface = screen;
   type = Farso::OBJECT_TEXT_BOX;
   x1 = xa;
   y1 = ya;
   x2 = xb;
   y2 = yb;
   framed = frameType;
   fontAlign = Font::ALIGN_LEFT;
   fontName = opt.getDefaultFont();
   fontStyle = Font::STYLE_NORMAL;
   fontSize = 10;
   firstLine = 0;
   recEvents = false;
   solid = false;
}

/*******************************************************
 *                        Destructor                   *
 *******************************************************/
TextBox::~TextBox()
{
   clear();
}

/*******************************************************
 *                        clear                        *
 *******************************************************/
void TextBox::clear()
{
   std::list<TextLine*>::iterator it;
   for(it = lines.begin(); it != lines.end(); it++)
   {
      delete(*it);
   }
   lines.clear();
}

/*******************************************************
 *                       setSolid                      *
 *******************************************************/
void TextBox::setSolid()
{
   solid = true;
}

/*******************************************************
 *                      getTextLine                    *
 *******************************************************/
string TextBox::getLineText(int line)
{
   TextLine* t = getTextLine(line);
   if(t)
   {
      return(t->text);
   }

   return("");
}

/*******************************************************
 *                    getRelatedInfo                   *
 *******************************************************/
void* TextBox::getRelatedInfo(int line)
{
   TextLine* t = getTextLine(line);
   if(t)
   {
      return(t->relatedInfo);
   }

   return(NULL);
}

/*******************************************************
 *                      getTextLine                    *
 *******************************************************/
TextLine* TextBox::getTextLine(int line)
{
   int i;
   if(line < (int)lines.size())
   {
      /* Get the line */
      std::list<TextLine*>::iterator it = lines.begin();
      for(i = 0; i < line; i++)
      {
         it++;
      }
      /* Get the info from line */
      return(*it);
   }

   return(NULL);
}

/*******************************************************
 *                        setText                      *
 *******************************************************/
void TextBox::setText(string txt)
{
   if(text != txt)
   {
      clear();
      text = txt;
      createLines(txt, fontName, fontSize, fontAlign, fontStyle,
                  colors.colorText.R, colors.colorText.G,
                  colors.colorText.B, NULL);
      draw();
   }
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void TextBox::addText(string txt, string font, int size,
                      int align, int style, int R, int G, int B,
                      void* info)
{
   text += txt;
   createLines(txt, font, size, align, style, R, G, B, info);
   draw();
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void TextBox::addText(string txt, string font, int size,
                      int align, int style, void* info)
{
   addText(txt, font, size, align, style,
               colors.colorText.R, colors.colorText.G,
               colors.colorText.B, info);
}

/*******************************************************
 *                        addText                      *
 *******************************************************/
void TextBox::addText(string txt, void* info)
{
   addText(txt, fontName, fontSize, fontAlign, fontStyle, 
               colors.colorText.R, colors.colorText.G,
               colors.colorText.B, info);
}

/*******************************************************
 *                  lastDrawableLines                  *
 *******************************************************/
int TextBox::lastDrawableLine()
{
   int i;
   TextLine* line;
   int lastLine = 0;
   int height = 0;

   /* Verify if exist lines */
   if(lines.size() <= 0)
   {
      return(0);
   }

   /* Get first text line */
   std::list<TextLine*>::iterator it = lines.begin();
   for(i = 0; i < firstLine; i++)
   {
      /* height must be the previous one. */
      height = (*it)->height;
   }
   line = (*it);
   
   int y = y1+2;
   /* Draw the text lines */
   for(i = firstLine; ( ( (y+height) < y2) && (i < (int)lines.size()) ) ; i++)
   {
      lastLine = i;
      if( i < (int)lines.size())
      {
         y += line->height;
         height = line->height;
         it++;
      }
   }
   return(lastLine);
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
void TextBox::draw()
{
   draw2();
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
void TextBox::draw(int i)
{
   int l;
   TextLine* line;
   Font fnt;

   /* No need to draw if not before first line or hidden */
   if( (i < firstLine) || (!isVisible()) || (i > (int)lines.size()) )
   {
      return;
   }

   if(lines.size() > 0)
   {
      /* Get desired text line */
      int y = y1+2;
      std::list<TextLine*>::iterator it = lines.begin();
      for(l = 0; (l < i); l++)
      {
         if(l >= firstLine)
         {
            y += (*it)->height;
         }
         it++;
      }
      line = (*it);
  
      /* Draw the desired line */
      if(line)
      {
         color_Set(0,0,0,255);
         fnt.defineFont(line->fontName, line->fontSize);
         fnt.defineFontAlign(line->fontAlign);
         fnt.defineFontStyle(line->fontStyle);
         fnt.writeSingleLine(wSurface, x1+2, y, line->text, 
               x1+2, y, x2, y2, solid);

         setChanged();
      }
   }
}

/*******************************************************
 *                          draw                       *
 *******************************************************/
int TextBox::draw2()
{
   int i;
   TextLine* line;
   Font fnt;
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
         color_Set(colors.colorButton.R,
                   colors.colorButton.G,
                   colors.colorButton.B,
                   colors.colorButton.A);
         rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
      }
      
      if(framed != 3)
      {
         color_Set(colors.colorCont[0].R, colors.colorCont[0].G,
                   colors.colorCont[0].B, colors.colorCont[0].A);
         rectangle_2Colors(wSurface,x1,y1,x2,y2,
                          colors.colorCont[1].R, colors.colorCont[1].G,
                          colors.colorCont[1].B, colors.colorCont[1].A);
      }
   }
   else if(solid)
   {
      /* Must set an empty background */
      color_Set(255, 255, 255, 0);
      rectangle_Fill(wSurface,x1+1,y1+1,x2-1,y2-1);
   }

   if(lines.size() > 0)
   {
      /* Get first text line */
      std::list<TextLine*>::iterator it = lines.begin();
      for(i = 0; i < firstLine; i++)
      {
         it++;
      }
   
      int y = y1+2;
      /* Draw the text lines */
      for(i = firstLine; ( (y+height < y2) && (i < (int)lines.size()) ) ; i++)
      {
         line = (*it);
         lastLine = i;
         color_Set(line->R,line->G,line->B,255);
         fnt.defineFont(line->fontName, line->fontSize);
         fnt.defineFontAlign(line->fontAlign);
         fnt.defineFontStyle(line->fontStyle);
         fnt.writeSingleLine(wSurface, x1+2, y, line->text, 
               x1+2, y, x2, y2, solid);
         y += line->height;
         height = line->height;
         it++;
      }
      setChanged();
   }
   return(lastLine);
}

/*******************************************************
 *                     getTotalLines                   *
 *******************************************************/
int TextBox::getTotalLines()
{
   return((int)lines.size());
}

/*******************************************************
 *                      getFirstLine                   *
 *******************************************************/
int TextBox::getFirstLine()
{
   return(firstLine);
}

/*******************************************************
 *                      setFirstLine                   *
 *******************************************************/
void TextBox::setFirstLine(int line)
{
   firstLine = line;
}

/*******************************************************
 *                      setLastLine                   *
 *******************************************************/
void TextBox::setLastLine(int line)
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
void TextBox::setFont(string name, int size, int align)
{
   setFont(name, size, align, Font::STYLE_NORMAL);
}

/*******************************************************
 *                        setFont                      *
 *******************************************************/
void TextBox::setFont(string name, int size, int align, int style)
{
   fontName = name;
   fontAlign = align;
   fontSize = size;
   fontStyle = style;

   /* Change all current Lines */
   TextLine* line;
   std::list<TextLine*>::iterator it = lines.begin();
   for(it = lines.begin(); it != lines.end(); it++)
   {
      line = (*it);
      line->fontName = fontName;
      line->fontAlign = fontAlign;
      line->fontSize = fontSize;
      line->fontStyle = fontStyle;
   }
}

/*******************************************************
 *                       setColor                      *
 *******************************************************/
void TextBox::setColor(int R, int G, int B)
{
   colors.colorText.R = R;
   colors.colorText.G = G;
   colors.colorText.B = B;

   /* Change all current Lines */
   TextLine* line;
   std::list<TextLine*>::iterator it = lines.begin();
   for(it = lines.begin(); it != lines.end(); it++)
   {
      line = (*it);
      line->R = R;
      line->G = G;
      line->B = B;
   }
}

/*******************************************************
 *                     setBackColor                    *
 *******************************************************/
void TextBox::setBackColor(int R, int G, int B)
{
   colors.colorButton.R = R;
   colors.colorButton.G = G;
   colors.colorButton.B = B;
}

/*******************************************************
 *                    receiveEvents                    *
 *******************************************************/
bool TextBox::receiveEvents()
{
   return(recEvents);
}

/*******************************************************
 *                    setReceiveEvents                 *
 *******************************************************/
void TextBox::setReceiveEvents(bool b)
{
   recEvents = b;
}

/*******************************************************
 *                     insertLine                      *
 *******************************************************/
void TextBox::insertLine(TextLine* line)
{
   lines.push_back(line);
}

/*******************************************************
 *                     createLines                     *
 *******************************************************/
void TextBox::createLines(string txt, string font, int size,
                          int align, int style, int R, int G, int B,
                          void* info)
{
   Font fnt;
   fnt.defineFont(font, size);
   fnt.defineFontAlign(align);
   fnt.defineFontStyle(style);

   TextLine* line = NULL;
   int lastPos = 0;

   while(lastPos < (int)txt.length())
   {
      line = new TextLine();
      line->R = R; 
      line->G = G;
      line->B = B;
      line->fontName = font;
      line->fontSize = size;
      line->fontAlign = align;
      line->fontStyle = style;
      line->text = fnt.getNextLine(txt, lastPos, x2-x1-2);
      line->height = fnt.getHeight();
      line->relatedInfo = info;
      insertLine(line);
   }
}

