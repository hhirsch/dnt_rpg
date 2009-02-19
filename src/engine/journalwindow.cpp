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
 *                        dupplicateCell                               *
 ***********************************************************************/
bTreeCell* journalAreas::dupplicateCell(bTreeCell* cell)
{
   journalArea* cur = (journalArea*)cell;
   journalArea* j = new journalArea();
   j->text = cur->text;
   j->descriptions = NULL; // don't dupplicate the list
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
   /* Create (or recreate) the missions info */
   createLists();
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

