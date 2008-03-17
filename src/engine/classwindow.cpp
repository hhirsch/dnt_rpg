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
   textDescTitle->setFont(DNT_FONT_TIMES,12,DNT_FONT_ALIGN_CENTER,
                          DNT_FONT_STYLE_BOLD);
   
   textDesc = intWindow->getObjectsList()->insertRolBar(74,36,345,345,"");

   setDescription();
   
   /* Class Characteristics */
   textCharacTitle = intWindow->getObjectsList()->insertTextBox(346,18,615,35,1,
                                       gettext("Class Characteristics"));
   textCharacTitle->setFont(DNT_FONT_TIMES,12,DNT_FONT_ALIGN_CENTER,
                            DNT_FONT_STYLE_BOLD);

   textCharac = intWindow->getObjectsList()->insertRolBar(346,36,615,345,"");
   setCharacteristics();

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(74,346,89,364,
                                                  fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(600,346,615,364,
                                                  fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(90,346,599,364,1, 
                                                  actualClass->name.c_str());
   textName->setFont(DNT_FONT_ARIAL,12,DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_BOLD);

   intWindow->getObjectsList()->insertTextBox(6,365,615,394,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(541,370,611,389,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(10,370,80,389,
                                               gettext("Cancel"),1);

   /* Open Class Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/********************************************************************
 *                       setCharacteristics                         *
 ********************************************************************/
void classWindow::setCharacteristics()
{
   int i;
   char tmp[1024];
   char c;
   skill* skTmp;
   sprintf(tmp,": d%d||",actualClass->lifeDiceID);


   textCharac->setText("|");
   textCharac->addText(string(gettext("Life Dice")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       66,4,13);

   textCharac->addText(string(gettext("Skill Points")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);

   /* Skill Points for First Level */
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
      sprintf(tmp,": ( %d %c %s) x %d|",actualClass->firstLevelSP.sum, 
              c, skTmp->name.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s) x %d|",actualClass->firstLevelSP.sum, 
              c, actualClass->firstLevelSP.attID.c_str(), 
              actualClass->firstLevelSP.mult);
   }
   textCharac->addText(string(gettext("First Level")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       66,4,13);

   /* Other Levels Points */
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
      sprintf(tmp,": ( %d %c %s) x %d||",
              actualClass->otherLevelsSP.sum,
              c, skTmp->name.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s) x %d||",
              actualClass->otherLevelsSP.sum,
              c, actualClass->otherLevelsSP.attID.c_str(), 
              actualClass->otherLevelsSP.mult);
   }
   textCharac->addText(string(gettext("Other Levels")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       66,4,13);

   /* Modifiers */
   textCharac->addText(string(gettext("Class Modifiers")) + "||", 
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);
   for(i=0; i<actualClass->totalModifiers; i++)
   {
      textCharac->addText(actualClass->classModifiers[i].description + "||");
   }

   if(actualClass->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   //TODO get Feat Name
   textCharac->addText(string(gettext("Class Feats")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);
   for(i=0; i<actualClass->totalFeats; i++)
   {
      textCharac->addText(actualClass->classFeats[i] + "|");
   }

   if(actualClass->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Feats.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   /* Skills */
   textCharac->addText("|");
   textCharac->addText(string(gettext("Class Skills")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       33, 65, 10);
   for(i=0; i<actualClass->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(actualClass->classSkills[i]);
      if(skTmp != NULL)
      {
         textCharac->addText(skTmp->name + "|");
      }
      else
      {
         textCharac->addText(actualClass->classSkills[i] + "|");
      }
   }

   if(actualClass->totalSkills == 0)
   {
      textCharac->addText(gettext("No Skills."), 
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          27, 20, 99);
   }

   textCharac->setFirstLine(0);
}

/********************************************************************
 *                         setDescription                           *
 ********************************************************************/
void classWindow::setDescription()
{
   textDesc->setText("");
   textDesc->addText(actualClass->citation + "||", DNT_FONT_ARIAL,
                     10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                     66,4,13);
   textDesc->addText(actualClass->description);
   textDesc->setFirstLine(0);
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
         setDescription();
         setCharacteristics();
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

