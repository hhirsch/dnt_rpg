/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2012 DNTeam <dnt@dnteam.org>
 
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
#include "../../gui/farsoopts.h"
using namespace std;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
partElementWindow::partElementWindow(guiInterface* interf)
{
   curWindow = NULL;
   element = NULL;
   part = NULL;
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
void partElementWindow::setElement(dntPartElement* e, string text, 
      particleSystem* p)
{
   if(!e)
   {
      return;
   }

   if( (e != element) || (!isOpen()) )
   {
      elementName = text;
      part= p;
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
   farsoOptions opt;
   int curPosX = 194;
   int curPosY = 64;
   dntFont fnt;

   /* Close it if already opened */
   if(isOpen())
   {
      curPosX = curWindow->getX1();
      curPosY = curWindow->getY1();
      closeWindow();
   }

   /* Create the window */
   curWindow = gui->insertWindow(curPosX, curPosY, curPosX+151, curPosY+212,
                                elementName); 

   /* Type things */
   previousType = curWindow->getObjectsList()->insertButton(10, 20, 20, 37, 
         fnt.createUnicode(0x25C4),0);
   previousType->defineFont(opt.getDefaultFont(), 9);
   type = curWindow->getObjectsList()->insertTextBox(21, 20, 130, 37, 1, "");
   type->setFont(opt.getDefaultFont(), 9, DNT_FONT_ALIGN_CENTER);
   nextType = curWindow->getObjectsList()->insertButton(131, 20, 141, 37, 
         fnt.createUnicode(0x25BA),0);
   nextType->defineFont(opt.getDefaultFont(), 9);

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
   /* Mult */
   texts[2] = curWindow->getObjectsList()->insertTextBox(10, 89, 80, 106, 0, 
         "Mult Value:");
   mult = curWindow->getObjectsList()->insertTextBar(81, 89, 141, 106, "", 0);
   /* Sum */
   texts[3] = curWindow->getObjectsList()->insertTextBox(10, 112, 80, 129, 0, 
         "Sum Value:");
   sum = curWindow->getObjectsList()->insertTextBar(81, 112, 141, 129, "", 0);
   /* Limits */
   useLimits = curWindow->getObjectsList()->insertCxSel(40, 139, false); 
   texts[4] = curWindow->getObjectsList()->insertTextBox(51, 135, 110, 152, 0,
         "Use Limits");
   /* Lower Limit */
   texts[5] = curWindow->getObjectsList()->insertTextBox(10, 158, 80, 175, 0, 
         "Lower Limit");
   lowerLimit = curWindow->getObjectsList()->insertTextBar(81, 158, 141, 175, 
         "", 0);
   /* Upper Limit */
   texts[6] = curWindow->getObjectsList()->insertTextBox(10, 181, 80, 198, 0, 
         "Upper Limit");
   upperLimit = curWindow->getObjectsList()->insertTextBar(81, 181, 141, 198, 
         "", 0);

   /* Finally open the window */
   gui->openWindow(curWindow);
   curWindow->setExternPointer(&curWindow);

   /* Set it */
   setTexts();
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

/***********************************************************************
 *                            setTypeText                              *
 ***********************************************************************/
void partElementWindow::setTypeThings()
{
   switch(element->type)
   {
      case DNT_PART_ELEMENT_CONSTANT:
      {
         type->setText("Constant");
         mult->setAvailable(false);
         sum->setAvailable(false);
         finalValue->setAvailable(false);
         initialValue->setAvailable(true);
         texts[0]->setText("Initial Value");
      }
      break;
      case DNT_PART_ELEMENT_RANDOM:
      {
         type->setText("Random");
         mult->setAvailable(true);
         sum->setAvailable(true);
         finalValue->setAvailable(false);
         initialValue->setAvailable(true);
         texts[0]->setText("Base Value");
      }
      break;
      case DNT_PART_ELEMENT_LINEAR:
      {
         type->setText("Linear");
         mult->setAvailable(false);
         sum->setAvailable(false);
         finalValue->setAvailable(true);
         initialValue->setAvailable(true);
         texts[0]->setText("Initial Value");
      }
      break;
      case DNT_PART_ELEMENT_RANDOM_INITIAL:
      {
         type->setText("Random Initial");
         mult->setAvailable(true);
         sum->setAvailable(true);
         finalValue->setAvailable(false);
         initialValue->setAvailable(true);
         texts[0]->setText("Base Value");
      }
      break;
   }
   curWindow->draw();
}

/***********************************************************************
 *                             setTexts                                *
 ***********************************************************************/
void partElementWindow::setTexts()
{
   char buf[128];
   setTypeThings();

   /* Mult */
   sprintf(buf, "%f", element->mult);
   mult->setText(buf);

   /* Sum */
   sprintf(buf, "%f", element->sum);
   sum->setText(buf);

   /* Initial */
   sprintf(buf, "%f", element->initialValue);
   initialValue->setText(buf);

   /* Final */
   sprintf(buf, "%f", element->finalValue);
   finalValue->setText(buf);

   /* UpperLimit */
   sprintf(buf, "%f", element->upperLimit);
   upperLimit->setText(buf);

   /* LowerLimit */
   sprintf(buf, "%f", element->lowerLimit);
   lowerLimit->setText(buf);

   /* Use limits */
   useLimits->setSelection(element->limitsDefined);
   upperLimit->setAvailable(element->limitsDefined);
   lowerLimit->setAvailable(element->limitsDefined);
}

/***********************************************************************
 *                            resetElement                             *
 ***********************************************************************/
void partElementWindow::resetElement()
{
   switch(element->type)
   {
      case DNT_PART_ELEMENT_CONSTANT:
      {
         element->setConstant(element->initialValue);
      }
      break;
      case DNT_PART_ELEMENT_RANDOM:
      {
         element->setRandom(element->initialValue, element->mult, element->sum);
      }
      break;
      case DNT_PART_ELEMENT_LINEAR:
      {
         element->setLinear(element->initialValue, element->finalValue, 
               part->getMaxParticleLifeTime());
      }
      break;
      case DNT_PART_ELEMENT_RANDOM_INITIAL:
      {
         element->setRandomInitial(element->initialValue, element->mult, 
               element->sum);
      }
      break;
   }
}

/***********************************************************************
 *                               treat                                 *
 ***********************************************************************/
bool partElementWindow::treat(guiObject* object, int eventInfo)
{
   if(!isOpen())
   {
      /* No need to verify events on a closed window */
      return(false);
   }

   /* Button press */
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      if(object == (guiObject*)nextType)
      {
         if(element->type < DNT_PART_ELEMENT_RANDOM_INITIAL)
         {
            element->type++;
            resetElement();
            setTypeThings();
            return(true);
         }
      }
      else if(object == (guiObject*)previousType)
      {
         if(element->type > 0)
         {
            element->type--;
            resetElement();
            setTypeThings();
            return(true);
         }
      }
   }
   /* cxSel changed */
   else if(eventInfo == FARSO_EVENT_MODIFIED_CX_SEL)
   {
      if(object == (guiObject*)useLimits)
      {
         /* Set useLimits */
         element->limitsDefined = useLimits->isSelected();
         lowerLimit->setAvailable(useLimits->isSelected());
         upperLimit->setAvailable(useLimits->isSelected());
         curWindow->draw();
         return(true);
      }
   }
   else if(eventInfo == FARSO_EVENT_WROTE_TEXT_BAR)
   {
      if(object == (guiObject*)initialValue)
      {
         sscanf(initialValue->getText().c_str(), "%f", &element->initialValue);
         resetElement();
         return(true);
      }
      else if(object == (guiObject*)finalValue)
      {
         sscanf(finalValue->getText().c_str(), "%f", &element->finalValue);
         resetElement();
         return(true);
      }
      else if(object == (guiObject*)mult)
      {
         sscanf(mult->getText().c_str(), "%f", &element->mult);
         resetElement();
         return(true);
      }
      else if(object == (guiObject*)sum)
      {
         sscanf(sum->getText().c_str(), "%f", &element->sum);
         resetElement();
         return(true);
      }
      else if(object == (guiObject*)lowerLimit)
      {
         sscanf(lowerLimit->getText().c_str(), "%f", &element->lowerLimit);
         return(true);
      }
      else if(object == (guiObject*)upperLimit)
      {
         sscanf(upperLimit->getText().c_str(), "%f", &element->upperLimit);
         return(true);
      }

   }

   return(false);
}

