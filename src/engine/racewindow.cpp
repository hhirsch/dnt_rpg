#include "racewindow.h"

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
raceWindow::raceWindow(races* rc, skills* sk, guiInterface* inter,
                       race** retRace)
{
   dntFont fnt;
   int i;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   glDisable(GL_LIGHTING);

   externalSkills = sk;
   choosedRace = retRace;
   curRace = -1;

   /* Alphabetical Order Races */
   totalRaces = rc->getTotalRaces();
   racesOrder = new race*[totalRaces];
   for(i = 0; i < totalRaces; i++)
   {
      racesOrder[i] = rc->getRaceByInteger(i);
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
   intWindow = inter->insertWindow(centerX-310,centerY-200,
                                   centerX+310,centerY+200,
                                   gettext("Race"));

   /* Race Image */
   intWindow->getObjectsList()->insertTextBox(278,18,344,345,2,"");
   raceImage = intWindow->getObjectsList()->insertPicture(279,152,0,0,NULL);   
   raceImage->set(racesOrder[curRace]->image);

   /* Race Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(6,18,277,35,1,
                                            gettext("Race Description"));
   textDescTitle->setFont(DNT_FONT_TIMES,12,DNT_FONT_ALIGN_CENTER,
                          DNT_FONT_STYLE_BOLD);
   
   textDesc = intWindow->getObjectsList()->insertRolBar(6,36,277,345,"");
   setDescription();

   /* Race Characteristics */
   textCharacTitle = intWindow->getObjectsList()->insertTextBox(345,18,615,35,1,
                                       gettext("Race Characteristcs"));
   textCharacTitle->setFont(DNT_FONT_TIMES,12,DNT_FONT_ALIGN_CENTER,
                            DNT_FONT_STYLE_BOLD);

   textCharac = intWindow->getObjectsList()->insertRolBar(345,36,615,345,"");
   setCharacteristics();

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(6,346,21,364,
                                             fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(600,346,615,364,
                                             fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(22,346,599,364,1, 
                                                  racesOrder[curRace]->name.c_str());
   textName->setFont(DNT_FONT_ARIAL,12,DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_BOLD);

   intWindow->getObjectsList()->insertTextBox(6,365,615,394,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(541,370,611,389,
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
   textCharac->setText("");
   textCharac->addText(string(gettext("Race Modifiers")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<racesOrder[curRace]->totalModifiers; i++)
   {
      textCharac->addText(racesOrder[curRace]->raceModifiers[i].description + "||");
   }

   if(racesOrder[curRace]->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75, 147, 207);
   }

   textCharac->addText(string(gettext("Race Feats")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);

   for(i=0; i<racesOrder[curRace]->totalFeats; i++)
   {
      textCharac->addText(racesOrder[curRace]->raceFeats[i] + "|");
   }

   if(racesOrder[curRace]->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Feats.")) + "|",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   /* Race Skills */
   skill* skTmp;
   textCharac->addText(string("|") + string(gettext("Race Skills")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<racesOrder[curRace]->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(racesOrder[curRace]->
                                                                 raceSkills[i]);
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
   if(eventInfo == PRESSED_BUTTON)
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
         raceImage->set(NULL);
         *choosedRace = racesOrder[curRace];
         inter->closeWindow(intWindow);
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         return(RACEW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         raceImage->set(NULL); //to not delete race images
         inter->closeWindow(intWindow);
         *choosedRace = NULL;
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         return(RACEW_CANCEL);
      }
   }
   return(RACEW_OTHER);
}

