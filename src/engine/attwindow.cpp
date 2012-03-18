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

#include <iostream>
using namespace std;

#include "attwindow.h"
#include "../classes/skills.h"
#include "../classes/dices.h"
#include "../gui/messages.h"
#include "../etc/dirs.h"
#include "util.h"

/**************************************************************
 *                      Constructor                           *
 **************************************************************/
attWindow::attWindow(skills* savSkill, Farso::GuiInterface* inter,
                     int modifiers[6],bool usePreviousValues)
{
   dirs dir;
   Farso::Font fnt;
   int centerY = Farso::SCREEN_Y / 2;
   int centerX = Farso::SCREEN_X / 2;
   int i;
   string saux;
   char tmp[16];

   string fontArial = dir.getRealFile(DNT_FONT_ARIAL);
   string fontPalladio = dir.getRealFile(DNT_FONT_PALLADIO);
   string fontTimes = dir.getRealFile(DNT_FONT_TIMES);

   saveSkill = savSkill;

   /* clear internal informations */
   for( i = 0; i < 6; i++)
   {
      points[i] = 0;
      used[i] = false;
      attPointsIndex[i] = -1;
      rcModifiers[i] = modifiers[i];
   }

   /* create intWindow */
   intWindow = inter->insertWindow(centerX-263,centerY-139,
                                   centerX+263,centerY+139,
                                   gettext("Attributes"));

   /* roll and write all rolled dices to string */
   if(!usePreviousValues)
   {
      rollAllDices();
   }
   else
   {
      assignPreviousToDices();
   }
   saux = "";
   for(i = 0; i < 6; i++)
   {
      sprintf(tmp,"%.2d ", points[i]); 
      saux += tmp;
   }
   rolledPoints = intWindow->getObjectsList()->insertTextBox(8,19,243,32,0,
                                                             saux.c_str());
   rolledPoints->setFont(fontArial, 12, Farso::Font::ALIGN_CENTER);
   /* reroll button */
   rerollButton = intWindow->getObjectsList()->insertButton(244,17,334,35,
                                                  gettext("Roll"),1);
   /* clear button */
   clearButton = intWindow->getObjectsList()->insertButton(335,17,425,35,
                                                  gettext("Clear"),1);

   /* auto button */
   autoButton = intWindow->getObjectsList()->insertButton(426,17,516,35,
                                                   gettext("Auto"),1);

   /* Strenght */
   intWindow->getObjectsList()->insertTextBox(9,50,85,63,0,gettext("Strength"));
   attPoints[0] = intWindow->getObjectsList()->insertTextBox(103,48,122,65,
                                                             1,"");
   attPoints[0]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[0] = intWindow->getObjectsList()->insertButton(92,48,102,65,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonPrev[0]->defineFont(fontArial, 9);
   attButtonNext[0] = intWindow->getObjectsList()->insertButton(123,48,133,65,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonNext[0]->defineFont(fontArial, 9);
   attTotals[0] = intWindow->getObjectsList()->insertTextBox(170,50,195,65,0,
                                                             "");
   attTotals[0]->setFont(fontTimes, 12, 0);
   attMods[0] = intWindow->getObjectsList()->insertTextBox(196,47,226,62,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,41,0,0,
                               dir.getRealFile("skills/Img/forca.png").c_str());

   /* Dexterity */
   intWindow->getObjectsList()->insertTextBox(9,84,85,97,0,
                                              gettext("Dexterity"));
   attPoints[1] = intWindow->getObjectsList()->insertTextBox(103,82,122,99,
                                                             1,"");
   attPoints[1]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[1] = intWindow->getObjectsList()->insertButton(92,82,102,99,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonPrev[1]->defineFont(fontArial, 9);
   attButtonNext[1] = intWindow->getObjectsList()->insertButton(123,82,133,99,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonNext[1]->defineFont(fontArial, 9);
   attTotals[1] = intWindow->getObjectsList()->insertTextBox(170,84,195,99, 
                                                             0,"");
   attTotals[1]->setFont(fontTimes, 12, 0);
   attMods[1] = intWindow->getObjectsList()->insertTextBox(196,81,226,96,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,75,0,0,
                            dir.getRealFile("skills/Img/destreza.png").c_str());

   /* Constitution */
   intWindow->getObjectsList()->insertTextBox(9,120,87,133,0,
                                              gettext("Constitution"));
   attPoints[2] = intWindow->getObjectsList()->insertTextBox(103,118,122,135,1,
                                                             "");
   attPoints[2]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[2] = intWindow->getObjectsList()->insertButton(92,118,102,135,
                                                   fnt.createUnicode(0x25C4),0);
   attButtonNext[2] = intWindow->getObjectsList()->insertButton(123,118,133,135,
                                                   fnt.createUnicode(0x25BA),0);
   attButtonPrev[2]->defineFont(fontArial, 9);
   attButtonNext[2]->defineFont(fontArial, 9);
   attTotals[2] = intWindow->getObjectsList()->insertTextBox(170,120,195,135,
                                                             0,"");
   attTotals[2]->setFont(fontTimes, 12, 0);
   attMods[2] = intWindow->getObjectsList()->insertTextBox(196,117,226,132,0,
                                                           "");
   intWindow->getObjectsList()->insertPicture(135,109,0,0,
                        dir.getRealFile("skills/Img/constituicao.png").c_str());

   /* Intelligence */
   intWindow->getObjectsList()->insertTextBox(9,154,87,167,0,
                                              gettext("Intelligence"));
   attPoints[3] = intWindow->getObjectsList()->insertTextBox(103,152,122,169,
                                                             1,"");
   attPoints[3]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[3] = intWindow->getObjectsList()->insertButton(92,152,102,169,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[3] = intWindow->getObjectsList()->insertButton(123,152,
                                                                133,169,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[3]->defineFont(fontArial, 9);
   attButtonNext[3]->defineFont(fontArial, 9);
   attTotals[3] = intWindow->getObjectsList()->insertTextBox(170,154,195,169,
                                                             0,"");
   attTotals[3]->setFont(fontTimes, 12, 0);
   attMods[3] = intWindow->getObjectsList()->insertTextBox(196,151,226,166,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,143,0,0,
                        dir.getRealFile("skills/Img/inteligencia.png").c_str());

   /* Wisdow */
   intWindow->getObjectsList()->insertTextBox(9,188,87,201,0,
                                              gettext("Wisdom"));
   attPoints[4] = intWindow->getObjectsList()->insertTextBox(103,186,122,203,
                                                             1,"");
   attPoints[4]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[4] = intWindow->getObjectsList()->insertButton(92,186,102,203,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[4] = intWindow->getObjectsList()->insertButton(123,186,
                                                                133,203,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[4]->defineFont(fontArial, 9);
   attButtonNext[4]->defineFont(fontArial, 9);
   attTotals[4] = intWindow->getObjectsList()->insertTextBox(170,188,195,203,
                                                             0,"");
   attTotals[4]->setFont(fontTimes, 12, 0);
   attMods[4] = intWindow->getObjectsList()->insertTextBox(196,185,226,200,
                                                           0,"");
   intWindow->getObjectsList()->insertPicture(135,177,0,0,
                           dir.getRealFile("skills/Img/sabedoria.png").c_str());

   /* Charism */
   intWindow->getObjectsList()->insertTextBox(9,222,87,235,0,
                                              gettext("Charisma"));
   attPoints[5] = intWindow->getObjectsList()->insertTextBox(103,220,122,237,1,
                                                             "");
   attPoints[5]->setFont(fontPalladio, 10, Farso::Font::ALIGN_CENTER);
   attButtonPrev[5] = intWindow->getObjectsList()->insertButton(92,220,102,237,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[5] = intWindow->getObjectsList()->insertButton(123,220,
                                                                133,237,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[5]->defineFont(fontArial, 9);
   attButtonNext[5]->defineFont(fontArial, 9);
   attTotals[5] = intWindow->getObjectsList()->insertTextBox(170,221,195,237,
                                                             0,"");
   attTotals[5]->setFont(fontTimes, 12, 0);
   attMods[5] = intWindow->getObjectsList()->insertTextBox(196,219,226,234,
                                                           0,"");
   intWindow->getObjectsList()->insertPicture(135,211,0,0,
                             dir.getRealFile("skills/Img/carisma.png").c_str());
                                              

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(6,15,519,37,2,"");
   intWindow->getObjectsList()->insertTextBox(8,40,245,73,2,"");
   intWindow->getObjectsList()->insertTextBox(8,74,245,107,2,"");
   intWindow->getObjectsList()->insertTextBox(8,108,245,141,2,"");
   intWindow->getObjectsList()->insertTextBox(8,142,245,175,2,"");
   intWindow->getObjectsList()->insertTextBox(8,176,245,209,2,"");
   intWindow->getObjectsList()->insertTextBox(8,210,245,243,2,"");
   intWindow->getObjectsList()->insertTextBox(6,38,247,245,2,"");

   /* Attribute Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(248,38,519,53,1,
                                             gettext("Attribute Description"));
   textDescTitle->setFont(fontTimes,12,Farso::Font::ALIGN_CENTER,
                          Farso::Font::STYLE_BOLD);
   textDescTitle->setBackColor(110, 0, 0);

   textDesc = intWindow->getObjectsList()->insertRolBar(248,54,519,245,"");

   setDescription(1);

   intWindow->getObjectsList()->insertTextBox(6,246,519,273,2,"");

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(445,249,515,268,
                                              gettext("Confirm"),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(10,249,80,268,
                                                          gettext("Cancel"),1);

   /* Define Previous Values, if needed */
   if(usePreviousValues)
   {
      assignPreviousValues();
   }
   
   /* Open Attribute Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,true,false,false);
   inter->openWindow(intWindow);
}

/**************************************************************
 *                      setDescription                        *
 **************************************************************/
void attWindow::setDescription(int sk)
{
   dirs dir;
   /* Get the definition */
   skillsDefinitions defSkills;
   skillDefinition* skDef = defSkills.getSkillDefinition(sk); 

   /* Set it to the window */
   if(skDef != NULL)
   {
      textDesc->setText("");
      textDesc->addText(skDef->name + "||", 
            dir.getRealFile(DNT_FONT_ARIAL), 12, Farso::Font::ALIGN_CENTER,
            Farso::Font::STYLE_UNDERLINE, 86, 161, 32);
      textDesc->addText(skDef->description);
      textDesc->setFirstLine(0);
   }
}

/**************************************************************
 *                       rollDices                            *
 **************************************************************/
int attWindow::rollDices()
{
   int value[4];
   int minor = 0;
   int i;
   int ret = 0;
   for(i = 0; i < 4; i++)
   {
      srand(SDL_GetTicks());
      value[i] = (rand() % DICE_D6) + 1;
      SDL_Delay(10);
      if(value[i] < value[minor])
      {
         minor = i;
      }
   }
   for(i = 0; i < 4; i++)
   {
      if( i != minor)
      {
         ret += value[i];
      }
   }
   return(ret);
}

/**************************************************************
 *                   assignPreviousToDices                    *
 **************************************************************/
void attWindow::assignPreviousToDices()
{
   int i;
   skill* sk;

   for(i = 0; i < 6; i++)
   {
      sk = saveSkill->getSkillByInt(i+1);
      if(sk != NULL)
      {
         points[i] = sk->points;
         used[i] = true;
      }
   }
}

/**************************************************************
 *                    assignPreviousValues                    *
 **************************************************************/
void attWindow::assignPreviousValues()
{
   int i;
   char tmp[10];
   skill* sk;

   for(i = 0; i < 6; i++)
   {
      sk = saveSkill->getSkillByInt(i+1);

      if(sk != NULL)
      {
         sprintf(tmp,"%.2d", sk->points);
         attPoints[i]->setText(tmp);
         attPointsIndex[i] = i;
         assignAttMod(i);
      }
   }
}

/**************************************************************
 *                        autoAssign                          *
 **************************************************************/
void attWindow::autoAssign()
{
   int i;
   char tmp[10];
   for(i = 0; i < 6; i++)
   {
      nextAvaiblePoints(i);
      sprintf(tmp,"%.2d", points[attPointsIndex[i]]);
      attPoints[i]->setText(tmp);
      assignAttMod(i);
   }
   intWindow->draw(0,0);
}

/**************************************************************
 *                      rollAllDices                          *
 **************************************************************/
void attWindow::rollAllDices()
{
   int i;
   for(i = 0; i < 6; i++)
   {
      points[i] = rollDices();
   }
}

/**************************************************************
 *                     nextAvaiblePoints                      *
 **************************************************************/
int attWindow::nextAvaiblePoints(int att)
{
   int i = attPointsIndex[att];
   if(i < 0)
   {
      i = 0;
   }
   else
   {
      used[i] = false;
   }

   i++;
   if(i >= 6)
   {
      i = 0;
   }
   
   while(used[i]) 
   {
      i++;
      if(i >= 6)
      {
         i = 0;
      }
   }

   attPointsIndex[att] = i;
   used[i] = true;
   return(i);
}

/**************************************************************
 *                  previousAvaiblePoints                     *
 **************************************************************/
int attWindow::previousAvaiblePoints(int att)
{
   int i = attPointsIndex[att];
   if(i < 0)
   {
      i = 0;
   }
   else
   {
      used[i] = false;
   }

   i--;
   if(i < 0)
   {
      i = 5;
   }

   
   while(used[i])
   {
      i--;
      if(i < 0)
      {
         i = 5;
      }
   }

   attPointsIndex[att] = i;
   used[i] = true;
   return(i);
}

/**************************************************************
 *                          clear                             *
 **************************************************************/
void attWindow::clear()
{
   int i;
   for( i = 0; i < 6; i++)
   {
      used[i] = false;
      attPointsIndex[i] = -1;
      attPoints[i]->setText("");
      attTotals[i]->setText("");
      attMods[i]->setText("");
      intWindow->draw(0,0);
   }
}

/**************************************************************
 *                        allAssigned                         *
 **************************************************************/
bool attWindow::allAssigned()
{
   int i;
   for( i = 0; i < 6; i++)
   {
      if(!used[i])
      {
         return(false);
      }
   }
   return(true);
}

/**************************************************************
 *                         assignAttMod                       *
 **************************************************************/
int attWindow::assignAttMod(int att)
{
   dirs dir;
   char tmpMod[10];
   char tmpTotal[10];
   int attBonus = 0;
   string total;
   skill* sk = saveSkill->getSkillByInt(att+1);

   if(sk != NULL)
   {
      /* Get the assign value */
      sk->points = points[attPointsIndex[att]];

      /* Apply the modifiers */
      sk->points += rcModifiers[att];

      /* Get the bonus */
      attBonus = (int)floor((sk->points-10) / 2.0);

      sprintf(tmpTotal,"%.2d",sk->points);
      total += tmpTotal;
      sprintf(tmpMod,"%+.2d",attBonus);
   }

   /* Define Color */
   if(attBonus > 0)
   {
      attMods[att]->setColor(52,200,12);
   }
   else if(attBonus == 0)
   {
      attMods[att]->setColor(232,174,16);
   }
   else
   {
      attMods[att]->setColor(255,55,0);
   }

   attTotals[att]->setText(total);
   attMods[att]->setText(tmpMod);
   attMods[att]->setFont(dir.getRealFile(DNT_FONT_PALLADIO),12,0);

   return(attBonus);
}

/**************************************************************
 *                            treat                           *
 **************************************************************/
int attWindow::treat(Farso::GuiObject* object, int eventInfo, 
      Farso::GuiInterface* inter, GLdouble proj[16], GLdouble modl[16],
      GLint viewPort[4])
{
   char tmp[5];
   int i;

   if(eventInfo == Farso::EVENT_PRESSED_BUTTON)
   {
      /* Confirm */
      if(object == (Farso::GuiObject*) buttonConfirm)
      {
         if(allAssigned())
         {
            inter->closeWindow(intWindow);
            intWindow = NULL;
            return(ATTW_CONFIRM);
         }
         else
         {
            Farso::Warning warn;
            warn.show(gettext("Warning"),
                      gettext("You Need to Assign All Attributes."), inter);
            return(ATTW_OTHER);
         }
      }
      /* Cancel */
      else if(object == (Farso::GuiObject*) buttonCancel) 
      {
          inter->closeWindow(intWindow);
          intWindow = NULL;
         return(ATTW_CANCEL);
      }
      /* Reroll */
      else if(object == (Farso::GuiObject*) rerollButton)
      {
          clear();
          /* ReRoll All Dices */
          rollAllDices();
          rolledPoints->setText("");
          for(i = 0; i < 6; i++)
          {
              sprintf(tmp,"%.2d ", points[i]); 
              rolledPoints->setText(rolledPoints->getText() + tmp);
          }
          intWindow->draw(0,0);
      }
      /* Clear */
      else if(object == (Farso::GuiObject*) clearButton)
      {
         clear();
      }
      /* Auto */
      else if(object == (Farso::GuiObject*) autoButton)
      {
         autoAssign();
      }
      /* Assign Buttons */
      else
      {
         for(i=0; i<6; i++) 
         {
            if(object == (Farso::GuiObject*) attButtonNext[i])
            {
                nextAvaiblePoints(i);
                sprintf(tmp,"%.2d", points[attPointsIndex[i]]);
                attPoints[i]->setText(tmp);
                assignAttMod(i);
                setDescription(i+1);
                intWindow->draw(0,0);
            }
            else if(object == (Farso::GuiObject*) attButtonPrev[i])
            {
                previousAvaiblePoints(i);
                sprintf(tmp,"%.2d", points[attPointsIndex[i]]);
                attPoints[i]->setText(tmp);
                assignAttMod(i);
                setDescription(i+1);
                intWindow->draw(0,0);
            }
         }
      }
   }
   return(ATTW_OTHER);
}

