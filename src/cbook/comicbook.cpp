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

#include "comicbook.h"

#include "../engine/util.h"
#include "../engine/cursor.h"
#include "../etc/defparser.h" 
#include "../etc/dirs.h"
#include "../gui/draw.h"
#include "../gui/mouse.h"
#include "../lang/translate.h"

#include <iostream>
using namespace std;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBook::comicBook()
{
   dirs dir;
   /* Default Values */
   title = "";
   skipScale = 1.0;
   skipSum = -0.05;
   exit = false;
   changeColor = false;

   /* Create the Skip Texture */
   glGenTextures(1, &skipTexture);
   SDL_Surface* img;
   img = IMG_Load(dir.getRealFile("texturas/cbook/skip.png").c_str());
   if(img)
   {
      setTexture(img, skipTexture);
      SDL_FreeSurface(img);
   }
   else
   {
      cerr << "Can't load skip image 'texturas/cbook/skip.png'" << endl;
   }
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
comicBook::~comicBook()
{
   empty();
   glDeleteTextures(1, &skipTexture);
}

/***********************************************************************
 *                               empty                                 *
 ***********************************************************************/
void comicBook::empty()
{
   pages.clearList();
}

/***********************************************************************
 *                             insertPage                              *
 ***********************************************************************/
comicPage* comicBook::insertPage(string t)
{
   comicPage* page = new comicPage();
   pages.insert(page);
   return(page);
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool comicBook::load(string bookFile)
{
   defParser parser;
   string key="", value="";
   comicPage *curPage = NULL;
   comicBox* curBox = NULL;
   int curVertex = 0;
   int posX1=0, posY1=0, posX2=0, posY2=0;
   int textColor[3];
   int textSize=16, textStyle = DNT_FONT_STYLE_NORMAL;

   /* First, empty all previous book pages */
   empty();

   /*! Parse the definitions file */
   if(!parser.load(bookFile))
   {
      return(false);
   }

   while(parser.getNextTuple(key, value))
   {
      /* Book Name Definition */
      if(key == "book")
      {
         title = translateDataString(value);
         curPage = NULL;
      }
      /* Create a Page */
      else if(key == "page")
      {
         /* Load OpenGL texture for previous page */
         if(curPage)
         {
            curPage->flushTexture();
         }

         /* Insert the new page at the book */
         curPage = insertPage(value);
         curBox = NULL;
      }
      /* Define Page's Texture */
      else if(key == "texture")
      {
         if(curPage)
         {
            curPage->defineTexture(value);
         }
         else
         {
            cerr << "Warning: texture '"<< value 
                 << "' without a page for book: '" << bookFile << "'" << endl;
         }
      }
      /* Insert Comic Box */
      else if(key == "box")
      {
         curVertex = 0;
         if(curPage)
         {
            curBox = new comicBox(value);
            curPage->insertBox(curBox);
         }
         else
         {
            cerr << "Warning: box '"<< value 
                 << "' without a page for book: '" << bookFile << "'" << endl;
         }
      }
      /* Define Comic Box Vertex */
      else if(key == "vertex")
      {
         if(curBox)
         {
            GLfloat x,y;
            sscanf(value.c_str(), "%f,%f", &x,&y);
            curBox->setVertex(curVertex, x, y, 
                              curPage->getWidth(), curPage->getHeight());
            curVertex++;
         }
         else
         {
            cerr << "Warning: vertex '" << value 
                 << "' without a box for book: '" << bookFile << "'" << endl;
         }
      }
      /* Define Comic Box Effect */
      else if(key == "effect")
      {
         if(curBox)
         {
            curBox->setEffect(atoi(value.c_str()));
         }
         else
         {
            cerr << "Warning: effect '" << value 
                 << "' without a box for book: '" << bookFile << "'" << endl;
         }
      }
      /* Define Comic Box WaitTime */
      else if(key == "timeout")
      {
         if(curBox)
         {
            curBox->setTimeout(atoi(value.c_str()));
         }
         else
         {
            cerr << "Warning: timeout '" << value 
                 << "' without a box for book: '" << bookFile << "'" << endl;
         }
      }
      /* Insert a text box at the comic Box */
      else if(key == "textbox")
      {
         /* Define the text Box position */
         sscanf(value.c_str(),"%d,%d,%d,%d", &posX1, &posY1, &posX2, &posY2);
         /* Default as black color */
         textColor[0] = 0;
         textColor[1] = 0;
         textColor[2] = 0;
         /* Default as normal style and 16 size */
         textStyle = DNT_FONT_STYLE_NORMAL;
         textSize = 16;
      }
      /* Define the text color */
      else if(key == "textcolor")
      {
         sscanf(value.c_str(),"%d,%d,%d", &textColor[0], &textColor[1], 
                                          &textColor[2]);
      }
      /* Define the text font size */
      else if(key == "textsize")
      {
         sscanf(value.c_str(),"%d",&textSize);
      }
      /* Define the text font style */
      else if(key == "textstyle")
      {
         sscanf(value.c_str(),"%d",&textStyle);
      }
      /* Insert the text at the textbox at the comic box */
      else if(key == "text")
      {
         value = translateDataString(value);
         if(curPage)
         {
            curPage->insertText(posX1,posY1,posX2,posY2,value,
                                textSize, textStyle, textColor[0],
                                textColor[1], textColor[2]);
         }
         else
         {
            cerr << "Warning: text '"<< value 
                 << "' without a page for book: '" << bookFile << "'" << endl;
         }
      }
      /* Unknow Key */
      else
      {
         cerr << "Unknow key: '" << key << "' at book: '" 
              << bookFile << "'" << endl;
      }
   }

   /* Flush OpenGL texture */
   if(curPage)
   {
      curPage->flushTexture();
   }

   return(true);
};

/***********************************************************************
 *                                 run                                 *
 ***********************************************************************/
void comicBook::run()
{
   int p;
   comicPage* curPage = (comicPage*)pages.getFirst();
   exit = false;

   /* Treat all book pages */
   for(p = 0; ((p < pages.getTotal()) && (!exit)); p++)
   {
      int b;
      comicBox* curBox = curPage->getFirstBox();
      for(b = 0; ( (b < curPage->getTotalBoxes()) && (!exit) ); b++)
      {
         /* Activate the box */
         curBox->activate();

         /* Wait it turn inactive */
         while( (curBox->getStatus() != COMIC_BOX_STATUS_DONE) && (!exit))
         {
            render(curPage);
            if(verifyInput())
            {
               /* Skip Current Box */
               curBox->skip();
               /* Keep Redering it until the mouse button is released */
               while(mButton & SDL_BUTTON(1))
               {
                  render(curPage);
               }
            }
         }
         curBox = (comicBox*)curBox->getNext();
      }

      /* Render the page with a scale-out effect */
      float scale = 1.0;
      while(scale > 0)
      {
         render(curPage, scale); 
         scale -= 0.04;
      }

      curPage = (comicPage*)curPage->getNext();
   }
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void comicBook::render(comicPage* curPage, float scale)
{
   /* Gather Keyboard and Mouse status */
   SDL_PumpEvents();
   keys = SDL_GetKeyState(NULL);
   mButton = SDL_GetMouseState(&mouseX, &mouseY);

   /* Clear screen */
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | 
           GL_STENCIL_BUFFER_BIT);

   draw2DMode();
   
   glColor3f(1.0,1.0,1.0);

   /* Render Page */
   glPushMatrix();
      glScalef(scale,scale,scale);
      curPage->render();
   glPopMatrix();

   /* Change Skip Color, if needed */
   if(changeColor)
   {
      glColor3f(1.0,0.0,0.0);
   }

   /* Render Skip Texture */
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, skipTexture);

   glPushMatrix();
      glTranslatef(SCREEN_X-23, 23, 0);
      glScalef(skipScale, skipScale, skipScale);
      glBegin(GL_QUADS);
         glTexCoord2f(0, 0);
         glVertex2f(-16, -16);
         glTexCoord2f(1, 0);
         glVertex2f(16, -16);
         glTexCoord2f(1, 1);
         glVertex2f(16, 16);
         glTexCoord2f(0, 1);
         glVertex2f(-16, 16);
      glEnd();
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   glColor3f(1.0,1.0,1.0);

   /* Change Skip Button scale */
   skipScale = skipScale + skipSum;
   if(skipScale > 1.0)
   {
      skipScale = 1.0;
      skipSum *= -1;
   }
   else if(skipScale < 0.5)
   {
      skipScale = 0.5;
      skipSum *= -1;
   }

   /* Render Mouse cursor */
   glPushMatrix();
     cursor cursors;
     cursors.draw(mouseX, mouseY);
   glPopMatrix();

   /* Back to 3D draw mode */
   draw3DMode(OUTDOOR_FARVIEW);

   /* Flush it to screen! */
   glFlush();
   SDL_GL_SwapBuffers();

   SDL_Delay(30);
}

/***********************************************************************
 *                              verifyInput                            *
 ***********************************************************************/
bool comicBook::verifyInput()
{
   /* Skip Comic Book if Esc key is pressed */
   if(keys[SDLK_ESCAPE])
   {
      exit = true;
   }

   /* If Skip Texture Pressed, skip comic book too */
   if(isMouseAt(SCREEN_X-55, SCREEN_Y-55, SCREEN_X, SCREEN_Y, mouseX, mouseY)) 
   {
      changeColor = true;
      if(mButton & SDL_BUTTON(1))
      {
         exit = true;
      }
   }
   else
   {
      changeColor = false;
      /* Skip ComicBox if Left Mouse Button is pressed */
      if(mButton & SDL_BUTTON(1))
      {
         return(true);
      }
   }

   return(false);
}

