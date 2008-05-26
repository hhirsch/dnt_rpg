/* DccNiTghtmare is in Public Domain. Do whatever you want with this code */

#include "classwindow.h"

/***********************************************************************
 *                          cmpClassFunction                           *
 ***********************************************************************/
static int cmpClassFunction(const void *p1,  const void *p2)
{
   /* Compare Function for the quicksort */
   classe** c1 = (classe**) p1;
   classe** c2 = (classe**) p2;

   return((*c1)->name.compare((*c2)->name));
}

/********************************************************************
 *                           Constructor                            *
 ********************************************************************/
classWindow::classWindow(classes* cls, skills* sk, guiInterface* inter, 
                         classe** retClass)
{
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   int i;

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalSkills = sk;
   
   choosedClass = retClass;
   curClass = -1;

   /* Alphabetical Order Races */
   totalClasses = cls->getTotalClasses();
   classesOrder = new classe*[totalClasses];
   for(i = 0; i < totalClasses; i++)
   {
      classesOrder[i] = cls->getClassByInteger(i);
   }
   qsort(&classesOrder[0], totalClasses, sizeof(classe**), cmpClassFunction);

   /* Define the previous one, if needed */
   if(*choosedClass != NULL)
   {
      /* Find it on vector */
      for(i = 0; ((i < totalClasses) && (curClass == -1)); i++)
      {
         if((*choosedClass)->name == classesOrder[i]->name)
         {
            curClass = i;
         }
      }
      if(curClass == -1)
      {
         /* Not found */
         curClass = 0;
      }
   }
   else
   {
      curClass = 0;
   }
   
   /* create intWindow */
   intWindow = inter->insertWindow(centerX-310,centerY-200,
                                   centerX+310,centerY+200,
                                   gettext("Class"));

   /* Class Image */
   intWindow->getObjectsList()->insertTextBox(5,18,73,364,2,"");
   classImage = intWindow->getObjectsList()->insertPicture(7,20,0,0,NULL);   
   classImage->set(classesOrder[curClass]->image);

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
                                          classesOrder[curClass]->name.c_str());
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
 *                           Destructor                             *
 ********************************************************************/
classWindow::~classWindow()
{
   delete[]classesOrder;
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
   sprintf(tmp,": d%d||",classesOrder[curClass]->lifeDiceID);


   textCharac->setText("|");
   textCharac->addText(string(gettext("Life Dice")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       157,18,18);

   textCharac->addText(string(gettext("Skill Points")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);

   /* Skill Points for First Level */
   if(classesOrder[curClass]->firstLevelSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(classesOrder[curClass]->firstLevelSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s) x %d|",classesOrder[curClass]->firstLevelSP.sum, 
              c, skTmp->name.c_str(), 
              classesOrder[curClass]->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s) x %d|",classesOrder[curClass]->firstLevelSP.sum, 
              c, classesOrder[curClass]->firstLevelSP.attID.c_str(), 
              classesOrder[curClass]->firstLevelSP.mult);
   }
   textCharac->addText(string(gettext("First Level")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       147,18,18);

   /* Other Levels Points */
   if(classesOrder[curClass]->otherLevelsSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp = externalSkills->getSkillByString(classesOrder[curClass]->otherLevelsSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s) x %d||",
              classesOrder[curClass]->otherLevelsSP.sum,
              c, skTmp->name.c_str(), 
              classesOrder[curClass]->otherLevelsSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s) x %d||",
              classesOrder[curClass]->otherLevelsSP.sum,
              c, classesOrder[curClass]->otherLevelsSP.attID.c_str(), 
              classesOrder[curClass]->otherLevelsSP.mult);
   }
   textCharac->addText(string(gettext("Other Levels")) + tmp, DNT_FONT_ARIAL,
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       147,18,18);

   /* Modifiers */
   textCharac->addText(string(gettext("Class Modifiers")) + "||", 
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalModifiers; i++)
   {
      textCharac->addText(classesOrder[curClass]->classModifiers[i].description + "||");
   }

   if(classesOrder[curClass]->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   //TODO get Feat Name
   textCharac->addText(string(gettext("Class Feats")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalFeats; i++)
   {
      textCharac->addText(classesOrder[curClass]->classFeats[i] + "|");
   }

   if(classesOrder[curClass]->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Feats.")) + "||",
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   /* Skills */
   textCharac->addText("|");
   textCharac->addText(string(gettext("Class Skills")) + "||",
                       DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalSkills; i++)
   {
      skTmp = externalSkills->getSkillByString(classesOrder[curClass]->classSkills[i]);
      if(skTmp != NULL)
      {
         textCharac->addText(skTmp->name + "|");
      }
      else
      {
         textCharac->addText(classesOrder[curClass]->classSkills[i] + "|");
      }
   }

   if(classesOrder[curClass]->totalSkills == 0)
   {
      textCharac->addText(gettext("No Skills."), 
                          DNT_FONT_ARIAL, 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   textCharac->setFirstLine(0);
}

/********************************************************************
 *                         setDescription                           *
 ********************************************************************/
void classWindow::setDescription()
{
   textDesc->setText("");
   textDesc->addText(classesOrder[curClass]->citation + "||", DNT_FONT_ARIAL,
                     10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                     147,18,18);
   textDesc->addText(classesOrder[curClass]->description);
   textDesc->setFirstLine(0);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int classWindow::treat(guiObject* object, int eventInfo, guiInterface* inter)
{
   if(eventInfo == PRESSED_BUTTON)
   {
      if( (object == (guiObject*) buttonNext) || 
          (object == (guiObject*) buttonPrevious))
      { 
         if(object == (guiObject*) buttonNext)
         {
            curClass = (curClass + 1) % totalClasses;
         }
         else
         {
            curClass--;
            if(curClass < 0)
            {
              curClass = totalClasses-1;
            }
         }

         textName->setText(classesOrder[curClass]->name);
         setDescription();
         setCharacteristics();
         classImage->set(classesOrder[curClass]->image);
         intWindow->draw(0,0);
      }
      else if(object == (guiObject*) buttonConfirm)
      {
         classImage->set(NULL);
         *choosedClass = classesOrder[curClass];
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

