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

#include "classwindow.h"
#include "../gui/farsoopts.h"
using namespace std;

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
classWindow::classWindow(guiInterface* inter, classe** retClass)
{
   farsoOptions fopt;
   classes cls;
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   int i;

   choosedClass = retClass;
   curClass = -1;

   /* Alphabetical Order Races */
   totalClasses = cls.getTotalClasses();
   classesOrder = new classe*[totalClasses];
   for(i = 0; i < totalClasses; i++)
   {
      classesOrder[i] = cls.getClassByInteger(i);
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
   intWindow = inter->insertWindow(centerX-175,centerY-200,
                                   centerX+175,centerY+200,
                                   gettext("Class"));

   /* Class Image */
   intWindow->getObjectsList()->insertTextBox(5,18,73,364,2,"");
   classImage = intWindow->getObjectsList()->insertPicture(7,20,0,0,NULL);   
   classImage->setSurfaceDeletion(false);
   classImage->set(classesOrder[curClass]->image);

   /* TabBox */
   tabBox *tab = (tabBox*)intWindow->getObjectsList()->defineTabBox(74,18,
                                                                    345,345);
   guiList *listChar = tab->insertOption(gettext("Characteristics"));
   guiList *listDesc = tab->insertOption(gettext("Description"));

   /* Class Description */
   textDesc = listDesc->insertRolBar(77,39,342,341,"");
   setDescription();
   
   /* Class Characteristics */
   textCharac = listChar->insertRolBar(77,39,342,341,"");
   setCharacteristics();

   /* Name and Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(74,346,89,364,
                                                  fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(330,346,345,364,
                                                  fnt.createUnicode(0x25BA),0);
   textName = intWindow->getObjectsList()->insertTextBox(90,346,329,364,1, 
                                          classesOrder[curClass]->name.c_str());
   textName->setFont(fopt.getDefaultFont(),10,DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_BOLD);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(271,370,341,389,
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
   farsoOptions fopt;
   int i;
   char tmp[1024];
   char c;
   skillDefinition* skTmp;
   skillsDefinitions skDefs;
   featsList curFeats;

   sprintf(tmp,": d%d||",classesOrder[curClass]->lifeDiceID);


   textCharac->setText("|");
   textCharac->addText(string(gettext("Life Dice")) + tmp, 
                       fopt.getDefaultFont(),
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       240, 120, 0);

   textCharac->addText(string(gettext("Skill Points")) + "||",
                       fopt.getDefaultFont(), 12, DNT_FONT_ALIGN_CENTER,
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
   skTmp =skDefs.getSkillDefinition(classesOrder[curClass]->firstLevelSP.attID);
   if(skTmp)
   {
      sprintf(tmp,": ( %d %c %s) x %d|",
              classesOrder[curClass]->firstLevelSP.sum, 
              c, skTmp->name.c_str(), 
              classesOrder[curClass]->firstLevelSP.mult);
   }
   else
   {
      sprintf(tmp,": ( %d %c %s) x %d|",
              classesOrder[curClass]->firstLevelSP.sum, 
              c, classesOrder[curClass]->firstLevelSP.attID.c_str(), 
              classesOrder[curClass]->firstLevelSP.mult);
   }
   textCharac->addText(string(gettext("First Level")) + tmp, 
                       fopt.getDefaultFont(),
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       220,0,0);

   /* Other Levels Points */
   if(classesOrder[curClass]->otherLevelsSP.signal == SIGNAL_DEC)
   {
      c = '-';
   }
   else
   {
      c = '+';
   }
   skTmp=skDefs.getSkillDefinition(classesOrder[curClass]->otherLevelsSP.attID);
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
   textCharac->addText(string(gettext("Other Levels")) + tmp, 
                       fopt.getDefaultFont(),
                       10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                       220,0,0);

   /* Modifiers */
   textCharac->addText(string(gettext("Class Modifiers")) + "||", 
                       fopt.getDefaultFont(),
                       12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalModifiers; i++)
   {
      textCharac->addText(classesOrder[curClass]->classModifiers[i].description
                          + "||");
   }

   if(classesOrder[curClass]->totalModifiers == 0)
   {
      textCharac->addText(string(gettext("No Modifiers.")) + "||",
                          fopt.getDefaultFont(), 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   /* Feats */
   featDescription* fDesc = NULL;
   textCharac->addText(string(gettext("Class Talents")) + "||",
                       fopt.getDefaultFont(), 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalFeats; i++)
   {
      fDesc = curFeats.featByString(classesOrder[curClass]->classFeats[i]);
      if(fDesc)
      {
         textCharac->addText(fDesc->name + "|");
      }
      else
      {
         textCharac->addText(classesOrder[curClass]->classFeats[i] + "|");
      }
   }

   if(classesOrder[curClass]->totalFeats == 0)
   {
      textCharac->addText(string(gettext("No Talents.")) + "||",
                          fopt.getDefaultFont(), 10, DNT_FONT_ALIGN_LEFT,
                          DNT_FONT_STYLE_NORMAL,
                          75,147,207);
   }

   /* Skills */
   textCharac->addText("|");
   textCharac->addText(string(gettext("Class Skills")) + "||",
                       fopt.getDefaultFont(), 12, DNT_FONT_ALIGN_CENTER,
                       DNT_FONT_STYLE_UNDERLINE,
                       86, 161, 32);
   for(i=0; i<classesOrder[curClass]->totalSkills; i++)
   {
      skTmp = skDefs.getSkillDefinition(classesOrder[curClass]->classSkills[i]);
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
                          fopt.getDefaultFont(), 10, DNT_FONT_ALIGN_LEFT,
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
   farsoOptions fopt;
   textDesc->setText("");
   textDesc->addText(classesOrder[curClass]->citation + "||", 
                     fopt.getDefaultFont(),
                     10, DNT_FONT_ALIGN_LEFT, DNT_FONT_STYLE_ITALIC,
                     220,0,0);
   textDesc->addText(classesOrder[curClass]->description);
   textDesc->setFirstLine(0);
}

/********************************************************************
 *                              treat                               *
 ********************************************************************/
int classWindow::treat(guiObject* object, int eventInfo, guiInterface* inter)
{
   if(eventInfo == FARSO_EVENT_PRESSED_BUTTON)
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
         *choosedClass = classesOrder[curClass];
         inter->closeWindow(intWindow);
         return(CLASSW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         inter->closeWindow(intWindow);
         *choosedClass = NULL;
         intWindow = NULL;
         return(CLASSW_CANCEL);
      }
   }
   return(CLASSW_OTHER);
}

