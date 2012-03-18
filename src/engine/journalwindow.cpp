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

#include "journalwindow.h"
#include "../classes/mission.h"
#include "../etc/dirs.h"
using namespace std;

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
   name = "";
}

/***********************************************************************
 *                            Destructor                               *
 ***********************************************************************/
journalArea::~journalArea()
{
   /* Delete all the descriptions */
   clearList();
}

/***********************************************************************
 *                             freeElement                             *
 ***********************************************************************/
void journalArea::freeElement(dntListElement* obj)
{
   journalDesc* desc = (journalDesc*)obj;
   delete(desc);
}

/***********************************************************************
 *                                getName                              *
 ***********************************************************************/
string journalArea::getName()
{
   return(name);
}

/***********************************************************************
 *                                setName                              *
 ***********************************************************************/
void journalArea::setName(string n)
{
   name = n;
}

/***********************************************************************
 *                            insertMission                            *
 ***********************************************************************/
void journalArea::insertMission(string desc, bool comp)
{
   /* Create and define the description */
   journalDesc* d = new journalDesc();
   d->text = desc;
   d->completed = comp;
   /* Insert it on the list */
   insert(d);
}

/***********************************************************************
 *                            compare                                  *
 ***********************************************************************/
int journalArea::compare(bTreeCell* cell)
{
   journalArea* c = (journalArea*)cell;

   return(name.compare(c->getName()));
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
   area->setName(title);

   return( (journalArea*)bTree::insert(area) );
}

/***********************************************************************
 *                              search                                 *
 ***********************************************************************/
journalArea* journalAreas::search(string title)
{
   journalArea area;
   area.setName(title);

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
   j->setName(cur->getName());
   j->clearList(); // don't dupplicate the list

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
void journalWindow::open(Farso::GuiInterface* inter)
{
   /* Set open position */
   int centerY = Farso::SCREEN_Y / 2;
   int centerX = Farso::SCREEN_X / 2;
   Farso::Font fnt;
   dirs dir;

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
   areaText->setFont(dir.getRealFile(DNT_FONT_ARIAL), 10,
                     Farso::Font::ALIGN_CENTER, 
                     Farso::Font::STYLE_BOLD);
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
   dirs dir;

   if( (areas) && (isOpen()) )
   {
      /* Clear the current text */
      missionsText->setText("");

      /* Get the desired area */
      journalArea* area = areas->get(curArea);
      if(area)
      {
         /* Set Area text */
         areaText->setText(area->getName());

         /* Show all area missions */
         journalDesc* desc = (journalDesc*)area->getFirst();
         for(i = 0; i < area->getTotal(); i++)
         {
            sprintf(buf,"%d - ", i+1);
            text = buf + desc->text;
            if(!desc->completed)
            {
               missionsText->addText(text, dir.getRealFile(DNT_FONT_ARIAL), 9,
                                     Farso::Font::ALIGN_LEFT, 
                                     Farso::Font::STYLE_NORMAL, 240, 120, 0);
            }
            else
            {
               missionsText->addText(text, dir.getRealFile(DNT_FONT_ARIAL), 9,
                                     Farso::Font::ALIGN_LEFT, 
                                     Farso::Font::STYLE_ITALIC, 0, 120, 0);
            }

            desc = (journalDesc*)desc->getNext();
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

