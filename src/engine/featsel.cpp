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

#include "featsel.h"
using namespace std;

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
      if( (def->selectable) &&
          (current->canHaveFeat(def)) && 
          (!current->haveFeat(def->idString)) )
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
   farsoOptions opt;
   int i, y;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   dntFont fnt;
   std::string fontArial = opt.getDefaultFont();

   /* Ignore, if pers or fList not defined */
   if( (!pers) || (!fList) )
   {
      return;
   }

   /* Set Current */
   current = pers;
   allFeats = fList;
   curTotal = total;
   curSelPage = 1;
   curAvailPage = 1;

   /* Define Available Feats */
   defineAvailableFeats();

   /* Create Window */
   intWindow = inter->insertWindow(centerX-200, centerY-200, 
         centerX+200, centerY+200, gettext("Talents"));

   textBox* tb = intWindow->getObjectsList()->insertTextBox(10, 18, 194, 36, 1, 
         gettext("Available"));
   tb->setFont(fontArial,12,DNT_FONT_ALIGN_CENTER, DNT_FONT_STYLE_BOLD);
   tb->setBackColor(110, 0, 0);

   tb = intWindow->getObjectsList()->insertTextBox(195, 18, 390, 36, 1, 
         gettext("Selected"));
   tb->setFont(fontArial,12,DNT_FONT_ALIGN_CENTER, DNT_FONT_STYLE_BOLD);
   tb->setBackColor(110, 0, 0);

   /* Each Feat */
   y = 37;
   for(i=0; i < FEATS_PER_PAGE; i++)
   {
      /* Available */
      picAvail[i] = intWindow->getObjectsList()->insertPicture(12,y+2,0,0,NULL);
      picAvail[i]->setSurfaceDeletion(false);
      textAvail[i] = intWindow->getObjectsList()->insertTextBox(46,y,175,y+34,
            0, "");
      textAvail[i]->setReceiveEvents(true);
      buttonInsert[i] = intWindow->getObjectsList()->insertButton(176, y+9, 
            190, y+27, fnt.createUnicode(0x25BA),0);
      buttonInsert[i]->defineFont(fontArial, 9);

      /* Selected */
      buttonRemove[i] =  intWindow->getObjectsList()->insertButton(200, y+9, 
            214, y+27, fnt.createUnicode(0x25C4),0); 
      buttonRemove[i]->defineFont(fontArial, 9);
      picSel[i] = intWindow->getObjectsList()->insertPicture(216,y+2,0,0,NULL);
      picSel[i]->setSurfaceDeletion(false);
      textSel[i] = intWindow->getObjectsList()->insertTextBox(250,y,388,y+34,
            0, "");
      textSel[i]->setReceiveEvents(true);

      y += 34;
   }

   /* Contorn */
   intWindow->getObjectsList()->insertTextBox(10, 37, 194, y+5, 1, "");
   intWindow->getObjectsList()->insertTextBox(195, 37, 390, y+5, 1, "");

   /* Available page selectors */
   prevAvailButton = intWindow->getObjectsList()->insertButton(10, y+6,
                                    25, y+24, fnt.createUnicode(0x25C4),0);
   prevAvailButton->defineFont(fontArial, 9);
   textAvailPage = intWindow->getObjectsList()->insertTextBox(26, y+6, 
                                   178, y+24, 1, "0/0");
   textAvailPage->setFont(fontArial,10,DNT_FONT_ALIGN_CENTER,
                          DNT_FONT_STYLE_BOLD);
   nextAvailButton = intWindow->getObjectsList()->insertButton(179, y+6,
                                    194, y+24, fnt.createUnicode(0x25BA),0);
   nextAvailButton->defineFont(fontArial, 9);

   /* Selected page selectors */ 
   prevSelButton = intWindow->getObjectsList()->insertButton(195, y+6,
                                    210, y+24, fnt.createUnicode(0x25C4),0);
   prevSelButton->defineFont(fontArial, 9);
   textSelPage = intWindow->getObjectsList()->insertTextBox(211, y+6, 
                                   374, y+24, 1, "0/0");
   textSelPage->setFont(fontArial,10,DNT_FONT_ALIGN_CENTER,
                          DNT_FONT_STYLE_BOLD);
   nextSelButton = intWindow->getObjectsList()->insertButton(375, y+6,
                                   390, y+24, fnt.createUnicode(0x25BA),0);
   nextSelButton->defineFont(fontArial, 9);

   y += 2;
   /* Title & Description */
   textTitle = intWindow->getObjectsList()->insertTextBox(10, y+25, 390, y+43,
         1, "");
   textTitle->setFont(fontArial,11,DNT_FONT_ALIGN_CENTER,
         DNT_FONT_STYLE_BOLD);
   textTitle->setBackColor(102, 0, 0);
   textDescription = intWindow->getObjectsList()->insertRolBar(10, y+44, 
                                   390, 350, "");
   picFeat = intWindow->getObjectsList()->insertPicture(84,354,0,0,NULL);
   picFeat->setSurfaceDeletion(false);
   textTotal = intWindow->getObjectsList()->insertTextBox(120, 360, 310, 380,
       0, "");
   textTotal->setFont(fontArial,13,DNT_FONT_ALIGN_LEFT, 
         DNT_FONT_STYLE_BOLD);

   /* Ok Button */
   okButton = intWindow->getObjectsList()->insertButton(320,370,390,389,
         gettext("Confirm"),1);

   /* Cancel Button */
   cancelButton = intWindow->getObjectsList()->insertButton(10,370,80,389,
         gettext("Cancel"),1);

   /* Open Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(true,true,false,false);
   inter->openWindow(intWindow);

   /* Draw things */
   drawThings(NULL);
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
 *                          drawThings                              *
 ********************************************************************/
void featSelWindow::drawThings(fSelFeat* f)
{
   int i;
   fSelFeat* ft;
   char buf[128];

   /* Set the feat, if isn't yet defined */
   if(f == NULL)
   {
      f = (fSelFeat*)availableFeats.getFirst();
      if(f == NULL)
      {
         f = (fSelFeat*)selectedFeats.getFirst();
         if(f == NULL)
         {
            return;
         }
      }
   }

   sprintf(buf, "%d/%d", curAvailPage, 
         ((availableFeats.getTotal()-1) / FEATS_PER_PAGE)+1);
   textAvailPage->setText(buf);
   /* Set the available feats */
   ft = (fSelFeat*)availableFeats.getFirst();
   /* Get to the page */
   for(i=0; i < (FEATS_PER_PAGE*(curAvailPage-1)); i++)
   {
      ft = (fSelFeat*)ft->getNext();
   }
   /* Insert it */
   for(i = 0; i < FEATS_PER_PAGE; i++)
   {
      if(i+(curAvailPage-1)*FEATS_PER_PAGE < availableFeats.getTotal())
      {
         picAvail[i]->set(ft->desc->image);
         textAvail[i]->setText(ft->desc->name);
         buttonInsert[i]->setAvailable(true);
         buttonInsert[i]->show();
         curAvail[i] = ft;
         ft = (fSelFeat*)ft->getNext();
      }
      else
      {
         curAvail[i] = NULL;
         picAvail[i]->set(NULL);
         textAvail[i]->setText("");
         buttonInsert[i]->setAvailable(false);
         buttonInsert[i]->hide();
      }
   }

   sprintf(buf, "%d/%d", curSelPage, 
         ((selectedFeats.getTotal()-1) / FEATS_PER_PAGE)+1);
   textSelPage->setText(buf);
   /* Set the selected feats */
   ft = (fSelFeat*)selectedFeats.getFirst();
   /* Get to the page */
   for(i=0; i < (FEATS_PER_PAGE*(curSelPage-1)); i++)
   {
      ft = (fSelFeat*)ft->getNext();
   }
   for(i = 0; i < FEATS_PER_PAGE; i++)
   {
      if(i+(curSelPage-1)*FEATS_PER_PAGE < selectedFeats.getTotal())
      {
         picSel[i]->set(ft->desc->image);
         textSel[i]->setText(ft->desc->name);
         if(ft->classFeat)
         {
            buttonRemove[i]->hide();
            buttonRemove[i]->setAvailable(!ft->classFeat);
         }
         else
         {
            buttonRemove[i]->setAvailable(true);
            buttonRemove[i]->show();
         }
         curSel[i] = ft;
         ft = (fSelFeat*)ft->getNext();
      }
      else
      {
         curSel[i] = NULL;
         picSel[i]->set(NULL);
         textSel[i]->setText("");
         buttonRemove[i]->setAvailable(false);
         buttonRemove[i]->hide();
      }
   }

   /* set the total text */
   sprintf(buf, "%s: (%d)", gettext("Remaining"), curTotal);
   textTotal->setText(buf);

   /* Set selected feat title and description */
   textTitle->setText(f->desc->name);
   textDescription->setText(f->desc->description);
   picFeat->set(f->desc->image);
   intWindow->draw(0,0);
}


/********************************************************************
 *                              treat                               *
 ********************************************************************/
int featSelWindow::treat(guiObject* object, int eventInfo)
{
   int i;

   if(!isOpen())
   {
      return(TALENT_WINDOW_CANCEL);
   }

   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      /* Ok */
      if(object == (guiObject*)okButton)
      {
         /* Add All selected feats to the character */
         fSelFeat* ft = (fSelFeat*)selectedFeats.getFirst();
         for(i=0; i < selectedFeats.getTotal(); i++)
         {
            current->actualFeats->insertFeat(ft->desc);
            ft = (fSelFeat*)ft->getNext();
         }
 
         close();
         return(TALENT_WINDOW_CONFIRM);
      }
      /* Cancel */
      else if(object == (guiObject*)cancelButton)
      {
         close();
         return(TALENT_WINDOW_CANCEL);
      }
      /* Next Avail Button */
      else if(object == (guiObject*)nextAvailButton)
      {
         if((((availableFeats.getTotal()-1) / FEATS_PER_PAGE)+1) > curAvailPage)
         {
            curAvailPage += 1;
         }
         else
         {
            curAvailPage = 1;
         }
         drawThings(NULL);
      }
      /* Previous Avail Button */
      else if(object == (guiObject*)prevAvailButton)
      {
         curAvailPage--;
         if(curAvailPage <= 0)
         {
            curAvailPage = ((availableFeats.getTotal()-1) / FEATS_PER_PAGE)+1;
         }
         drawThings(NULL);
      }
      /* Next Selected Button */
      else if(object == (guiObject*)nextSelButton)
      {
         if((((selectedFeats.getTotal()-1) / FEATS_PER_PAGE)+1) > curSelPage)
         {
            curSelPage += 1;
         }
         else
         {
            curSelPage = 1;
         }
         drawThings(NULL);
      }
      /* Previous Selected Button */
      else if(object == (guiObject*)prevSelButton)
      {
         curSelPage--;
         if(curSelPage <= 0)
         {
            curSelPage = ((selectedFeats.getTotal()-1) / FEATS_PER_PAGE)+1;
         }
         drawThings(NULL);
      }
      /* Insert/Delete Buttons */
      else
      {
         for(i=0; i < FEATS_PER_PAGE; i++)
         {
            if(object == buttonInsert[i])
            {
               if(curTotal > 0)
               {
                  /* Insert at selected */
                  availableFeats.removeWithoutDelete(curAvail[i]);
                  selectedFeats.insert(curAvail[i]);
                  curTotal--;
                  drawThings(curAvail[i]);
               }
               else
               {
                  warning w;
                  w.show(gettext("Warning"), 
                         gettext("You can't select more talents"), inter);
               }
            }
            else if(object == buttonRemove[i])
            {
               /* Remove from selected */
               selectedFeats.removeWithoutDelete(curSel[i]);
               availableFeats.insert(curSel[i]);
               curTotal++;
               drawThings(curAvail[i]);
            }
         }
      }
   }
   /* Verify text press */
   else if(eventInfo == FARSO_EVENT_ON_PRESS_TEXT_BOX)
   {
      for(i=0; i < FEATS_PER_PAGE; i++)
      {
         if(object == textAvail[i])
         {
            drawThings(curAvail[i]);
            return(TALENT_WINDOW_OTHER);
         }
         else if(object == textSel[i])
         {
            drawThings(curSel[i]);
            return(TALENT_WINDOW_OTHER);
         }
      }
   }
   /* Verify picture press */
   else if(eventInfo == FARSO_EVENT_ON_PRESS_PICTURE)
   {
      for(i=0; i < FEATS_PER_PAGE; i++)
      {
         if(object == picAvail[i])
         {
            drawThings(curAvail[i]);
            return(TALENT_WINDOW_OTHER);
         }
         else if(object == picSel[i])
         {
            drawThings(curSel[i]);
            return(TALENT_WINDOW_OTHER);
         }
      }

   }
   return(TALENT_WINDOW_OTHER);
}

/********************************************************************
 *                  applyAllNewPermanentFeats                       *
 ********************************************************************/
void featSelWindow::applyAllNewPermanentFeats()
{
   int i;
   feat* f = NULL;

   /* Search on all permanent feats */
   fSelFeat* ft = (fSelFeat*)selectedFeats.getFirst();
   for(i=0; i < selectedFeats.getTotal(); i++)
   {
      if(ft->desc->type == FEAT_TYPE_PERMANENT)
      {
         f = current->actualFeats->featByString(ft->desc->idString);
         if(f)
         {
            /* Apply it */
            current->actualFeats->applyPermanentFeat(current, f->featNumber);
         }
      }

      ft = (fSelFeat*)ft->getNext();
   }
}

