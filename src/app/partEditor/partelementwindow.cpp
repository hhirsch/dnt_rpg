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

#include "partelementwindow.h"

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partElementWindow::partElementWindow(guiInterface* interf)
{
   curWindow = NULL;
   element = NULL;
   elementName = "";
   gui = interf;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
partElementWindow::~partElementWindow()
{
   closeWindow();
}

/***********************************************************************
 *                              setElement                             *
 ***********************************************************************/
void partElementWindow::setElement(dntPartElement* e, string text)
{
   if( (e != element) || (!isOpen()) )
   {
      elementName = text;
      element = e;
      openWindow();
   }
}

/***********************************************************************
 *                              isOpened                               *
 ***********************************************************************/
bool partElementWindow::isOpen()
{
   return(curWindow != NULL);
}

/***********************************************************************
 *                             openWindow                              *
 ***********************************************************************/
void partElementWindow::openWindow()
{
   int curPosX = 240;
   int curPosY = 40;
   dntFont fnt;

   /* Close it if already opened */
   if(isOpen())
   {
      curPosX = curWindow->getX1();
      curPosY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(curPosX, curPosY, curPosX+220, curPosY+300,
                                elementName); 

   /* Type things */
   previousType = curWindow->getObjectsList()->insertButton(60, 20, 70, 37, 
         fnt.createUnicode(0x25C4),0);
   previousType->defineFont(DNT_FONT_ARIAL, 9);
   type = curWindow->getObjectsList()->insertTextBox(71, 20, 150, 37, 1, "");
   nextType = curWindow->getObjectsList()->insertButton(151, 20, 161, 37, 
         fnt.createUnicode(0x25BA),0);
   nextType->defineFont(DNT_FONT_ARIAL, 9);

   /* Initial Value */
   texts[0] = curWindow->getObjectsList()->insertTextBox(10, 43, 80, 60, 0, 
         "Initial Value:");
   initialValue = curWindow->getObjectsList()->insertTextBar(81, 43, 141, 60,
         "", 0);
   /* Final Value */
   texts[1] = curWindow->getObjectsList()->insertTextBox(10, 66, 80, 83, 0, 
         "Final Value:");
   finalValue = curWindow->getObjectsList()->insertTextBar(81, 66, 141, 83,
         "", 0);
   finalValue->setAvailable(false);

   gui->openWindow(curWindow);

#if 0
      cxSel* useLimits;         /**< If will use the limits or not */
      textBar* lowerLimit;      /**< Lower limit */
      textBar* upperLimit;      /**< Upper limit */

      /* Random */
      textBar* mult;            /**< Multiplier value */
      textBar* sum;             /**< Sum value */

      /* Linear */
      textBar* initialValue;    /**< initial value */
      textBar* finalValue;      /**< final value */

      textBox* texts[7]
#endif
}

/***********************************************************************
 *                            closeWindow                              *
 ***********************************************************************/
void partElementWindow::closeWindow()
{
   if(isOpen())
   {
      gui->closeWindow(curWindow);
   }
}

