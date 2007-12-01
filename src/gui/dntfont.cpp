#include "dntfont.h"
#include "draw.h"

/**********************************************************************
 *                               init                                 *
 **********************************************************************/
void dntFont::init()
{
   /* Init SDL_ttf */
    if(TTF_Init() == -1)
    {
       printf("Can't init SDL_ttf : %s\n", TTF_GetError());
       exit(3);
    }
    activeFont = NULL;
    activeFontName = "";
    activeFontSize = 0;
    activeFontAlign = DNT_FONT_ALIGN_LEFT;
    defineFont(DNT_FONT_ARIAL,12);
}

/**********************************************************************
 *                                end                                 *
 **********************************************************************/
void dntFont::end()
{
   /* Close the active Font, if is opened */
   if(activeFont)
   {
      TTF_CloseFont(activeFont);
   }
   /* Quit SDL_ttf */
   TTF_Quit();
}

/**********************************************************************
 *                            defineFont                              *
 **********************************************************************/
bool dntFont::defineFont(string fileName, int size)
{
   if( (activeFontName != fileName) || (size != activeFontSize) )
   {
      if(activeFont)
      {
         TTF_CloseFont(activeFont);
      }
      activeFont = TTF_OpenFont(fileName.c_str(), size);
      if(activeFont)
      {
         activeFontName = fileName;
         activeFontSize = size;
         return(true);
      }
      else
      {
         activeFontName = "";
         activeFontSize = 0;
         printf("Can't open font file: %s\n", fileName.c_str());
         return(false);
      }
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

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int dntFont::write(SDL_Surface *screen,int x,int y,string text,int init,
                   int end, int x1,int y1,int x2,int y2)
{
   int aux, curY, w = 0, h = 0;
   int maxWidth = x2 - x1;
   int uni = 0;
   strLine[0] = 0;

   Uint16* unicodeText;

   SDL_Color color;
   SDL_Rect rect;
   SDL_Surface* writeSurface = NULL;

   /* Verify if avaible */
   if(!activeFont)
   {
      return(-1);
   }

   /* Convert to unicode */
   //FIXME -> if already on unicode, don't need to do this!
   convertToUnicode(curUnicode, text.c_str(), text.length());
   unicodeText = curUnicode;

   /* Verify if is already on unicode */
   if( (text[0] == '\\') )
   {
      text.erase(0,1);
      unicodeText = (Uint16*)text.c_str();
   }

   /* Verify Alignment */
   if(activeFontAlign == DNT_FONT_ALIGN_CENTER)
   {
      TTF_SizeUNICODE(activeFont, unicodeText, &w, &h);
      x = ((x2 + x1) / 2) - (w / 2)-1;
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
         strLine[uni] = unicodeText[aux];
         strLine[uni+1] = 0;
         uni++;
         TTF_SizeUNICODE(activeFont, strLine, &w, &h);
         if(w >= maxWidth)
         {
            /* So, if the width is bigger, write the string without 
             * the character */
            uni--;
            strLine[uni] = 0;
            writeSurface = TTF_RenderUNICODE_Blended(activeFont, strLine, 
                                                  color);

            /* Put the character */
            uni = 0;
            strLine[uni] = unicodeText[aux];
            uni++;
            strLine[uni] = 0;

            /* Blit the result surface to the desired one on the desired 
             * position  */
            rect.y = curY;
            SDL_BlitSurface(writeSurface, NULL, screen, &rect);

            /* Avoid memory leacks */
            SDL_FreeSurface(writeSurface);

            curY += h;
         }
         
      }
      else
      {
         TTF_SizeUNICODE(activeFont, strLine, &w, &h);
         /* | breaks a line */
         writeSurface = TTF_RenderUNICODE_Blended(activeFont, strLine,
                                                  color);

         /* Blit the result surface to the desired one on the desired
          * position  */
         uni = 0;
         strLine[uni] = 0;
         rect.y = curY;
         SDL_BlitSurface(writeSurface, NULL, screen, &rect);

         /* Avoid memory leacks */
         SDL_FreeSurface(writeSurface);

         curY += h;
      }
   }

   if(uni != 0)
   {
      TTF_SizeUNICODE(activeFont, strLine, &w, &h);
      /* Remaining things to write */
      writeSurface = TTF_RenderUNICODE_Blended(activeFont, strLine, color);

      /* Blit the result surface to the desired one on the desired position  */
      rect.y = curY;
      SDL_BlitSurface(writeSurface, NULL, screen, &rect);

      /* Avoid memory leacks */
      SDL_FreeSurface(writeSurface);
   }

   return(curY);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
void dntFont::write(SDL_Surface *screen,int x,int y,string text)
{
   write(screen,x,y,text,0,text.length()-1,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int dntFont::write(SDL_Surface *screen,int x, int y,string text,int x1,int y1,
                   int x2,int y2)
{
   return(write(screen,x,y,text,0,text.length()-1,x1,y1,x2,y2));
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
void dntFont::write(SDL_Surface *screen,int x,int y,string text,
                    int init,int end)
{
   write(screen,x,y,text,init,end,0,0,screen->w-1,screen->h-1);
}

/***********************************************************************
 *                            writeUnicode                             *
 ***********************************************************************/
void dntFont::writeUnicode(SDL_Surface* screen, int x, int y, string text)
{
   SDL_Color color;
   SDL_Rect rect;
   SDL_Surface* writeSurface;

   /* Get Color */
   color_Get(&color.r,&color.g, &color.b);

   /* Write Unicode Text */
   writeSurface = TTF_RenderUNICODE_Blended(activeFont, (Uint16*)text.c_str(), 
                                            color);

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
void dntFont::convertToUnicode(Uint16 *unicode, const char *text, int len)
{
   int i;
   for( i=0; i < len; ++i ) 
   {
      unicode[i] = ((const unsigned char *)text)[i];
   }
   unicode[i] = 0;
}

/***********************************************************************
 *                              getincCP                               *
 ***********************************************************************/
int dntFont::getIncCP()
{
   int adv = 0;
   if(activeFont)
   {
      TTF_GlyphMetrics(activeFont, 'm', NULL, NULL, NULL, NULL, &adv);
      return(adv);
   }
   return(0);
}

/***********************************************************************
 *                           getTotalLines                             *
 ***********************************************************************/
int dntFont::getTotalLines(string source)
{
   int i;
   char c = 0;
   int line = -1;
   for(i=0; (i < (int)source.length()) ; i++)
   {
      c = source.at(i);
      if(c == '|')
      {
         line++;
      }
   }
   /* If last character is different of |, theres a remanescent line */
   if(c != '|')
   {
      line++;
   }
   return(line);
}

/***********************************************************************
 *                             copyLines                               *
 ***********************************************************************/
string dntFont::copyLines(string source, int firstLine, int lastLine)
{
   string result = "";
   char c;
   Uint32 i;
   int line = 0;
   /* Positionate the string to the first desired line */
   for(i=0; ( (i < source.length()) && (line < firstLine)) ; i++)
   {
      c = source.at(i);
      if(c == '|')
      {
         line++;
      }
   }
   
   /* Copy to the desired line (or end of the string, witch occurs first) */
   for(; ( (i<source.length()) && (line <= lastLine)); i++)
   {
      c = source.at(i);
      result += c;
      if(c == '|')
      {
         line++;
      }
   }

   return(result);
}

/* Static Variables */
TTF_Font*  dntFont::activeFont;
string     dntFont::activeFontName;
int        dntFont::activeFontSize;
int        dntFont::activeFontAlign;

