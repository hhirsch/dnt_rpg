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
   dirs dir;
   /* Set open position */
   int posY = 1;
   int posX = 0;

   //char buf[256];

   /* Close the window, if it is openned */
   if(isOpen())
   {
      posX = intWindow->getX1();
      posY = intWindow->getY1();
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(posX, posY, posX+288, posY+330,
                                   pers->name);

   /* Character Image */
   picture* fig;
   fig = intWindow->getObjectsList()->insertPicture(8,18,
                                                    0,0,
                   dir.getRealFile(pers->getPortraitFileName()).c_str());
   intWindow->getObjectsList()->insertTextBox(5,15,65,75,1,"");

   /* Character Classes */


   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,188,158,207,
                                                        gettext("Ok"),1);                                   
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


