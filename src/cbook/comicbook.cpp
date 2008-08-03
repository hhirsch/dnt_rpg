#include "comicbook.h"

#include "../engine/util.h"
#include "../etc/defparser.h" 
#include "../lang/translate.h"

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
comicBook::comicBook()
{
   title = "";
   pages = NULL;
   totalPages = 0;
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
comicBook::~comicBook()
{
   empty();
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
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | 
                    GL_STENCIL_BUFFER_BIT);

            draw2DMode();
            curPage->render();
            draw3DMode(OUTDOOR_FARVIEW);
            glFlush();
            SDL_GL_SwapBuffers();

            SDL_Delay(30);
         }
         curBox = curBox->getNext();
      }
      curPage = curPage->getNext();
   }
}

/***********************************************************************
 *                              verifyInput                            *
 ***********************************************************************/

/***********************************************************************
 *                                 draw                                *
 ***********************************************************************/

