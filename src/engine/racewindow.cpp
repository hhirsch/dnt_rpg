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
   intWindow = inter->insertWindow(90,100,710,499, gettext("Race"));

   /* Race Image */
   intWindow->getObjectsList()->insertTextBox(278,18,344,345,2,"");
   raceImage = intWindow->getObjectsList()->insertPicture(279,152,0,0,NULL);   
   raceImage->set(actualRace->image);

   /* Race Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(6,18,277,35,1,
                                            gettext("Race Description"));
   textDescTitle->setFont(FMINI,2,ALIGN_CENTER);
   
   textDesc = intWindow->getObjectsList()->insertRolBar(6,36,277,345,
              (actualRace->citation + "||" + actualRace->description).c_str(),
              intWindow->getSurface());

   /* Race Characteristics */
   textCharacTitle = intWindow->getObjectsList()->insertTextBox(345,18,615,35,1,
                                       gettext("Race Characteristcs"));
   textCharacTitle->setFont(FMINI,2,ALIGN_CENTER);

   textCharac = intWindow->getObjectsList()->insertRolBar(345,36,615,345,
                                              getCharacteristics().c_str(),
                                              intWindow->getSurface());

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(6,346,21,364,
                                                              "<",0);
   buttonNext = intWindow->getObjectsList()->insertButton(600,346,615,364,
                                                          ">",0);
   textName = intWindow->getObjectsList()->insertTextBox(22,346,599,364,1, 
                                                  actualRace->name.c_str());
   textName->setFont(FMINI,1,ALIGN_CENTER);

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
 *                       getCharacteristics                         *
 ********************************************************************/
string raceWindow::getCharacteristics()
{
   int i;
   //char mod[5];
   string text = gettext("Race Modifiers||");
   for(i=0; i<actualRace->totalModifiers; i++)
   {
      text += actualRace->raceModifiers[i].description + "||";
   }

   if(actualRace->totalModifiers == 0)
   {
      text += gettext("No Modifiers.||");
   }

   //TODO get race Name
   text += gettext("Race Feats||");
   for(i=0; i<actualRace->totalFeats; i++)
   {
      text += actualRace->raceFeats[i] + "|";
   }

   if(actualRace->totalFeats == 0)
   {
      text += gettext("No Feats.|");
   }

   /* Race Skills */
   skill* skTmp;
   text += gettext("|Race Skills||");
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
      text += gettext("No Skills.");
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

