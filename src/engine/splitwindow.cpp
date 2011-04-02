/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2011 DNTeam <dnt@dnteam.org>
 
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
void splitWindow::open(guiInterface* inter, object* o, engine* usedEngine, 
      character* c)
{
   /* Set things */
   interf = inter;
   obj = o;
   curEngine = usedEngine;
   owner = c;
   target = NULL;

   /* (re)open the window */
   open();
}

/***********************************************************************
 *                                 open                                *
 ***********************************************************************/
void splitWindow::open(guiInterface* inter, object* o, engine* usedEngine,
      character* c, character* t)
{
   /* Set things */
   interf = inter;
   obj = o;
   curEngine = usedEngine;
   owner = c;
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
   intWindow = interf->insertWindow(sX-110,sY-36,sX+110,sY+36, 
         gettext("Quantity"));
   list = intWindow->getObjectsList();

   /* Ok Button */
   okButton = list->insertButton(42,47,110,65,gettext("Ok"),1);
   cancelButton = list->insertButton(122,47,180,65,gettext("Cancel"),1);
   
   /* Quantity */
   posY = 17;
   lessButton = list->insertButton(139,posY+12,151,posY+23,
                                   fnt.createUnicode(0x25BC),0);
   lessButton->defineFont(DNT_FONT_ARIAL, 8);
   qtyText = list->insertTextBox(78,posY,138,posY+23,1,"");
   qtyText->setFont(DNT_FONT_ARIAL, 16, DNT_FONT_ALIGN_CENTER);
   moreButton = list->insertButton(139,posY,151,posY+11,
                                   fnt.createUnicode(0x25B2),0);
   moreButton->defineFont(DNT_FONT_ARIAL, 8);

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
bool splitWindow::treat(guiObject* guiObj, int eventInfo)
{
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
      }
   }

   /* Put the delta back to the usual, since no event got here. */
   delta = 1;

   return(false);
}

/***********************************************************************
 *                               static                                *
 ***********************************************************************/
object* splitWindow::obj = NULL;
int splitWindow::curQty = 0;
int splitWindow::delta = 1;
character* splitWindow::target = NULL;
character* splitWindow::owner = NULL;
engine* splitWindow::curEngine = NULL;
guiInterface* splitWindow::interf = NULL;
window* splitWindow::intWindow = NULL;
textBox* splitWindow::qtyText = NULL;
button* splitWindow::moreButton = NULL;
button* splitWindow::lessButton = NULL;
button* splitWindow::okButton = NULL;
button* splitWindow::cancelButton = NULL;

