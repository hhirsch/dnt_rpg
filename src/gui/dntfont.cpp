#include "dntfont.h"
#include "draw.h"
#include "../engine/options.h"
#include "../etc/dirs.h"
#include <libintl.h>

/**********************************************************************
 *                               init                                 *
 **********************************************************************/
void dntFont::init()
{
   /* Init SDL_ttf */
    if(TTF_Init() == -1)
    {
       printf(gettext("Can't init SDL_ttf : %s\n"), TTF_GetError());
       exit(3);
    }
    fontsList = NULL;
    totalFonts = 0;
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
   int i;
   loadedFont* fnt = fontsList;
   loadedFont* tmp;

   /* Close and delete all fonts on the list */
   for(i = 0; i < totalFonts; i++)
   {
      tmp = fnt;
      fnt = fnt->next;
      if(tmp->font)
      {
         TTF_CloseFont(tmp->font);
      }
      delete(tmp);
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
   loadedFont* fnt = fontsList;
   for(i = 0; i < totalFonts; i++)
   {
      if( (fontName == fnt->fontName) && (fontSize == fnt->fontSize) )
      {
         /* Found */
         return(fnt);
      }
      fnt = fnt->next;
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
         printf(gettext("Can't open font file: %s\n"), 
                dir.getRealFile(fontName).c_str());
         return(NULL);
      }

      if(fontsList == NULL)
      {
         fontsList = fnt;
         fnt->next = fnt;
         fnt->previous = fnt;
      }
      else
      {
         fnt->next = fontsList;
         fnt->previous = fontsList->previous;
         fnt->next->previous = fnt;
         fnt->previous->next = fnt;
         fontsList = fnt;
      }

      totalFonts++;
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

   //TTF_SetFontStyle(activeFont->font, TTF_STYLE_UNDERLINE );

   /* Convert to unicode, if needed */
   unicodeText = convertToUnicode(curUnicode, text.c_str(), text.length());

   /* Verify if is on unicode special DNT button */
   if( (text[0] == '\\') )
   {
      text.erase(0,1);
      /*end--;

      if(end > (int)((text.length() / 2)-1))
      {
         end = (text.length() / 2)-1;
      }

      unicodeText = copyUnicode((Uint16*)text.c_str(), end+1);*/
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
   color_Get(&color.r,&color.g, &color.b);
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

            /* Write with the font */
            if(solid)
            {
               writeSurface = TTF_RenderUNICODE_Solid(activeFont->font,strLine, 
                                                      color);
            }
            else
            {
               writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,
                                                        strLine, color);
            }

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

         if(solid)
         {
            writeSurface = TTF_RenderUNICODE_Solid(activeFont->font,strLine,
                                                   color);
         }
         else
         {
            writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,strLine,
                                                     color);
         }

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
      /* Remaining things to write */
      if(solid)
      {
         writeSurface = TTF_RenderUNICODE_Solid(activeFont->font, strLine,
                                                color);
      }
      else
      {
         writeSurface = TTF_RenderUNICODE_Blended(activeFont->font, strLine,
                                                  color);
      }

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
   SDL_Rect rect;
   SDL_Surface* writeSurface;

   /* Get Color */
   color_Get(&color.r,&color.g, &color.b);

   /* Write Unicode Text */
   if(solid)
   {
      writeSurface = TTF_RenderUNICODE_Solid(activeFont->font,
                                             (Uint16*)text.c_str(), 
                                             color);
   }
   else
   {
      writeSurface = TTF_RenderUNICODE_Blended(activeFont->font,
                                               (Uint16*)text.c_str(), 
                                                color);
   }

   /* Blit the result surface to the desired one on the desired position  */
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
   int i;

   options opt;
   if(opt.isLanguageUnicode())
   {
      for( i=0; i < (len / 2); ++i ) 
      {
         unicode[i] = text[i];
      }
      unicode[i] = 0;
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
 *                           getTotalLines                             *
 ***********************************************************************/
int dntFont::getTotalLines(string source, int x1, int x2)
{
   int i,k,last;
   int lines = -1;

   int maxWidth = x2 - x1;
   int w;

   Uint16* uniStr = convertToUnicode(curUnicode,source.c_str(),source.length());
   int uni = 0;
   int lastSpace = -1;
   strLine[0] = 0;

   //FIXME the size!
   int size = (int)source.length();

   if(!activeFont)
   {
      return(0);
   }
   else
   {
      if(TTF_GetFontStyle(activeFont->font) != activeFontStyle)
      {
         TTF_SetFontStyle(activeFont->font, activeFontStyle);
      }
   } 

   for(i=0; (i < size) ; i++)
   {
      if(uniStr[i] == '|')
      {
         lines++;
      }
      else
      {
         if(uniStr[i] == ' ')
         {
            lastSpace = uni;
         }
         strLine[uni] = uniStr[i];
         uni++;
         strLine[uni] = 0;
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         if(w >= maxWidth)
         {
            lines++;
            if(lastSpace != -1)
            {
               /* Copy all characters from the last space to the position */
               last = uni;
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
               uni = 0;
               strLine[uni] = uniStr[i];
               uni++;
               strLine[uni] = 0;
            }
            lastSpace = -1;
         }
      }
   }
   /* If last character is different of |, theres a remanescent line */
   if(uni > 0)
   {
      lines++;
   }
   return(lines);
}

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

/***********************************************************************
 *                             copyLines                               *
 ***********************************************************************/
string dntFont::copyLines(string source, int firstLine, int lastLine, 
                          int x1, int x2)
{
   string result = "";
   int line = 0;
   int aux, k, w = 0;
   int maxWidth = x2 - x1;
   int uni = 0;
   int last = -1, lastSpace = -1;
   strLine[0] = 0;

   Uint16* unicodeText;

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

   for(aux=0;( (aux < (int)source.length()) && (line <= lastLine));aux++)
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
               //result += " ";
               if(line >= firstLine)
               {
                  result += unicodeToString(strLine, lastSpace);
               }
            }
            else
            {
               /* Ignore the last character */
               strLine[uni] = 0;
               if(line >= firstLine)
               {
                  result += unicodeToString(strLine, uni);
               }
            }
            line++;

            /* Put the character */
            if(lastSpace != -1)
            {
               /* Copy all characters from the last space to the position */
               uni = 1;
               strLine[0] = ' ';
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
         }
         
      }
      else
      {
         TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
         /* | breaks a line */

         if(line >= firstLine)
         {
            result += unicodeToString(strLine, uni) + '|';
         }
         line++;
         uni = 0;
         strLine[uni] = 0;
      }
   }

   if( (line >= firstLine) && (line <= lastLine) && (uni != 0))
   {
      TTF_SizeUNICODE(activeFont->font, strLine, &w, NULL);
      result += unicodeToString(strLine, uni);
   }

   return(result);
}

/* Static Variables */
loadedFont*  dntFont::activeFont;
loadedFont*  dntFont::fontsList;
int          dntFont::totalFonts;
int          dntFont::activeFontAlign;
int          dntFont::activeFontStyle;

