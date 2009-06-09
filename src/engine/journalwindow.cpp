/* 
  DccNiTghtmare: a satiric post-apocalyptical RPG.
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

#include "journalwindow.h"

#include "../classes/mission.h"

/***************************************************************************
 *                                                                         *
 *                             JournalArea                                 *
 *                                                                         *
 ***************************************************************************/

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
journalArea::journalArea()
{
   text = "";
   descriptions = NULL;
   totalDescriptions = 0;
}

/***********************************************************************
 *                            Destructor                               *
 ***********************************************************************/
journalArea::~journalArea()
{
   int i;
   journalDesc* desc;

   /* Delete all the descriptions */
   for(i = 0; i < totalDescriptions; i++)
   {
      desc = descriptions;
      descriptions = descriptions->next;
      delete(desc);
   }
}

/***********************************************************************
 *                            insertMission                            *
 ***********************************************************************/
void journalArea::insertMission(string desc, bool comp)
{
   journalDesc* d = new journalDesc();
   d->text = desc;
   d->completed = comp;

   if(!descriptions)
   {
      d->next = d;
      d->previous = d;
   }
   else
   {
      d->next = descriptions;
      d->previous = descriptions->previous;
      d->next->previous = d;
      d->previous->next = d;
   }

   descriptions = d;
   totalDescriptions++;
}

/***********************************************************************
 *                            compare                                  *
 ***********************************************************************/
int journalArea::compare(bTreeCell* cell)
{
   journalArea* c = (journalArea*)cell;

   return(text.compare(c->text));
}

/***********************************************************************
 *                             merge                                   *
 ***********************************************************************/
void journalArea::merge(bTreeCell* cell)
{
   /* Do not used */
}

/***************************************************************************
 *                                                                         *
 *                            JournalAreas                                 *
 *                                                                         *
 ***************************************************************************/

/***********************************************************************
 *                          Constructor                                *
 ***********************************************************************/
journalAreas::journalAreas() : bTree(false)
{
}

/***********************************************************************
 *                           Destructor                                *
 ***********************************************************************/
journalAreas::~journalAreas()
{
   clearSubTree(getRoot());
}

/***********************************************************************
 *                             insert                                  *
 ***********************************************************************/
journalArea* journalAreas::insert(string title)
{
   journalArea* area = new journalArea();
   area->text = title;

   return( (journalArea*)bTree::insert(area) );
}

/***********************************************************************
 *                              search                                 *
 ***********************************************************************/
journalArea* journalAreas::search(string title)
{
   journalArea area;
   area.text = title;

   return( (journalArea*)bTree::search(&area) );
}

/***********************************************************************
 *                               get                                   *
 ***********************************************************************/
journalArea* journalAreas::get(int num)
{
   int cur = 0;
   return(get(num, (journalArea*)getRoot(), cur));
}

/***********************************************************************
 *                               get                                   *
 ***********************************************************************/
journalArea* journalAreas::get(int num, journalArea* curRoot, int& cur)
{
   journalArea* ret = NULL;

   /* Initial Condition */
   if(curRoot == NULL)
   {
      return(NULL);
   }

   /* Return condition: equal indexes */
   if(num == cur)
   {
      return(curRoot);
   }

   /* Call for left */
   cur++;
   ret = get(num, (journalArea*)curRoot->getLeft(), cur);

   /* Call for right */
   if(!ret)
   {
      cur++;
      ret = get(num, (journalArea*)curRoot->getRight(), cur);
   }

   return(ret);
}

/***********************************************************************
 *                        dupplicateCell                               *
 ***********************************************************************/
bTreeCell* journalAreas::dupplicateCell(bTreeCell* cell)
{
   journalArea* cur = (journalArea*)cell;
   journalArea* j = new journalArea();
   j->text = cur->text;
   j->descriptions = NULL; // don't dupplicate the list

   return(j);
}

/***********************************************************************
 *                           freeCell                                  *
 ***********************************************************************/
void journalAreas::freeCell(bTreeCell* cell)
{
   journalArea* j = (journalArea*) cell;
   delete(j);
}

/***************************************************************************
 *                                                                         *
 *                            JournalWindow                                *
 *                                                                         *
 ***************************************************************************/

/***********************************************************************
 *                          Constructor                                *
 ***********************************************************************/
journalWindow::journalWindow()
{
   /* Nullify pointers */
   areas = NULL;
   gui = NULL;
   internalWindow = NULL;
   nextButton = NULL;
   previousButton = NULL;
   areaText = NULL;
   missionsText = NULL;
}

/***********************************************************************
 *                           Destructor                                *
 ***********************************************************************/
journalWindow::~journalWindow()
{
   /* Clear the bTree */
   if(areas)
   {
      delete(areas);
   }

   /* Close the window */
   close();
}

/***********************************************************************
 *                            isOpen                                   *
 ***********************************************************************/
bool journalWindow::isOpen()
{
   return(internalWindow != NULL);
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void journalWindow::close()
{
   if(isOpen())
   {
      gui->closeWindow(internalWindow);
   }
}

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void journalWindow::open(guiInterface* inter)
{
   /* Set open position */
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   dntFont fnt;

   /* Set the inteface */
   gui = inter;

   /* Create (or recreate) the missions info */
   createLists();

   /* Create Window */
   internalWindow = gui->insertWindow(centerX-128, centerY-128,
                                      centerX+128, centerY+128,
                                      gettext("Journal"));
   
   missionsText = internalWindow->getObjectsList()->insertRolBar(8,20,248,230,
                                                                 "");
   previousButton = internalWindow->getObjectsList()->insertButton(8,231,23,249,
                                                   fnt.createUnicode(0x25C4),0);
   areaText = internalWindow->getObjectsList()->insertTextBox(24,231,229,249,1,
                                                              "");
   areaText->setFont(DNT_FONT_ARIAL,10,DNT_FONT_ALIGN_CENTER, 
                     DNT_FONT_STYLE_BOLD);
   nextButton = internalWindow->getObjectsList()->insertButton(230,231,248,249,
                                                   fnt.createUnicode(0x25BA),0);

   /* Populate the window */
   internalWindow->setExternPointer(&internalWindow);
   gui->openWindow(internalWindow);

   /* Load the info to the window */
   curArea = 0;
   showArea();
}

/***********************************************************************
 *                             showArea                                *
 ***********************************************************************/
void journalWindow::showArea()
{
   int i;
   char buf[8];
   string text;

   if( (areas) && (isOpen()) )
   {
      /* Clear the current text */
      missionsText->setText("");

      /* Get the desired area */
      journalArea* area = areas->get(curArea);
      if(area)
      {
         /* Set Area text */
         areaText->setText(area->text);

         /* Show all area missions */
         journalDesc* desc = area->descriptions;
         for(i = 0; i < area->totalDescriptions; i++)
         {
            sprintf(buf,"%d - ", i+1);
            text = buf + desc->text;
            if(!desc->completed)
            {
               missionsText->addText(text, DNT_FONT_ARIAL, 9,
                                     DNT_FONT_ALIGN_LEFT, 
                                     DNT_FONT_STYLE_NORMAL, 240, 120, 0);
            }
            else
            {
               missionsText->addText(text, DNT_FONT_ARIAL, 9,
                                     DNT_FONT_ALIGN_LEFT, 
                                     DNT_FONT_STYLE_ITALIC, 0, 120, 0);
            }

            desc = desc->next;
         }
      }
      else
      {
         /* No know missions, so tell it! */
         areaText->setText(gettext("Unknow Area"));
         missionsText->setText(gettext("No missions."));
      }
   }
}

/***********************************************************************
 *                            createLists                              *
 ***********************************************************************/
void journalWindow::createLists()
{
   /* Recreate the structure */
   if(areas)
   {
      delete(areas);
   }
   areas = new journalAreas();

   /* populate the bTree */
   missionsController missions;
   mission* mis;
   journalArea* area;
   journalArea aSearch;

   /* with current missions */
   for(mis = missions.getFirstCurrentMission(); mis != NULL;
       mis = missions.getNextCurrentMission())
   {
      area = areas->search(mis->getArea());
      if(!area)
      {
         area = areas->insert(mis->getArea());
      }
      area->insertMission(mis->getDescription(), false);
   }

   /* with completed missions */
   for(mis = missions.getFirstCompletedMission(); mis != NULL;
       mis = missions.getNextCompletedMission())
   {
      area = areas->search(mis->getArea());
      if(!area)
      {
         area = areas->insert(mis->getArea());
      }
      area->insertMission(mis->getDescription(), true);
   }
}

