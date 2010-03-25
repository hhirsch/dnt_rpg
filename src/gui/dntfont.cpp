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

#include "dntfont.h"

#include "draw.h"
#include "../engine/options.h"
#include "../etc/dirs.h"

#include <libintl.h>
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
   clearList();
}

/**********************************************************************
 *                              freeElement                           *
 **********************************************************************/
void loadedFontList::freeElement(dntListElement* obj)
{
   loadedFont* fnt = (loadedFont*)obj;
   
   /* Close the font, before delete */
   if(fnt->font)
   {
      TTF_CloseFont(fnt->font);
   }

   /* Delete it! */
   delete(fnt);
}

/**********************************************************************
 *                               init                                 *
 **********************************************************************/
void dntFont::init()
{
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
   defineFont(DNT_FONT_ARIAL,12);
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
   int i;
   loadedFont* fnt = (loadedFont*)fonts->getFirst();
   for(i = 0; i < fonts->getTotal(); i++)
   {
      if( (fontName == fnt->fontName) && (fontSize == fnt->fontSize) )
      {
         /* Found */
         return(fnt);
      }
      fnt = (loadedFont*)fnt->getNext();
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

   dirs dir;

   fnt = findFont(fontName, fontSize);

   if(!fnt)
   {
      /* Not found the font, so open it! */
      fnt = new loadedFont();
      fnt->fontName = fontName;
      fnt->fontSize = fontSize;
      fnt->font = TTF_OpenFont(dir.getRealFile(fontName).c_str(), fontSize);

      if(!fnt->font)
      {
         cerr << "Can't open font file: " << dir.getRealFile(fontName) << endl;
         delete(fnt);
         return(NULL);
      }

      fonts->insert(fnt);
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


/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int dntFont::write(SDL_Surface *screen,int x,int y,string text,int init,
                   int end, int x1,int y1,int x2,int y2, bool solid)
{
   int aux, k, curY, w = 0;
   int maxWidth = x2 - x1;
   int uni = 0;
   int last = -1, lastSpace = -1;
   strLine[0] = 0;

   Uint16* unicodeText;

   SDL_Color color;
   Uint8 a=0;

   SDL_Rect rect;
   SDL_Surface* writeSurface = NULL;

   int height = getHeight();

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

   /* Convert to unicode, if needed */
   unicodeText = convertToUnicode(curUnicode, text.c_str(), text.length());

   /* Verify if is on unicode special DNT button */
   if( (text[0] == '\\') )
   {
      text.erase(0,1);

      //FIXME Put it to work at this function, not calling another one!
      unicodeText = (Uint16*)text.c_str();
      if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
      {
         TTF_SizeUNICODE(activeFont->font, unicodeText, &w, NULL);
         x = ((x2 + x1) / 2) - (w / 2)-1;
      }
      writeUnicode(screen, x, y, text, solid);
      return(1);
   }

   /* Init things */
   color_Get(&color.r,&color.g, &color.b, &a);
   curY = y;
   rect.x = x;
   rect.y = y;

   for(aux=init;(aux<=end);aux++)
   {
      if(unicodeText[aux] != '|')
      {
         if(unicodeText[aux] == ' ')
         {
            lastSpace = uni;
         }
         strLine[uni] = unicodeText[aux];
         strLine[uni+1] = 0;
         uni++;
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         if(w >= maxWidth)
         {
            /* So, if the width is bigger, write the string without 
             * the characters after the last space, or without the last 
             * character */
            if(lastSpace != -1)
            {
               last = uni;
               strLine[lastSpace] = 0;
            }
            else
            {
               /* Ignore the last character */
               strLine[uni] = 0;
            }

            writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,
                                                     strLine, color);

            /* Define Position */
            if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
            {
               TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
               rect.x = ((x2 + x1) / 2) - (w / 2)-1;
            }
            else
            {
               rect.x = x;
            }
            rect.y = curY;

            /* Put the character */
            if(lastSpace != -1)
            {
               /* Copy all characters from the last space to the position */
               uni = 0;
               for(k=lastSpace+1; k < last; k++)
               {
                  strLine[uni] = strLine[k];
                  uni++;
               }
               strLine[uni] = 0;
            }
            else
            {
               /* Copy only the last character */
               strLine[0] = unicodeText[aux];
               uni = 1;
               strLine[uni] = 0;
            }
            lastSpace = -1;

            /* Blit the result surface to the desired one on the desired 
             * position  */
            if(solid)
            {
               SDL_SetAlpha(writeSurface, 0, 0);
            }
            SDL_BlitSurface(writeSurface, NULL, screen, &rect);

            /* Avoid memory leacks */
            SDL_FreeSurface(writeSurface);

            curY += height;
         }
         
      }
      else
      {
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         /* | breaks a line */

         writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,strLine,
                                                  color);

         /* Define Align */
         if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
         {
            TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
            rect.x = ((x2 + x1) / 2) - (w / 2)-1;
         }
         else
         {
            rect.x = x;
         }
         rect.y = curY;

         /* Blit the result surface to the desired one on the desired
          * position  */
         if(solid)
         {
            SDL_SetAlpha(writeSurface, 0, 0);
         }
         uni = 0;
         strLine[uni] = 0;
         SDL_BlitSurface(writeSurface, NULL, screen, &rect);

         /* Avoid memory leacks */
         SDL_FreeSurface(writeSurface);

         curY += height;
      }
   }

   if(uni != 0)
   {
      TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
      writeSurface = TTF_RenderUNICODE_Blended(activeFont->font, strLine,
                                               color);

      /* Blit the result surface to the desired one on the desired position  */
      rect.y = curY;
      if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
      {
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         rect.x = ((x2 + x1) / 2) - (w / 2)-1;
      }
      else
      {
         rect.x = x;
      }
      if(solid)
      {
         SDL_SetAlpha(writeSurface, 0, 0);
      }
      SDL_BlitSurface(writeSurface, NULL, screen, &rect);

      /* Avoid memory leacks */
      SDL_FreeSurface(writeSurface);
   }

   return(curY);
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
 *                         convertToUnicode                            *
 ***********************************************************************/
Uint16* dntFont::convertToUnicode(Uint16 *unicode, const char *text, int len)
{
   int i,j;
   Uint16 ch;

   options opt;
   if(opt.isLanguageUnicode())
   {
      /*for( i=0; i < (len / 2); ++i ) 
      {
         unicode[i] = text[i];
      }
      unicode[i] = 0;*/
      /**/

      for ( i = 0, j = 0; i < len; ++i, ++j ) 
      {
         ch = ( ( const unsigned char * )text)[i];
         if ( ch >= 0xF0 ) 
         {
            ch  =  ( Uint16 )( text[i] & 0x07 ) << 18;
            ch |=  ( Uint16 )( text[++i] & 0x3F ) << 12;
            ch |=  ( Uint16 )( text[++i] & 0x3F ) << 6;
            ch |=  ( Uint16 )( text[++i] & 0x3F );
         } 
         else if ( ch >= 0xE0 ) 
         {
            ch  =  ( Uint16 )( text[i] & 0x0F ) << 12;
            ch |=  ( Uint16 )( text[++i] & 0x3F ) << 6;
            ch |=  ( Uint16 )( text[++i] & 0x3F );
         } 
         else if ( ch >= 0xC0 ) 
         {
            ch  =  ( Uint16 )( text[i] & 0x1F ) << 6;
            ch |=  ( Uint16 )( text[++i] & 0x3F );
         }
         unicode[j] = ch;
      }
      unicode[j] = 0;

      return(unicode);
   }
   else
   {
       // this is the LATIN1 to unicode. Put the others convertions
      for( i=0; i < len; ++i ) 
      {
         unicode[i] = ((const unsigned char *)text)[i];
      }
      unicode[i] = 0;
   }

   //FIXME the size of the string returned!
   return(unicode);
}

/***********************************************************************
 *                             copyUnicode                             *
 ***********************************************************************/
Uint16* dntFont::copyUnicode(Uint16 *uni, int len)
{
   int i;
   for( i=0; i < len; ++i ) 
   {
      curUnicode[i] = uni[i];
   }
   curUnicode[i] = 0;
   return(curUnicode);
}


/***********************************************************************
 *                          getStringWidth                             *
 ***********************************************************************/
int dntFont::getStringWidth(string s)
{
   int w = 0;
   Uint16* uniStr = convertToUnicode(curUnicode, s.c_str(), s.length());
   if(activeFont != NULL)
   {
      TTF_SizeUNICODE(activeFont->font, uniStr, &w, NULL);
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
 *                          unicodeToString                            *
 ***********************************************************************/
string unicodeToString(Uint16* unicode, int size)
{
   int i;
   string res = "";
   //FIXME, when is UTF8, not Latin!
   for(i = 0; i < size; i++)
   {
      res += (char)unicode[i];
   }
   return(res);
}

/***********************************************************************
 *                            getNextLine                              *
 ***********************************************************************/
string dntFont::getNextLine(string source, int& lastLinePos,
                            int maxWidth)
{
   string result = "";
   Uint16* unicodeText;
   int w, uni = 0;
   int last = -1, lastSpace = -1;
   int lastRealSpacePos = -1;
   bool lineGot = false;

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



   /* Convert to unicode, if needed */
   unicodeText = convertToUnicode(curUnicode, source.c_str(), source.length());

   int aux;
   for(aux=lastLinePos; 
       ( (aux < (int)source.length()) && (!lineGot) ); aux++)
   {
      if(unicodeText[aux] != '|')
      {
         if(unicodeText[aux] == ' ')
         {
            lastSpace = uni;
            lastRealSpacePos = aux;
         }
         strLine[uni] = unicodeText[aux];
         strLine[uni+1] = 0;
         uni++;
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         if(w >= maxWidth)
         {
            /* So, if the width is bigger, write the string without 
             * the characters after the last space, or without the last 
             * character */
            if(lastSpace != -1)
            {
               lastLinePos = lastRealSpacePos+1;
               last = uni;
               strLine[lastSpace] = 0;
               result += unicodeToString(strLine, lastSpace);
            }
            else
            {
               lastLinePos = aux;
               /* Ignore the last character */
               strLine[uni] = 0;
               result += unicodeToString(strLine, uni);
            }
            lineGot = true;

            lastSpace = -1;
         }
         
      }
      else
      {
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         /* | breaks a line */

         result += unicodeToString(strLine, uni);
         /* Jump the | */
         lastLinePos = aux+1;
         lineGot = true;
         uni = 0;
         //strLine[uni] = 0;
      }
   }

   if( (!lineGot) && (uni != 0))
   {
      TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
      result += unicodeToString(strLine, uni);
      lastLinePos = aux;
   }


   return(result);
}

/* Static Variables */
loadedFont*      dntFont::activeFont;
loadedFontList*  dntFont::fonts=NULL;
int              dntFont::activeFontAlign;
int              dntFont::activeFontStyle;

