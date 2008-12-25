#include "comicpage.h"
#include "../etc/dirs.h"
#include "../gui/dntfont.h"
#include "../gui/draw.h"

#include <iostream>
#include <SDL/SDL_image.h>
using namespace std;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
comicPage::comicPage()
{
   boxes = NULL; 
   totalBoxes = 0;
   next = NULL;
   previous = NULL;
   texture = NULL;

   glGenTextures(1, &tex);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
comicPage::~comicPage()
{
   int i;

   /* Free texture */
   glDeleteTextures(1,&tex);
   if(texture != NULL)
   {
      SDL_FreeSurface(texture);
   }

   /* Free All Boxes */
   comicBox* box;
   for(i = 0; i < totalBoxes; i++)
   {
      box = boxes;
      boxes = boxes->getNext();

      delete(box);
   }
}

/***********************************************************************
 *                            flushTexture                             *
 ***********************************************************************/
void comicPage::flushTexture()
{
   if(texture != NULL)
   {
      setTexture(texture, tex);
   }
}

/***********************************************************************
 *                             getFirstBox                             *
 ***********************************************************************/
comicBox* comicPage::getFirstBox()
{
   return(boxes);
}

/***********************************************************************
 *                            getTotalBoxes                            *
 ***********************************************************************/
int comicPage::getTotalBoxes()
{
   return(totalBoxes);
}

/***********************************************************************
 *                               setNext                               *
 ***********************************************************************/
void comicPage::setNext(comicPage* page)
{
   next = page;
}

/***********************************************************************
 *                               getNext                               *
 ***********************************************************************/
comicPage* comicPage::getNext()
{
   return(next);
}

/***********************************************************************
 *                             setPrevious                             *
 ***********************************************************************/
void comicPage::setPrevious(comicPage* page)
{
   previous = page;
}

/***********************************************************************
 *                             getPrevious                             *
 ***********************************************************************/
comicPage* comicPage::getPrevious()
{
   return(previous);
}

/***********************************************************************
 *                               getWidth                              *
 ***********************************************************************/
int comicPage::getWidth()
{
   if(texture)
   {
      return(texture->w);
   }
   return(0);
}

/***********************************************************************
 *                               getHeight                             *
 ***********************************************************************/
int comicPage::getHeight()
{
   if(texture)
   {
      return(texture->h);
   }
   return(0);
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void comicPage::render()
{
   int i;
   comicBox *box = boxes;

   /* Calculate scale ratio */
   GLfloat ratio = 1.0;
   if(texture)
   { 
      ratio = (float)SCREEN_Y / (float)texture->h;
   }

   glColor4f(1.0,1.0,1.0,1.0);

   glPushMatrix();
   glDisable(GL_DEPTH_TEST);

   /* Center the Page on screen */
   if(texture)
   {
      glTranslatef(((SCREEN_X/2.0) - ratio*(texture->w/2.0)),0.0,0.0);

      /* Render Blank Page */
      GLfloat midX = ratio * texture->w;
      GLfloat midY = ratio * texture->h;
      glBegin(GL_QUADS);
         glVertex2f(0, 0);
         glVertex2f(midX, 0);
         glVertex2f(midX, midY);
         glVertex2f(0, midY);
      glEnd();

   }

   /* Render Page Boxes */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, tex);
   for(i = 0; i < totalBoxes; i++)
   {
      if(box->getStatus() != COMIC_BOX_STATUS_INACTIVE)
      {
         box->render();
      }
      box = box->getNext();
   }
   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
}

/***********************************************************************
 *                              insertBox                              *
 ***********************************************************************/
void comicPage::insertBox(comicBox* box)
{
   if(box)
   {
      if(boxes == NULL)
      {
         /* First box on the list */
         box->setNext(box);
         box->setPrevious(box);
         boxes = box;
      }
      else
      {
         /* Put it at the list's end */
         box->setNext(boxes);
         box->setPrevious(boxes->getPrevious());

         box->getNext()->setPrevious(box);
         box->getPrevious()->setNext(box);
      }
      totalBoxes++;
   }
}


/***********************************************************************
 *                             insertText                              *
 ***********************************************************************/
void comicPage::insertText(int x1, int y1, int x2, int y2, string text)
{
   insertText(x1, y1, x2, y2, text, 16, DNT_FONT_STYLE_NORMAL, 0, 0, 0);
}

/***********************************************************************
 *                             insertText                              *
 ***********************************************************************/
void comicPage::insertText(int x1, int y1, int x2, int y2, string text,
                           int fontSize, int fontStyle, 
                           int R, int G, int B)
{
   if(texture != NULL)
   {
      dntFont fnt;
      fnt.defineFont(DNT_FONT_TIMES, fontSize);
      fnt.defineFontAlign(DNT_FONT_ALIGN_CENTER);
      fnt.defineFontStyle(fontStyle);

      color_Set(R, G, B, 255);
      fnt.write(texture, x1+1,y1+2, text, x1,y1,x2,y2);

      fnt.defineFontStyle(DNT_FONT_STYLE_NORMAL);
   }
   else
   {
      cerr << "Warning: tried to define comic text '" << text 
           << "' without texture!" << endl;
   }
}


/***********************************************************************
 *                          defineTexture                              *
 ***********************************************************************/
bool comicPage::defineTexture(string textureFile)
{
   dirs dir;
   if(texture != NULL)
   {
      SDL_FreeSurface(texture);
   }

   texture = IMG_Load(dir.getRealFile(textureFile).c_str());

   if(!texture)
   {
      cerr << "Error on loading image: '" << textureFile << "'" << endl;
      return(false);
   }

   return(true);
}

