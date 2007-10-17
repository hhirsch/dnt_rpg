#include "grasswindow.h"

/**********************************************************
 *                      constructor                       *
 **********************************************************/
grassWindow::grassWindow(interface* actualGui)
{
   actualWindow = NULL;
   actualGrass = NULL;
   gui = actualGui;
   lastTime = 0;
}

/**********************************************************
 *                       destructor                       *
 **********************************************************/
grassWindow::~grassWindow()
{
}

/**********************************************************
 *                        setGrass                        *
 **********************************************************/
void grassWindow::setGrass(grass* gr)
{
   actualGrass = gr;
   if(actualWindow == NULL)
   {
      openWindow();
   }
   defineValues();
}

/**********************************************************
 *                      openWindow                        *
 **********************************************************/
void grassWindow::openWindow()
{
   actualWindow = gui->insertWindow(184,599-345,403,599-248,"Grass");
   
   actualWindow->getObjectsList()->insertTextBox(7,23,87,40,0,"Povoation:");
   buttonPovoationMore = actualWindow->getObjectsList()->insertButton(88,23,
                                                                      98,40,
                                                                      "<",0);
   textPovoation = actualWindow->getObjectsList()->insertTextBox(99,23,150,40,
                                                                 1,"20.0");
   buttonPovoationLess = actualWindow->getObjectsList()->insertButton(151,23,
                                                                      161,40,
                                                                      ">",0);
   actualWindow->getObjectsList()->insertTextBox(7,43,87,60,0,"ScaleFactor:");
   buttonSizeLess = actualWindow->getObjectsList()->insertButton(88,43,98,60,
                                                                 "<",0);
   textSize = actualWindow->getObjectsList()->insertTextBox(99,43,150,60,
                                                            1,"1.0");
   buttonSizeMore = actualWindow->getObjectsList()->insertButton(151,43,161,60,
                                                                 ">",0);

   /* Open Window */
   actualWindow->setExternPointer(&actualWindow);
   actualWindow->setAttributes(false,true,false,false);
   gui->openWindow(actualWindow);
}

/**********************************************************
 *                     defineValues                       *
 **********************************************************/
void grassWindow::defineValues()
{
   char buf[64];
   GLfloat pX1=0, pX2=0, pZ1=0, pZ2=0;
   float povValue;

   if(actualWindow)
   {
      /* Write povoation */
      actualGrass->getPosition(pX1,pZ1,pX2,pZ2);
      povValue = ((pZ2-pZ1)*(pX2-pX1) / (float)actualGrass->getMaxParticles());
      sprintf(buf,"%.2f",povValue);
      textPovoation->setText(buf);

      /* Write Size */
      sprintf(buf,"%.2f",actualGrass->getScaleFactor());
      textSize->setText(buf);
      actualWindow->draw(0,0);
   }
}

/**********************************************************
 *                      drawTemporary                     *
 **********************************************************/
void grassWindow::drawTemporary()
{
   //TODO
}

/**********************************************************
 *                       eventGot                         *
 **********************************************************/
bool grassWindow::eventGot(int type, guiObject* object)
{
   if(!actualWindow)
   {
      return(false);
   }
   float size;
   float pov;
   bool treated = false;
   int actualTime = SDL_GetTicks();
   if( (type == ON_PRESS_BUTTON) && 
       (actualTime - lastTime > 100) )
   {  
      lastTime = actualTime;
      if(object == (guiObject*)buttonSizeMore)
      {
         /* Inc the size of actual grass */
         if(actualGrass)
         {
            size = actualGrass->getScaleFactor();
            if(size <= 9.9)
            {
               actualGrass->setScaleFactor(size+0.1);
            }
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonSizeLess)
      {
         /* Dec the size of actual grass */
         if(actualGrass)
         {
            size = actualGrass->getScaleFactor();
            if(size >= 0.2)
            {
               actualGrass->setScaleFactor(size-0.1);
            }
         }
         treated = true;
      }
      if(object == (guiObject*)buttonPovoationMore)
      {
         /* Inc the povoation of actual grass */
         if(actualGrass)
         {
         }
         treated = true;
      }
      else if(object == (guiObject*)buttonPovoationLess)
      {
      }

      /* Actualize Window, if needed */
      if(treated)
      {
         defineValues();
      }
   }

   return(treated);
}

