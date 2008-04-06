#include "racewindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
raceWindow::raceWindow(races* rc, skills* sk, interface* inter,
                       race** retRace)
{
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   externalSkills = sk;

   choosedRace = retRace;
   
   externalRaces = rc;
   if(*choosedRace != NULL)
   {
      actualRace = *choosedRace;
   }
   else
   {
      actualRace = externalRaces->getRaceByInteger(0);
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-310,centerY-200,
                                   centerX+310,centerY+200,
                                   gettext("Race"));

   /* Race Image */
   intWindow->getObjectsList()->insertTextBox(278,18,344,345,2,"");
   raceImage = intWindow->getObjectsList()->insertPicture(279,152,0,0,NULL);   
   raceImage->set(actualRace->image);

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
                                                  actualRace->name.c_str());
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
 *                         setDescription                           *
 ********************************************************************/
void raceWindow::setDescription()
{
   textDesc->setText("");
   textDesc->addText(actualRace->citation + "||", DNT_FONT_ARIAL,
                     10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                     147,18,18);
   textDesc->addText(actualRace->description);
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
                       33, 65, 10);
   for(i=0; i<actualRace->totalModifiers; i++)
   {
      textCharac->addText(actualRace->raceModifiers[i].description + "||");
   }

   if(actualRace->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   textCharac->addText(string(gettext("Race Feats")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);

   for(i=0; i<actualRace->totalFeats; i++)
   {
      textCharac->addText(actualRace->raceFeats[i] + "|");
   }

   if(actualRace->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Feats.")) + "|",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   /* Race Skills */
   skill* skTmp;
   textCharac->addText(string("|") + string(gettext("Race Skills")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);
   for(i=0; i<actualRace->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualRace->raceSkills[i]);
      if(skTmp)
      {
         textCharac->addText(skTmp->name + "|");
      }
      else
      {
         textCharac->addText(actualRace->raceSkills[i] + "|");
      }
   }

   if(actualRace->totalSkills == 0)
   {
      textCharac->addText(gettext("No Skills."),
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   textCharac->setFirstLine(0);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int raceWindow::treat(guiObject* object, int eventInfo, 
                      interface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      {
         if(object == (guiObject*) buttonNext)
         {
            actualRace = actualRace->next;
         }
         else
         {
            actualRace = actualRace->previous;
         }
         textName->setText(actualRace->name);
         setDescription();
         setCharacteristics();
         raceImage->set(actualRace->image);
         intWindow->draw(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         raceImage->set(NULL);
         *choosedRace = actualRace;
         inter->closeWindow(intWindow);
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         raceImage->set(NULL); //to not delete race images
         inter->closeWindow(intWindow);
         *choosedRace = NULL;
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(RACEW_CANCEL);
      }
   }
   return(RACEW_OTHER);
}

