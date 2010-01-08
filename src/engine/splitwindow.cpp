/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2010 DNTeam <dnt@dnteam.org>
 
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

#include "splitwindow.h"

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void splitWindow::open(guiInterface* inter, object* o, void* usedEngine, 
      GLfloat pX, GLfloat pZ)
{
   /* Set things */
   interf = inter;
   obj = o;
   curEngine = usedEngine;
   posX = pX;
   posZ = pZ;
   target = NULL;

   /* (re)open the window */
   open();
}

/***********************************************************************
 *                                 open                                *
 ***********************************************************************/
void splitWindow::open(guiInterface* inter, object* o, character* t)
{
   /* Set things */
   interf = inter;
   obj = o;
   curEngine = NULL;
   posX = -1;
   posZ = -1;
   target = t;

   /* (re)open the window */
   open();
}

/***********************************************************************
 *                                 open                                *
 ***********************************************************************/
void splitWindow::open()
{
   dntFont fnt;
   guiList* list;
   int posY;
   int sX = SCREEN_X / 2;
   int sY = SCREEN_Y / 2;

   /* Close it if is open */
   close();

   /* Create the window */
   intWindow = interf->insertWindow(sX-110,sY-46,sX+110,sY+46, 
         gettext("Quantity"));
   list = intWindow->getObjectsList();

   /* Ok Button */
   okButton = list->insertButton(92,57,150,75,gettext("Ok"),1);
   cancelButton = list->insertButton(92,57,150,75,gettext("Cancel"),1);
   
   /* Message */
   textBox* quadText = list->insertTextBox(5,17,215,33,0,"Quantity:");
   quadText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);

   /* Quantity */
   posY = 37;
   lessButton = list->insertButton(79,posY,89,posY+17,
                                   fnt.createUnicode(0x25C4),0);
   lessButton->defineFont(DNT_FONT_ARIAL, 9);
   qtyText = list->insertTextBox(78,posY,138,posY+17,1,"");
   qtyText->setFont(DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_CENTER);
   moreButton = list->insertButton(139,posY,149,posY+17,
                                   fnt.createUnicode(0x25BA),0);
   moreButton->defineFont(DNT_FONT_ARIAL, 9);

   /* Set the window and open */
   intWindow->setModal();
   intWindow->setExternPointer(&intWindow);
   interf->openWindow(intWindow);

   /* Do the text update */
   updateText();


}

/***********************************************************************
 *                                close                                *
 ***********************************************************************/
void splitWindow::close()
{
   if( (isOpen()) && (interf) )
   {
      interf->closeWindow(intWindow);
   }
}

/***********************************************************************
 *                                isOpen                               *
 ***********************************************************************/
bool splitWindow::isOpen()
{
   return(intWindow != NULL);
}

/***********************************************************************
 *                             updateText                              *
 ***********************************************************************/
void splitWindow::updateText()
{
   char buf[8];

   sprintf(buf, "%d", curQty);
   qtyText->setText(buf);
}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
bool splitWindow::treat(guiObject* guiObj, int eventInfo, GLfloat X, GLfloat Z)
{
   bool ret = false;

   /* TODO: delta change! */

   if(isOpen())
   {
      /* Verify if the current object quantity is greater than or equal to
       * the current defined quantity at the window */
      if(curQty > obj->getState())
      {
         /* Must apply the limit */
         curQty = obj->getState();
         updateText();
      }

      /* Verify button press */
      if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
      {
         if(guiObj == okButton)
         {
            /* TODO: apply action!  */

            /* Done with the split, so close the window */
            close();
            return(true);
         }
         else if(guiObj == cancelButton)
         {
            /* canceled the split */
            close();
            return(true);
         }
      }

      /* Verify button pressing */
      else if(eventInfo == FARSO_EVENT_ON_PRESS_BUTTON)
      {
         if(guiObj == moreButton)
         {
            /* Inc the value */
            if(curQty + delta < obj->getState())
            {
               curQty += delta;
            }
            else
            {
               curQty = obj->getState();
            }
            updateText();
            return(true);
         }
         else if(guiObj == lessButton)
         {
            /* Dec the value */
            if(curQty - delta > 0)
            {
               curQty -= delta;
            }
            else
            {
               curQty = 0;
            }
            updateText();
            return(true);
        }
        else
        {
           /* Put the delta back to the usual */
           delta = 1;
        }
      }
   }

   return(ret);
}

/***********************************************************************
 *                               static                                *
 ***********************************************************************/
object* splitWindow::obj = NULL;
GLfloat splitWindow::posX = -1;
GLfloat splitWindow::posZ = -1;
int splitWindow::curQty = 0;
int splitWindow::delta = 1;
character* splitWindow::target = NULL;
void* splitWindow::curEngine = NULL;
guiInterface* splitWindow::interf = NULL;
window* splitWindow::intWindow = NULL;
textBox* splitWindow::qtyText = NULL;
button* splitWindow::moreButton = NULL;
button* splitWindow::lessButton = NULL;
button* splitWindow::okButton = NULL;
button* splitWindow::cancelButton = NULL;

