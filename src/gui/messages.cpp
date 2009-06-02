#include "messages.h"

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
//
//                            quantityWindow
//
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                           setRange                                  *
 ***********************************************************************/
void quantityWindow::setRange(float min, float max)
{
   quantityWindow::min = min;
   quantityWindow::max = max;
}

/***********************************************************************
 *                           getValue                                  *
 ***********************************************************************/
float quantityWindow::getValue()
{
   return(qty);
}

/***********************************************************************
 *                           setValue                                  *
 ***********************************************************************/
void quantityWindow::setValue(float val)
{
   qty = val;
}

/***********************************************************************
 *                           setDelta                                  *
 ***********************************************************************/
void quantityWindow::setDelta(float val)
{
   delta = val;
}

/***********************************************************************
 *                           updateText                                *
 ***********************************************************************/
void quantityWindow::updateText()
{
   char buf[16];

   sprintf(buf, "%.2f", qty);
   qtyText->setText(buf);
}

/***********************************************************************
 *                              show                                   *
 ***********************************************************************/
void quantityWindow::show(string title, string message, guiInterface* gui)
{
   dntFont fnt;

   /* Close the window if it is opened */
   close();

   /* Define the gui */
   lastGui = gui;

   /* Calculate Window Position And size */
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   int sizeX = fnt.getStringWidth(message);
   sizeX = (sizeX > 88)?sizeX:88;
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;
   int med = sizeX / 2;
   guiList* list;

   /* Create the window */
   qtyWindow = gui->insertWindow(sX-med-10,sY-46,sX+med+10,sY+46,title);
   list = qtyWindow->getObjectsList();
   
   /* Ok Button */
   okButton = list->insertButton(med-28+10,57,med+28+10,75,"Ok",1);
   
   /* Message */
   textBox* quadText = list->insertTextBox(5,17,sizeX+10,33,0,message);
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);

   /* Quantity */
   int posY = 37;
   med += 10;
   lessButton = list->insertButton(med-39,posY,med-29,posY+17,
                                   fnt.createUnicode(0x25C4),0);
   lessButton->defineFont(DNT_FONT_ARIAL, 9);
   qtyText = list->insertTextBox(med-28,posY,med+28,posY+17,1,"");
   qtyText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   moreButton = list->insertButton(med+29,posY,med+39,posY+17,
                                   fnt.createUnicode(0x25BA),0);
   moreButton->defineFont(DNT_FONT_ARIAL, 9);

   /* Set the window and open */
   qtyWindow->setModal();
   qtyWindow->setExternPointer(&qtyWindow);
   gui->openWindow(qtyWindow);

   /* Do the text update */
   updateText();
}

/***********************************************************************
 *                          verifyEvents                               *
 ***********************************************************************/
bool quantityWindow::verifyEvents(guiObject* actObj, int eventInfo)
{
   if(isOpened())
   {
      if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
      {
         if(actObj == okButton)
         {
            /* Done with the get, so close the window */
            close();
            return(true);
         }
      }
      else if(eventInfo == FARSO_EVENT_ON_PRESS_BUTTON)
      {
         if(actObj == moreButton)
         {
            /* Inc the value */
            if(qty + delta < max)
            {
               qty += delta;
            }
            else
            {
               qty = max;
            }
            updateText();
            return(true);
         }
         else if(actObj == lessButton)
         {
            /* Dec the value */
            if(qty - delta > min)
            {
               qty -= delta;
            }
            else
            {
               qty = min;
            }
            updateText();
            return(true);
        }
      }
   }
   return(false);
}

/***********************************************************************
 *                             isOpened                                *
 ***********************************************************************/
bool quantityWindow::isOpened()
{
   return(qtyWindow != NULL);
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void quantityWindow::close()
{
   if(qtyWindow != NULL)
   {
      lastGui->closeWindow(qtyWindow);
   }
}

/***********************************************************************
 *                           static members                            *
 ***********************************************************************/
window* quantityWindow::qtyWindow = NULL;
guiInterface* quantityWindow::lastGui = NULL;
textBox* quantityWindow::qtyText = NULL;
button* quantityWindow::moreButton = NULL;
button* quantityWindow::lessButton = NULL;
button* quantityWindow::okButton = NULL;
float quantityWindow::qty = 0.0f;
float quantityWindow::delta = 1.0f;
float quantityWindow::min = 0.0f;
float quantityWindow::max = 10.0f;

///////////////////////////////////////////////////////////////////////////
//
//                                warning
//
//////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                               Show                                  *
 ***********************************************************************/
void warning::show(string title, string message, guiInterface* gui)
{ 
   dntFont fnt;

   /* Close the window if it is opened */
   close();

   /* Define the gui */
   lastGui = gui;

   /* Calculate Window Position And size */
   fnt.defineFont(DNT_FONT_ARIAL, 10);
   int sizeX = fnt.getStringWidth(message);
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;
   int med = sizeX / 2;

   warnWindow = gui->insertWindow(sX-med-10,sY-31,sX+med+10,sY+31,title);
   okButton = warnWindow->getObjectsList()->insertButton(med-28+10,37,
                                                         med+28+10,55,
                                                         "Ok",1);
   textBox* quadText = warnWindow->getObjectsList()->insertTextBox(5,17,
                                                      sizeX+10,33,0,
                                                      message.c_str());
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   warnWindow->setModal();
   warnWindow->setExternPointer(&warnWindow);
   gui->openWindow(warnWindow);
}

/***********************************************************************
 *                            getOkButton                              *
 ***********************************************************************/
button* warning::getOkButton()
{
   return(okButton);
}

/***********************************************************************
 *                             isOpened                                *
 ***********************************************************************/
bool warning::isOpened()
{
   return(warnWindow != NULL);
}

/***********************************************************************
 *                               close                                 *
 ***********************************************************************/
void warning::close()
{
   if(warnWindow != NULL)
   {
      lastGui->closeWindow(warnWindow);
   }
}

/***********************************************************************
 *                           static members                            *
 ***********************************************************************/
window* warning::warnWindow = NULL;
guiInterface* warning::lastGui = NULL;
button* warning::okButton = NULL;

