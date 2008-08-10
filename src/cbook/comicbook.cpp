#include "comicbook.h"

#include "../engine/util.h"
#include "../etc/defparser.h" 
#include "../etc/dirs.h"
#include "../gui/draw.h"
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
   pages = NULL;
   totalPages = 0;
   skipScale = 1.0;
   skipSum = -0.05;

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
   int i;
   comicPage* pag;
   /* Empty the book, deleting each page */
   for(i = 0; i < totalPages; i++)
   {
      pag = pages;
      pages = pages->getNext();
      delete(pag);
   }
}

/***********************************************************************
 *                             insertPage                              *
 ***********************************************************************/
comicPage* comicBook::insertPage(string t)
{
   comicPage* page = new comicPage();
   if(pages == NULL)
   {
      /* Is the first page */
      pages = page;
      page->setNext(page);
      page->setPrevious(page);
   }
   else
   {
      /* Insert at linked list end */
      page->setNext(pages);
      page->setPrevious(pages->getPrevious());

      page->getNext()->setPrevious(page);
      page->getPrevious()->setNext(page);
   }

   totalPages++;
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
         sscanf(value.c_str(),"%d,%d,%d,%d", &posX1, &posY1, &posX2, &posY2);
      }
      /* Insert the text at the textbox at the comic box */
      else if(key == "text")
      {
         value = translateDataString(value);
         if(curPage)
         {
            curPage->insertText(posX1,posY1,posX2,posY2,value);
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
   comicPage *curPage = pages;

   /* Treat all book pages */
   for(p = 0; p < totalPages; p++)
   {
      int b;
      comicBox* curBox = curPage->getFirstBox();
      for(b = 0; b < curPage->getTotalBoxes(); b++)
      {
         /* Activate the box */
         curBox->activate();

         /* Wait it turn inactive */
         while(curBox->getStatus() != COMIC_BOX_STATUS_DONE)
         {
            render(curPage);
         }
         curBox = curBox->getNext();
      }

      /* Render the page with a scale-out effect */
      float scale = 1.0;
      while(scale > 0)
      {
         render(curPage, scale); 
         scale -= 0.04;
      }

      curPage = curPage->getNext();
   }
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void comicBook::render(comicPage* curPage, float scale)
{
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | 
           GL_STENCIL_BUFFER_BIT);

   draw2DMode();
         
   /* Render Page */
   glPushMatrix();
      glScalef(scale,scale,scale);
      curPage->render();
   glPopMatrix();


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

   draw3DMode(OUTDOOR_FARVIEW);

   glFlush();
   SDL_GL_SwapBuffers();

   SDL_Delay(30);
}

/***********************************************************************
 *                              verifyInput                            *
 ***********************************************************************/


