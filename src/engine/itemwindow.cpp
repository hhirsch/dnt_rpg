
#include "itemwindow.h"


/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
itemWindow::itemWindow(interface* interf)
{
   inter = interf;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
itemWindow::~itemWindow()
{
   if(fig)
   {
      /* To avoid delete the item image */
      fig->set(NULL);
   }
   //TODO
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void itemWindow::open(object* item)
{
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   /* Close the window, if it is openned */
   if(isOpen())
   {
      centerX = intWindow->getX1();
      centerY = intWindow->getY1();
      close();
   }

   /* Create Window */
   intWindow = inter->insertWindow(centerX-128,centerY-128,
                                   centerX+128,centerY+128,
                                   item->getName());

   /***********************
    * Common Object Things 
    ***********************/
   
   /* Cost */

   /* Weight */


   //TODO

   /* Item Image */
   fig = intWindow->getObjectsList()->insertPicture(8,15,0,0,NULL);
   fig->set(item->get2dModel());

   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(88,228,158,247,
                                                        gettext("Ok"),1);


   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void itemWindow::close()
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
bool itemWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int itemWindow::treat(guiObject* object, int eventInfo)
{
   //TODO
   if(eventInfo == PRESSED_BUTTON)
   {
      if(object == (guiObject*) okButton)
      {
         fig->set(NULL); //to not delete item image
         inter->closeWindow(intWindow);
         intWindow = NULL;
         return(1);
      }
   }
   return(0);
}

