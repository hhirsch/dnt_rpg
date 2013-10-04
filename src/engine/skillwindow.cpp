/* 
  DNT: a satirical post-apocalyptical RPG.
  Copyright (C) 2005-2013 DNTeam <dnt@dnteam.org>
 
  This file is part of DNT.
 
  DNT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  DNT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with DNT.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
using namespace std;

#include "skillwindow.h"
#include "../classes/defs.h"
#include "../gui/farsoopts.h"
#include "../lang/translate.h"

/***********************************************************************
 *                          cmpSkillFunction                           *
 ***********************************************************************/
static int cmpSkillFunction(const void *p1,  const void *p2)
{
   /* Compare Function for the quicksort */
   skill** s1 = (skill**) p1;
   skill** s2 = (skill**) p2;

   return((*s1)->definition->name.compare((*s2)->definition->name));
}

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
skillWindow::skillWindow(skills* savSkill, Farso::GuiInterface* inter,
                         int actualLevel, bool readOnlyMode)
{
   Farso::Options opt;
   Farso::Font fnt;
   int centerY = Farso::SCREEN_Y / 2;
   int centerX = Farso::SCREEN_X / 2;

   char tmp[5];
   string saux;
   int i;

   charLevel = actualLevel;
   saveSkill = savSkill;

   curSkill = 0; 
   avaiblePoints = savSkill->getAvaiblePoints();

   readOnly = readOnlyMode;

   /* Alphabetical Order Skills */
   totalSkills = savSkill->getTotalSkills()-ATT_SKILL_FIRST;
   skillsOrder = new skill*[totalSkills];
   for(i = 0; i < totalSkills; i++)
   {
      skillsOrder[i] = savSkill->getSkillByInt(i+ATT_SKILL_FIRST);
      skillsOrder[i]->prevPoints = skillsOrder[i]->points;
   }
   qsort(&skillsOrder[0], totalSkills, sizeof(skill**), cmpSkillFunction);

   /* Create Skill Window */
   intWindow = inter->insertWindow(centerX-132,centerY-133,
                                   centerX+132,centerY+133,
                                   gettext("Skills"));

   /* Free Points */
   if(!readOnly)
   {
      sprintf(tmp,"%d",avaiblePoints);
      saux = tmp;
      intWindow->getObjectsList()->insertTextBox(8,20,125,33,0,
            gettext("Total Free Points:"));
      txtAvaiblePoints = intWindow->getObjectsList()->insertTextBox(127,20,162,
            33,3,saux.c_str());
   }
   else
   {
      txtAvaiblePoints = NULL;
   }

   /* Skill Description */
   skillDefinition* sk = skillsOrder[curSkill]->definition;
   desc = intWindow->getObjectsList()->insertRolBar(8,38,256,170,
                                                    sk->description.c_str());

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(8,171,256,234,2,"");
   intWindow->getObjectsList()->insertTextBox(8,18,256,37,2,"");
   intWindow->getObjectsList()->insertTextBox(8,235,256,260,2,"");
 
   /* Skill Name & Selectors */
   buttonPrevious = intWindow->getObjectsList()->insertButton(52,175,66,193,
                                              fnt.createUnicode(0x25C4),0);
   buttonNext = intWindow->getObjectsList()->insertButton(234,175,248,193,
                                              fnt.createUnicode(0x25BA),0);
   skillName = intWindow->getObjectsList()->insertTextBox(67,175,233,193,1,
                                                          sk->name.c_str());
   skillName->setFont(opt.getDefaultFont(), 10, Farso::Font::ALIGN_CENTER,
                      Farso::Font::STYLE_ITALIC);

   /* Skill Image */
   skFig = intWindow->getObjectsList()->insertPicture(13,175,0,0,NULL);
   skFig->setSurfaceDeletion(false);
   skFig->set(sk->image);
 
   /* Skill Points */
   intWindow->getObjectsList()->insertTextBox(52,200,101,214,0,
                                              gettext("Points:"));
   sprintf(tmp,"%d",skillsOrder[curSkill]->points);
   saux = tmp;
   txtPoints = intWindow->getObjectsList()->insertTextBox(111,198,133,216,1,
                                                   saux.c_str());
   txtPoints->setFont(opt.getDefaultFont(), 10, Farso::Font::ALIGN_CENTER,
                      Farso::Font::STYLE_NORMAL);
   txtPoints->setColor(255,255,255);

   /* Show Only Class/Race Skills cxSel */
   cxOnlyClass = intWindow->getObjectsList()->insertCxSel(52,220,false);
   intWindow->getObjectsList()->insertTextBox(63,217,248,226,0,
                                        gettext("Only show race/class skills"));

   if(!readOnly)
   {
      buttonSum = intWindow->getObjectsList()->insertButton(134,198,144,216,
            fnt.createUnicode(0x25BA),0);
      buttonSum->defineFont(opt.getDefaultFont(), 9);
      buttonDec = intWindow->getObjectsList()->insertButton(100,198,110,216,
            fnt.createUnicode(0x25C4),0);
      buttonDec->defineFont(opt.getDefaultFont(), 9);
   }
   else
   {
      buttonSum = NULL;
      buttonDec = NULL;
   }

   /* Skill Costs */
   intWindow->getObjectsList()->insertTextBox(160,200,215,214,0,
                                       gettext("Cost"));
   sprintf(tmp,"%d",skillsOrder[curSkill]->mod);
   saux = tmp;
   txtCosts = intWindow->getObjectsList()->insertTextBox(216,200,256,214,3,
                                                  saux.c_str());

   if(!readOnly)
   {
      /* Confirm Button */
      buttonConfirm = intWindow->getObjectsList()->insertButton(178,238,248,257,
            gettext("Confirm"),1);
      /* Cancel Button */
      buttonCancel = intWindow->getObjectsList()->insertButton(13,238,83,257,
            gettext("Cancel"),1);
   }
   else
   {
      buttonConfirm = NULL;
      /* Cancel Button, with close text at read-only */
      buttonCancel = intWindow->getObjectsList()->insertButton(92,238,163,257,
            gettext("Close"),1);
   }

   
   
   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
   updateSkillInfo();
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
skillWindow::~skillWindow()
{
   delete[] skillsOrder;
}

/**************************************************************
 *                           isOpened                         *
 **************************************************************/
bool skillWindow::isOpened()
{
   return(intWindow != NULL);
}

/**************************************************************
 *                             close                          *
 **************************************************************/
void skillWindow::close(Farso::GuiInterface* inter)
{
   if(isOpened())
   {
      inter->closeWindow(intWindow);
      intWindow = NULL;
   }
}

/**************************************************************
 *                         updateSkillInfo                    *
 **************************************************************/
void skillWindow::updateSkillInfo()
{
   Farso::Options opt;
   string saux;  
   skill* att;
   char tmp[5];

   /* Set Point Color */
   setColors();

   /* Name and Description */
   skillDefinition* sk = skillsOrder[curSkill]->definition;
   skillName->setText(sk->name.c_str());
   desc->setText(sk->description.c_str());
   desc->addText("|");
   if(sk->baseAttribute >= 0)
   {
      att = saveSkill->getSkillByInt(sk->baseAttribute);
   }
   else
   {
      att = saveSkill->getSkillByInt(-sk->baseAttribute);
   }
   saux = gettext("Base Attribute:") + string(" ");
   if(att)
   {
      if(sk->baseAttribute < 0)
      {
         saux += "-";
      }
      saux += att->definition->name;
   }
   else
   {
      saux += gettext("Unknow!");
   }
   desc->addText(saux, opt.getDefaultFont(), 10, Farso::Font::ALIGN_LEFT,
         Farso::Font::STYLE_NORMAL, 86, 161, 3);

   /* Set Current Image */
   skFig->set(sk->image);

   /* Define Skill Points */
   sprintf(tmp,"%d",skillsOrder[curSkill]->points);
   saux = tmp;
   txtPoints->setText(saux);
   sprintf(tmp,"%d",skillsOrder[curSkill]->mod);
   saux = tmp;
   txtCosts->setText(saux);

   /* Define Available Points */
   if(!readOnly)
   {
      sprintf(tmp,"%d",avaiblePoints);
      saux = tmp;
      txtAvaiblePoints->setText(saux);
   }
   intWindow->draw(0,0);
}

/**************************************************************
 *                           setColors                        *
 **************************************************************/
void skillWindow::setColors()
{
   /* Define Skill Points color, due to changes */
   if( skillsOrder[curSkill]->points > 
       skillsOrder[curSkill]->prevPoints )
   {
      txtPoints->setColor(13, 250, 85);
   }
   else
   {
      txtPoints->setColor(255,255,255);
   }
}

/**************************************************************
 *                     previousSkill                          *
 **************************************************************/
void skillWindow::previousSkill()
{
   if(curSkill > 0)
   {
      curSkill--;
   }
   else
   {
      curSkill = totalSkills-1;
   }
}

/**************************************************************
 *                        nextSkill                           *
 **************************************************************/
void skillWindow::nextSkill()
{
   curSkill = (curSkill+1) % totalSkills;
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int skillWindow::treat(Farso::GuiObject* object, int eventInfo, Farso::GuiInterface* inter)
{
   /* No need to verify NULL events (should not occurs, anyway) */
   if(object == NULL)
   {
      return(SKILLW_OTHER);
   }

   /* Verify cxSel Events */
   if(eventInfo == Farso::EVENT_MODIFIED_CX_SEL)
   {
      if(object == (Farso::GuiObject*)cxOnlyClass)
      {
         if( (cxOnlyClass->isSelected()) && (skillsOrder[curSkill]->mod != 1))
         {
            while(skillsOrder[curSkill]->mod != 1)
            {
               nextSkill();
            }
            updateSkillInfo();
         }
      }
   }

   /* Verify Button Events */
   else if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
   {
      if( (object == (Farso::GuiObject*) buttonSum) && (!readOnly))
      {
         /* Try to sum a point to the skill */
         if( ( avaiblePoints - skillsOrder[curSkill]->mod >=0 ) && 
             (skillsOrder[curSkill]->points+1 <= charLevel+3) )
         {
             skillsOrder[curSkill]->points++;
             avaiblePoints -= skillsOrder[curSkill]->mod;
             updateSkillInfo();
         }
      }

      /* Try to dec a point from the skill */
      else if( (object == (Farso::GuiObject*) buttonDec) && (!readOnly) )
      {
         if(skillsOrder[curSkill]->points - 1 >= 
            skillsOrder[curSkill]->prevPoints)
         {
            skillsOrder[curSkill]->points--;
            avaiblePoints += skillsOrder[curSkill]->mod;
            updateSkillInfo();
         }
      }

      /* Show next skill info */
      else if(object == (Farso::GuiObject*) buttonNext)
      {
         nextSkill();
         if(cxOnlyClass->isSelected())
         {
            while(skillsOrder[curSkill]->mod != 1)
            {
               nextSkill();
            }
         }
         updateSkillInfo();
      }

      /* Show Previous skill info */
      else if(object == (Farso::GuiObject*) buttonPrevious)
      {
         previousSkill();
         if(cxOnlyClass->isSelected())
         {
            while(skillsOrder[curSkill]->mod != 1)
            {
               previousSkill();
            }
         }
        
         updateSkillInfo();
      }

      /* Confirm all Changes done to the skills and exit */
      else if( (object == (Farso::GuiObject*) buttonConfirm) && (!readOnly) )
      {
         close(inter);
         saveSkill->setAvaiblePoints(avaiblePoints);
         return(SKILLW_CONFIRM);
      }

      /* Cancel all changes done to skills and exit */
      else if(object == (Farso::GuiObject*) buttonCancel) 
      {
         /* Undo */
         int aux;
         for(aux = 0; aux < totalSkills; aux++)
         { 
            skillsOrder[aux]->points = skillsOrder[aux]->prevPoints;
         }
         close(inter);
         return(SKILLW_CANCEL);
      }
   }
   
   return(SKILLW_OTHER);
}

