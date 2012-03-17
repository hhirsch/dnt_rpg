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

#include "dntfont.h"

#include "draw.h"
#include "farsoopts.h"

#include <iostream>
using namespace std;

/**********************************************************************
 *                            constructor                             *
 **********************************************************************/
loadedFontList::loadedFontList()
{
}

/**********************************************************************
 *                             destructor                             *
 **********************************************************************/
loadedFontList::~loadedFontList()
{
   loadedFont* fnt;
   std::list<loadedFont*>::iterator it;
   for(it=fonts.begin(); it != fonts.end(); it++)
   {
      fnt = (*it);
      if(fnt->font)
      {
         TTF_CloseFont(fnt->font);
      }
      delete(fnt);
   }
   fonts.clear();
}

/**********************************************************************
 *                               init                                 *
 **********************************************************************/
void dntFont::init()
{
   farsoOptions opts;
   /* Init SDL_ttf */
   if(TTF_Init() == -1)
   {
      cerr << "Can't init SDL_ttf : " << TTF_GetError() << endl;
      exit(3);
   }

   /* Create the list */
   fonts = new loadedFontList();

   /* Define some defaults */
   activeFont = NULL;
   activeFontAlign = DNT_FONT_ALIGN_LEFT;
   activeFontStyle = DNT_FONT_STYLE_NORMAL;
   defineFont(opts.getDefaultFont(),12);
}

/**********************************************************************
 *                                end                                 *
 **********************************************************************/
void dntFont::end()
{
   if(fonts != NULL)
   {
      delete(fonts);
   }

   /* Quit SDL_ttf */
   TTF_Quit();
}

/**********************************************************************
 *                            findFont                                *
 **********************************************************************/
loadedFont* dntFont::findFont(string fontName, int fontSize)
{
   std::list<loadedFont*>::iterator it;
   loadedFont* fnt;
   for(it=fonts->fonts.begin(); it != fonts->fonts.end(); it++)
   {
      fnt = (*it);
      if( (fontName == fnt->fontName) && (fontSize == fnt->fontSize) )
      {
         /* Found */
         return(fnt);
      }
   }
   /* Not found */
   return(NULL);
}

/**********************************************************************
 *                             loadFont                               *
 **********************************************************************/
loadedFont* dntFont::loadFont(string fontName, int fontSize)
{
   loadedFont* fnt = NULL;

   fnt = findFont(fontName, fontSize);

   if(!fnt)
   {
      /* Not found the font, so open it! */
      fnt = new loadedFont();
      fnt->fontName = fontName;
      fnt->fontSize = fontSize;
      fnt->font = TTF_OpenFont(fontName.c_str(), fontSize);

      if(!fnt->font)
      {
         cerr << "Can't open font file: " << fontName << endl;
         delete(fnt);
         return(NULL);
      }

      fonts->fonts.push_back(fnt);
   }

   return(fnt);
}

/**********************************************************************
 *                            defineFont                              *
 **********************************************************************/
bool dntFont::defineFont(string fileName, int size)
{
   if( (!activeFont) ||
       (activeFont->fontName != fileName) || 
       (activeFont->fontSize != size) )
   {
      activeFont = loadFont(fileName.c_str(), size);
   }
   return(true);
}

/**********************************************************************
 *                          defineFontAlign                           *
 **********************************************************************/
void dntFont::defineFontAlign(int align)
{
   activeFontAlign = align;
}

/**********************************************************************
 *                          defineFontStyle                           *
 **********************************************************************/
void dntFont::defineFontStyle(int style)
{
   activeFontStyle = style;
}

/**********************************************************************
 *                         defineFontOutline                          *
 **********************************************************************/
void dntFont::defineFontOutline(int outLine)
{
   if(activeFont)
   {
      TTF_SetFontOutline(activeFont->font, outLine);
   }
}

/**********************************************************************
 *                            renderText                              *
 **********************************************************************/
SDL_Surface* dntFont::renderText(string str, SDL_Color color, bool isUtf8)
{
   SDL_Surface* writeSurface = NULL;

   if(isUtf8)
   {
      writeSurface = TTF_RenderUTF8_Blended(activeFont->font, 
            str.c_str(), color);
   }
   else
   {
      writeSurface = TTF_RenderText_Blended(activeFont->font,
            str.c_str(), color);
   }

   return(writeSurface);
}

/***********************************************************************
 *                             blitText                                *
 ***********************************************************************/
void dntFont::blitText(SDL_Surface *screen, SDL_Surface* writeSurface, 
      int x,int y, string text, int x1,int y1,int x2,int y2, bool solid, 
      bool isUtf8)
{
   /* Define Position */
   if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
   {
      rect.x = ((x2 + x1) / 2) - 
         (getStringWidth(text, isUtf8) / 2)-1;
   }
   else
   {
      rect.x = x;
   }
   rect.y = y;

   /* Blit the result surface to the desired one on the desired 
    * position  */
   if(solid)
   {
      SDL_SetAlpha(writeSurface, 0, 0);
   }
   SDL_BlitSurface(writeSurface, NULL, screen, &rect);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int dntFont::write(SDL_Surface *screen,int x,int y,string text,int init,
                   int end, int x1,int y1,int x2,int y2, bool solid)
{
   farsoOptions opt;

   SDL_Color color;
   Uint8 a=0;

   int curY;
   int i;
   int maxWidth = x2 - x1;

   bool isUtf8 = false;

   SDL_Surface* writeSurface = NULL;

   int height = getHeight();

   string curLine = "";
   string potLine = "";
   string renderedText = "";

   /* Verify if avaible */
   if(!activeFont)
   {
      return(-1);
   }
   else
   {
      if(TTF_GetFontStyle(activeFont->font) != activeFontStyle)
      {
         TTF_SetFontStyle(activeFont->font, activeFontStyle);
      }
   }

   /* Verify if is on unicode special DNT button */
   if(text[0] == '\\')
   {
      text.erase(0,1);

      if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
      {
         x = ((x2 + x1) / 2) - (getStringWidth(text, true) / 2)-1;
      }
      writeUnicode(screen, x, y, text, solid);
      return(1);
   }

   /* Verify language code */
   isUtf8 = opt.isLanguageUnicode();

   /* Init things */
   color_Get(&color.r,&color.g, &color.b, &a);
   curY = y;

   for(i = init; i <= end ;i++)
   {
      if(text[i] != '|')
      {
         /* Normal character */
         if(text[i] == ' ')
         {
            /* Space */
            curLine += potLine;
            potLine = "";
         }

         /* Try to put character on the string */
         potLine += text[i];
         if(getStringWidth(curLine + potLine, isUtf8) > maxWidth)
         {
            /* Write the current thing  */
            renderedText = curLine;
            writeSurface = renderText(curLine, color, isUtf8);
            curLine = potLine.substr(1, potLine.length());
            potLine = "";
         }
      }
      else
      {
         /* Got an line break */
         renderedText = curLine + potLine;
         writeSurface = renderText(curLine + potLine, color, isUtf8);
         curLine = "";
         potLine = "";
      }

      if(writeSurface != NULL)
      {
         /* Blit the text */
         blitText(screen, writeSurface, x, curY, renderedText, x1, y1, x2, y2, 
               solid, isUtf8);
         
         /* Avoid memory leacks */
         SDL_FreeSurface(writeSurface);
         writeSurface = NULL;

         /* Next line! */
         curY += height;
      }
   }

   if((!potLine.empty()) || (!curLine.empty()))
   {
      renderedText = curLine + potLine;
      writeSurface = renderText(curLine + potLine, color, isUtf8);

      /* Blit the text */
      blitText(screen, writeSurface, x, curY, renderedText, x1, y1, x2, y2, 
            solid, isUtf8);  

      /* Avoid memory leacks */
      SDL_FreeSurface(writeSurface);
      writeSurface = NULL;
   }

   return(curY);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
void dntFont::writeSingleLine(SDL_Surface* screen, int x, int y, string text,
                              int x1, int y1, int x2, int y2, bool solid)
{
   farsoOptions opt;
   SDL_Surface* writeSurface; 
   SDL_Color color;
   Uint8 a=0;
   bool isUtf8=false;

   /* get color */
   color_Get(&color.r,&color.g, &color.b, &a);
   isUtf8 = opt.isLanguageUnicode();

   /* Write to a new surface */
   writeSurface = renderText(text, color, isUtf8);

   /* Blit the text */
   blitText(screen, writeSurface, x, y, text, x1, y1, x2, y2, 
         solid, isUtf8);  

   /* Avoid memory leacks */
   SDL_FreeSurface(writeSurface);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
void dntFont::write(SDL_Surface *screen,int x,int y,string text, bool solid)
{
   write(screen,x,y,text,0,text.length()-1,0,0,screen->w-1,screen->h-1,solid);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int dntFont::write(SDL_Surface *screen,int x, int y,string text,int x1,int y1,
                   int x2,int y2,bool solid)
{
   return(write(screen,x,y,text,0,text.length()-1,x1,y1,x2,y2,solid));
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
void dntFont::write(SDL_Surface *screen,int x,int y,string text,
                    int init,int end,bool solid)
{
   write(screen,x,y,text,init,end,0,0,screen->w-1,screen->h-1,solid);
}

/***********************************************************************
 *                            writeUnicode                             *
 ***********************************************************************/
void dntFont::writeUnicode(SDL_Surface* screen, int x, int y, string text,
                           bool solid)
{
   SDL_Color color;
   Uint8 a = 0;
   SDL_Rect rect;
   SDL_Surface* writeSurface;

   /* Get Color */
   color_Get(&color.r,&color.g, &color.b, &a);

   writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,
                                            (Uint16*)text.c_str(), 
                                             color);

   /* Blit the result surface to the desired one on the desired position  */
   if(solid)
   {
      SDL_SetAlpha(writeSurface, 0, 0);
   }
   rect.x = x;
   rect.y = y;
   SDL_BlitSurface(writeSurface, NULL, screen, &rect);

   /* Avoid memory leacks */
   SDL_FreeSurface(writeSurface);
}

/***********************************************************************
 *                           createUnicode                             *
 ***********************************************************************/
string dntFont::createUnicode(Uint16 character)
{
   Uint16 c[2];
   c[0] = character;
   c[1] = 0;
   string ret = "\\";
   ret += (char*)&c;
   return(ret);
}

/***********************************************************************
 *                          getStringWidth                             *
 ***********************************************************************/
int dntFont::getStringWidth(string s)
{
   farsoOptions opt;
   return(getStringWidth(s, opt.isLanguageUnicode()));
}

/***********************************************************************
 *                          getStringWidth                             *
 ***********************************************************************/
int dntFont::getStringWidth(string s, bool isUtf8)
{
   int w = 0;
   if(activeFont != NULL)
   {
      if(isUtf8)
      {
         TTF_SizeUTF8(activeFont->font, s.c_str(), &w, NULL);
      }
      else
      {
         TTF_SizeText(activeFont->font, s.c_str(), &w, NULL);
      }
   }
   return(w);
}

/***********************************************************************
 *                              getincCP                               *
 ***********************************************************************/
int dntFont::getIncCP()
{
   int adv = 0;
   if(activeFont)
   {
      TTF_GlyphMetrics(activeFont->font, 'm', NULL, NULL, NULL, NULL, &adv);
      return(adv);
   }
   return(0);
}

/***********************************************************************
 *                             getHeight                               *
 ***********************************************************************/
int dntFont::getHeight()
{
   if(activeFont)
   {
      return(TTF_FontHeight(activeFont->font));
   }
   return(0);
}

/***********************************************************************
 *                            getNextLine                              *
 ***********************************************************************/
string dntFont::getNextLine(string source, int& lastLinePos,
                            int maxWidth)
{
   farsoOptions opt;

   int i, lastSpace=-1;

   bool isUtf8 = false;

   string curLine = "";
   string potLine = "";
   string result = "";

   /* Verify if avaible */
   if(!activeFont)
   {
      return("");
   }
   else
   {
      if(TTF_GetFontStyle(activeFont->font) != activeFontStyle)
      {
         TTF_SetFontStyle(activeFont->font, activeFontStyle);
      }
   }

   /* Verify language code */
   isUtf8 = opt.isLanguageUnicode();

   for(i = lastLinePos; i < (int)source.length();i++)
   {
      if(source[i] != '|')
      {
         /* Normal character */
         if(source[i] == ' ')
         {
            /* Space */
            curLine += potLine;
            potLine = "";
            lastSpace = i;
         }

         /* Try to put character on the string */
         potLine += source[i];
         if(getStringWidth(curLine + potLine, isUtf8) > maxWidth)
         {
            /* Width overflow, got a line! */
            if(lastSpace >= 0)
            {
               lastLinePos = lastSpace+1;
               return(curLine);
            }
            else
            {
               /* Can't write! */
               lastLinePos = source.length();
               return("");
            }
         }
      }
      else
      {
         /* Got an line break */
         lastLinePos = i+1;
         return(curLine + potLine);
      }
   }

   if((!potLine.empty()) || (!curLine.empty()))
   {
      lastLinePos = source.length();
      return(curLine + potLine);
   }
   
   return("");
}

/* Static Variables */
loadedFont*      dntFont::activeFont;
loadedFontList*  dntFont::fonts=NULL;
int              dntFont::activeFontAlign;
int              dntFont::activeFontStyle;
SDL_Rect         dntFont::rect;

