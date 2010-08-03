/* 
  DccNiTghtmare: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2009 DNTeam <dnt@dnteam.org>
 
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

#include "featsel.h"

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
featSelWindow::fSelFeatList::~fSelFeatList()
{
   clearList();
}

/********************************************************************
 *                           freeElement                            *
 ********************************************************************/
void featSelWindow::fSelFeatList::freeElement(dntListElement* obj)
{
   featSelWindow::fSelFeat* f = (featSelWindow::fSelFeat*)obj;
   delete(f);
}

/********************************************************************
 *                             getFeat                              *
 ********************************************************************/
featSelWindow::fSelFeat* featSelWindow::fSelFeatList::getFeat(string featId)
{
   int i;
   featSelWindow::fSelFeat* f = (featSelWindow::fSelFeat*)getFirst();
   for(i=0; i < total; i++)
   {
      if(f->desc->idString == featId)
      {
         return(f);
      }
      f = (featSelWindow::fSelFeat*)f->getNext();
   }

   return(NULL);
}

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
featSelWindow::featSelWindow(guiInterface* interf)
{
   /* set interface */
   inter = interf;

   /* Nullify pointers */
   intWindow = NULL;
}

/********************************************************************
 *                            Destructor                            *
 ********************************************************************/
featSelWindow::~featSelWindow()
{
   close();
}

/********************************************************************
 *                       defineAvailableFeats                       *
 ********************************************************************/
void featSelWindow::defineAvailableFeats()
{
   int i, j;
   featDescription* def;
   fSelFeat* fSel;

   /* Verify all available feats */
   for(i=0; i < allFeats->getTotal(); i++)
   {
      def = allFeats->featByNumber(i);
      if( (current->canHaveFeat(def)) && (!current->haveFeat(def->idString)) )
      {
         /* Available: Insert it at list */
         fSel = new(fSelFeat);
         fSel->desc = def;
         fSel->classFeat = false;
         availableFeats.insert(fSel);
      }
   }
   
   /* Define all race feats */
   if(current->actualRace)
   {
      for(j = 0; j < current->actualRace->totalFeats; j++)
      {
         fSel = availableFeats.getFeat(current->actualRace->raceFeats[j]);
         if(fSel)
         {
            fSel->classFeat = true;
            availableFeats.removeWithoutDelete(fSel);
            selectedFeats.insert(fSel); 
         }
      }
   }

   /* Define all class feats (already putting it at the selected) */
   for(i=0; i < MAX_DISTINCT_CLASSES; i++)
   {
      if(current->actualClass[i])
      {
         /* Verify all class feats */
         for(j = 0; j < current->actualClass[i]->totalFeats; j++)
         {
            fSel=availableFeats.getFeat(current->actualClass[i]->classFeats[j]);
            if(fSel)
            {
               fSel->classFeat = true;
               availableFeats.removeWithoutDelete(fSel);
               selectedFeats.insert(fSel); 
            }
         }
      }
   }
}

/********************************************************************
 *                              open                                *
 ********************************************************************/
void featSelWindow::open(character* pers, featsList* fList, int total)
{
   dirs dir;
   int i;

   /* Set open position */
   int posY = 1;
   int posX = 0;

   char buf[512];
   string tmpStr = "";

   /* Ignore, if pers or fList not defined */
   if( (!pers) || (!fList) )
   {
      return;
   }

   /* Set Current */
   current = pers;
   allFeats = fList;

   /* Define Available Feats */
   defineAvailableFeats();

   /* Create Window */
   intWindow = inter->insertWindow(posX, posY, posX+340, posY+278,
                                   pers->name);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                             close                                *
 ********************************************************************/
void featSelWindow::close()
{
   /* Close Character Window */
   if(intWindow)
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
      current = NULL;
   }
}

/********************************************************************
 *                             isOpen                               *
 ********************************************************************/
bool featSelWindow::isOpen()
{
   return(intWindow != NULL);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int featSelWindow::treat(guiObject* object, int eventInfo,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   if(!isOpen())
   {
      return(0);
   }

   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
   }
   return(0);
}

