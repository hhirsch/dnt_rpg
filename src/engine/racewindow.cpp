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


#include "racewindow.h"
using namespace std;

/***********************************************************************
 *                           cmpRaceFunction                           *
 ***********************************************************************/
static int cmpRaceFunction(const void *p1,  const void *p2)
{
   /* Compare Function for the quicksort */
   race** r1 = (race**) p1;
   race** r2 = (race**) p2;

   return((*r1)->name.compare((*r2)->name));
}

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
raceWindow::raceWindow(guiInterface* inter, race** retRace)
{
   races rc;
   dntFont fnt;
   int i;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   choosedRace = retRace;
   curRace = -1;

   /* Alphabetical Order Races */
   totalRaces = rc.getTotalRaces();
   racesOrder = new race*[totalRaces];
   for(i = 0; i < totalRaces; i++)
   {
      racesOrder[i] = rc.getRaceByInteger(i);
   }
   qsort(&racesOrder[0], totalRaces, sizeof(race**), cmpRaceFunction);
   
   if(*choosedRace != NULL)
   {
      /* Find it on vector */
      for(i = 0; ((i < totalRaces) && (curRace == -1)); i++)
      {
         if((*choosedRace)->name == racesOrder[i]->name)
         {
            curRace = i;
         }
      }
      if(curRace == -1)
      {
         /* Not found */
         curRace = 0;
      }
   }
   else
   {
      curRace = 0;
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-175,centerY-200,
                                   centerX+175,centerY+200,
                                   gettext("Race"));

   /* Race Image */
   intWindow->getObjectsList()->insertTextBox(278,18,344,364,2,"");
   raceImage = intWindow->getObjectsList()->insertPicture(279,157,0,0,NULL);   
   raceImage->setSurfaceDeletion(false);
   raceImage->set(racesOrder[curRace]->image);

   /* TabBox */
   tabBox *tab=(tabBox*)intWindow->getObjectsList()->defineTabBox(6,18,277,345);

   guiList *listChar = tab->insertOption(gettext("Characteristics"));
   guiList *listDesc = tab->insertOption(gettext("Description"));

   /* Race Description */
   textDesc = listDesc->insertRolBar(9,39,274,341,"");
   setDescription();

   /* Race Characteristics */
   textCharac = listChar->insertRolBar(9,39,274,341,"");
   setCharacteristics();

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(6,346,21,364,
                                             fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(262,346,277,364,
                                             fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(22,346,261,364,1, 
                                             racesOrder[curRace]->name.c_str());
   textName->setFont(DNT_FONT_ARIAL,10,DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_BOLD);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(270,370,340,389,
                                         gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(10,370,80,389,
                                               gettext("Cancel"),1);

   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                           Destructor                             *
 ********************************************************************/
raceWindow::~raceWindow()
{
   delete[]racesOrder;
}

/********************************************************************
 *                         setDescription                           *
 ********************************************************************/
void raceWindow::setDescription()
{
   textDesc->setText("");
   textDesc->addText(racesOrder[curRace]->citation + "||", DNT_FONT_ARIAL,
                     10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                     220,0,0);
   textDesc->addText(racesOrder[curRace]->description);
   textDesc->setFirstLine(0);
}

/********************************************************************
 *                       setCharacteristics                         *
 ********************************************************************/
void raceWindow::setCharacteristics()
{
   int i;
   skillDefinition* skTmp;
   skillsDefinitions skDefs;

   textCharac->setText("");
   textCharac->addText(string(gettext("Race Modifiers")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<racesOrder[curRace]->totalModifiers; i++)
   {
      textCharac->addText(racesOrder[curRace]->raceModifiers[i].description + 
                          "||");
   }

   if(racesOrder[curRace]->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75, 147, 207);
   }

   textCharac->addText(string(gettext("Race Talents")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);

   for(i=0; i<racesOrder[curRace]->totalFeats; i++)
   {
      textCharac->addText(racesOrder[curRace]->raceFeats[i] + "|");
   }

   if(racesOrder[curRace]->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Talents.")) + "|",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   /* Race Skills */
   textCharac->addText(string("|") + string(gettext("Race Skills")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<racesOrder[curRace]->totalSkills; i++)
   {
      skTmp = skDefs.getSkillDefinition(racesOrder[curRace]->raceSkills[i]);
      if(skTmp)
      {
         textCharac->addText(skTmp->name + "|");
      }
      else
      {
         textCharac->addText(racesOrder[curRace]->raceSkills[i] + "|");
      }
   }

   if(racesOrder[curRace]->totalSkills == 0)
   {
      textCharac->addText(gettext("No Skills."),
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   textCharac->setFirstLine(0);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int raceWindow::treat(guiObject* object, int eventInfo, 
                      guiInterface* inter)
{
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      {
         if(object == (guiObject*) buttonNext)
         {
            curRace = (curRace + 1) % totalRaces;
         }
         else
         {
            curRace--;
            if(curRace < 0)
            {
              curRace = totalRaces-1;
            }
         }
         textName->setText(racesOrder[curRace]->name);
         setDescription();
         setCharacteristics();
         raceImage->set(racesOrder[curRace]->image);
         intWindow->draw(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         *choosedRace = racesOrder[curRace];
         inter->closeWindow(intWindow);
         intWindow = NULL;
         return(RACEW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         inter->closeWindow(intWindow);
         *choosedRace = NULL;
         intWindow = NULL;
         return(RACEW_CANCEL);
      }
   }
   return(RACEW_OTHER);
}

