/* DccNiTghtmare is in Public Domain. Do whatever you want with this code */

#include "classwindow.h"

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
classWindow::classWindow(classes* cls, skills* sk, interface* inter, 
                         classe** retClass)
{
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalSkills = sk;
   
   externalClasses = cls;
   choosedClass = retClass;

   if(*choosedClass != NULL)
   {
      actualClass = *choosedClass;
   }
   else
   {
      actualClass = externalClasses->getClassByInteger(0);
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-310,centerY-200,
                                   centerX+310,centerY+200,
                                   gettext("Class"));

   /* Class Image */
   intWindow->getObjectsList()->insertTextBox(5,18,73,364,2,"");
   classImage = intWindow->getObjectsList()->insertPicture(7,20,0,0,NULL);   
   classImage->set(actualClass->image);

   /* Class Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(74,18,345,35,1,
                                           gettext("Class Description"));
   textDescTitle->setFont(DNT_FONT_TIMES, 12, DNT_FONT_ALIGN_CENTER);
   
   textDesc = intWindow->getObjectsList()->insertRolBar(74,36,345,345,
             (actualClass->citation + "||" + actualClass->description).c_str(),
             intWindow->getSurface());

   /* Race Characteristics */
   textCharacTitle = intWindow->getObjectsList()->insertTextBox(346,18,615,35,1,
                                       gettext("Class Characteristics"));
   textCharacTitle->setFont(DNT_FONT_TIMES, 12, DNT_FONT_ALIGN_CENTER);

   textCharac = intWindow->getObjectsList()->insertRolBar(346,36,615,345,
                                              getCharacteristics().c_str(),
                                              intWindow->getSurface());

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(74,346,89,364,
                                                  fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(600,346,615,364,
                                                  fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(90,346,599,364,1, 
                                                  actualClass->name.c_str());
   textName->setFont(DNT_FONT_ARIAL,12,DNT_FONT_ALIGN_LEFT);

   intWindow->getObjectsList()->insertTextBox(6,365,615,394,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(541,370,611,389,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(10,370,80,389,
                                               gettext("Cancel"),1);

   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,false,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                       getCharacteristics                         *
 ********************************************************************/
string classWindow::getCharacteristics()
{
   int i;
   char tmp[1024];
   char c;
   skill* skTmp;
   sprintf(tmp,": d%d||",actualClass->lifeDiceID);
   string text = gettext("Life Dice");
   text += tmp;

   text += gettext("Skill Points|");

   if(actualClass->firstLevelSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(actualClass->firstLevelSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, skTmp->name.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s)x%d|",actualClass->firstLevelSP.sum, 
              c, actualClass->firstLevelSP.attID.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   text += gettext("First Level");
   text += tmp;
   if(actualClass->otherLevelsSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(actualClass->otherLevelsSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, skTmp->name.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s)x%d||",
              actualClass->otherLevelsSP.sum,
              c, actualClass->otherLevelsSP.attID.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   text += gettext("Other Levels");
   text += tmp;
   text += gettext("Class Modifiers||");
   for(i=0; i<actualClass->totalModifiers; i++)
   {
      text += actualClass->classModifiers[i].description + "||";
   }

   if(actualClass->totalModifiers == 0)
   {
      text += gettext("No Modifiers||");
   }

   //TODO get Feat Name
   text += gettext("Class Feats||");
   for(i=0; i<actualClass->totalFeats; i++)
   {
      text += actualClass->classFeats[i] + "|";
   }

   if(actualClass->totalFeats == 0)
   {
      text += gettext("No Feats.|");
   }

   //Print Skills
   text += "|";
   text += gettext("Class Skills||");
   for(i=0; i<actualClass->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualClass->classSkills[i]);
      if(skTmp != NULL)
      {
         text += skTmp->name + "|";
      }
      else
      {
         text += actualClass->classSkills[i] + "|";
      }
   }

   if(actualClass->totalSkills == 0)
   {
      text += gettext("No Skills.");
   }

   return(text);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int classWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      {
         if(object == (guiObject*) buttonNext)
         {
            actualClass = actualClass->next;
         }
         else
         {
            actualClass = actualClass->previous;
         }
         textName->setText(actualClass->name);
         textDesc->setText(actualClass->citation + "||" +
                           actualClass->description);
         textCharac->setText(getCharacteristics());
         classImage->set(actualClass->image);
         intWindow->draw(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         classImage->set(NULL);
         *choosedClass = actualClass;
         inter->closeWindow(intWindow);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         classImage->set(NULL); //to not delete classes images
         inter->closeWindow(intWindow);
         *choosedClass = NULL;
         intWindow = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(CLASSW_CANCEL);
      }
   }
   return(CLASSW_OTHER);
}

