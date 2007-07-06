#include "racewindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
raceWindow::raceWindow(races* rc, skills* sk, interface* inter,
                       race** retRace)
{
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
   intWindow = inter->insertWindow(90,100,710,499,
                                language.RACEW_TITLE.c_str());
   /* Race Image */
   raceImage = intWindow->getObjectsList()->insertPicture(7,20,0,0,NULL);   
   raceImage->set(actualRace->image);

   /* Race Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(71,20,342,35,1,
                                            language.RACEW_DESCRIPTION.c_str());
   textDescTitle->setFont(FHELVETICA,1,ALIGN_LEFT);
   
   textDesc = intWindow->getObjectsList()->insertRolBar(71,36,342,345,
              (actualRace->citation + "||" + actualRace->description).c_str(),
              intWindow->getSurface());

   /* Race Characteristics */
   textCharacTitle = intWindow->getObjectsList()->insertTextBox(343,20,613,35,1,
                                       language.RACEW_CHARACTERISTICS.c_str());
   textCharacTitle->setFont(FHELVETICA,1,ALIGN_LEFT);

   textCharac = intWindow->getObjectsList()->insertRolBar(343,36,613,345,
                                              getCharacteristics().c_str(),
                                              intWindow->getSurface());

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(71,346,86,364,"<",0);
   buttonNext = intWindow->getObjectsList()->insertButton(598,346,613,364,">",0);
   textName = intWindow->getObjectsList()->insertTextBox(87,346,597,364,1, 
                                                  actualRace->name.c_str());
   textName->setFont(FMINI,1,ALIGN_LEFT);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(543,370,613,389,
                                         language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,370,78,389,
                                               language.SKILL_CANCEL.c_str(),1);

   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                       getCharacteristics                         *
 ********************************************************************/
string raceWindow::getCharacteristics()
{
   int i;
   //char mod[5];
   string text = language.RACEW_MODIFIERS + "||";
   for(i=0; i<actualRace->totalModifiers; i++)
   {
      text += actualRace->raceModifiers[i].description + "||";
   }

   if(actualRace->totalModifiers == 0)
   {
      text += language.RACEW_NO_MODIFIERS + "||";
   }

   //TODO get race Name
   text += language.RACEW_FEATS + "||";
   for(i=0; i<actualRace->totalFeats; i++)
   {
      text += actualRace->raceFeats[i] + "|";
   }

   if(actualRace->totalFeats == 0)
   {
      text += language.RACEW_NO_FEATS + "|";
   }

   /* Race Skills */
   skill* skTmp;
   text += "|" + language.RACEW_SKILLS + "||";
   for(i=0; i<actualRace->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualRace->raceSkills[i]);
      if(skTmp)
      {
         text += skTmp->name + "|";
      }
      else
      {
         text += actualRace->raceSkills[i] + "|";
      }
   }

   if(actualRace->totalSkills == 0)
   {
      text += language.RACEW_NO_SKILLS;
   }

   return(text);
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
         textDesc->setText(actualRace->citation + "||" +
                           actualRace->description);
         textCharac->setText(getCharacteristics());
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

