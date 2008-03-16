#include "charwindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
charWindow::charWindow(interface* interf)
{
   inter = interf;
   intWindow = NULL;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
charWindow::~charWindow()
{
   close();
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void charWindow::open(character* pers)
{
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   //FIXME
   fig = NULL;

   //char buf[256];

   /* Close the window, if it is openned */
   if(isOpen())
   {
      centerX = intWindow->getX1()+128;
      centerY = intWindow->getY1()+108;
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-128,centerY-108,
                                   centerX+128,centerY+108,
                                   pers->name);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void charWindow::close()
{
   if(intWindow)
   {
      if(fig)
      {
         /* To avoid delete the item image */
         fig->set(NULL);
      }
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/********************************************************************
 *                             isOpen                               *
 ********************************************************************/
bool charWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int charWindow::treat(guiObject* object, int eventInfo)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if(object == (guiObject*) okButton)
      {
         close();
         return(1);
      }
   }
   return(0);
}


