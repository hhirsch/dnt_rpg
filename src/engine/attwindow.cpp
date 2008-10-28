/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include <iostream>
using namespace std;

#include "attwindow.h"
#include "../classes/skills.h"
#include "../classes/dices.h"
#include "../gui/messages.h"
#include "../etc/dirs.h"

/**************************************************************
 *                      Constructor                           *
 **************************************************************/
attWindow::attWindow(skills* sk, skills* savSkill, guiInterface* inter,
                     int modifiers[6],bool usePreviousValues)
{
   dirs dir;
   dntFont fnt;
   int centerY = SCREEN_Y / 2;
   int centerX = SCREEN_X / 2;
   int i;
   string saux;
   char tmp[16];

   externalSkill = sk;
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
   rolledPoints = intWindow->getObjectsList()->insertTextBox(8,19,320,32,0,
                                                             saux.c_str());
   rolledPoints->setFont(DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER);
   /* reroll button */
   rerollButton = intWindow->getObjectsList()->insertButton(323,17,386,34,
                                                  gettext("Roll"),0);
   /* clear button */
   clearButton = intWindow->getObjectsList()->insertButton(387,17,451,34,
                                                  gettext("Clear"),0);

   /* auto button */
   autoButton = intWindow->getObjectsList()->insertButton(452,17,516,34,
                                                   gettext("Auto"),0);

   /* Strenght */
   intWindow->getObjectsList()->insertTextBox(9,50,85,63,0,gettext("Strength"));
   attPoints[0] = intWindow->getObjectsList()->insertTextBox(103,48,122,65,
                                                             1,"");
   attPoints[0]->setFont(DNT_FONT_ARIAL, 11, 0);
   attButtonPrev[0] = intWindow->getObjectsList()->insertButton(92,48,102,65,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonPrev[0]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[0] = intWindow->getObjectsList()->insertButton(123,48,133,65,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonNext[0]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[0] = intWindow->getObjectsList()->insertTextBox(170,50,195,65,0,
                                                             "");
   attTotals[0]->setFont(DNT_FONT_TIMES, 12, 0);
   attMods[0] = intWindow->getObjectsList()->insertTextBox(196,47,226,62,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,41,0,0,
                               dir.getRealFile("skills/Img/forca.png").c_str());

   /* Dextery */
   intWindow->getObjectsList()->insertTextBox(9,84,85,97,0,
                                              gettext("Dexterity"));
   attPoints[1] = intWindow->getObjectsList()->insertTextBox(103,82,122,99,
                                                             1,"");
   attPoints[1]->setFont(DNT_FONT_ARIAL, 11, 0);
   attButtonPrev[1] = intWindow->getObjectsList()->insertButton(92,82,102,99,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonPrev[1]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[1] = intWindow->getObjectsList()->insertButton(123,82,133,99,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonNext[1]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[1] = intWindow->getObjectsList()->insertTextBox(170,84,195,99, 
                                                             0,"");
   attTotals[1]->setFont(DNT_FONT_TIMES, 12, 0);
   attMods[1] = intWindow->getObjectsList()->insertTextBox(196,81,226,96,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,75,0,0,
                            dir.getRealFile("skills/Img/destreza.png").c_str());

   /* Constitution */
   intWindow->getObjectsList()->insertTextBox(9,120,87,133,0,
                                              gettext("Constitution"));
   attPoints[2] = intWindow->getObjectsList()->insertTextBox(103,118,122,135,1,
                                                             "");
   attPoints[2]->setFont(DNT_FONT_ARIAL, 10, 0);
   attButtonPrev[2] = intWindow->getObjectsList()->insertButton(92,118,102,135,
                                                   fnt.createUnicode(0x25C4),0);
   attButtonNext[2] = intWindow->getObjectsList()->insertButton(123,118,133,135,
                                                   fnt.createUnicode(0x25BA),0);
   attButtonPrev[2]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[2]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[2] = intWindow->getObjectsList()->insertTextBox(170,120,195,135,
                                                             0,"");
   attTotals[2]->setFont(DNT_FONT_TIMES, 12, 0);
   attMods[2] = intWindow->getObjectsList()->insertTextBox(196,117,226,132,0,
                                                           "");
   intWindow->getObjectsList()->insertPicture(135,109,0,0,
                        dir.getRealFile("skills/Img/constituicao.png").c_str());

   /* Intelligence */
   intWindow->getObjectsList()->insertTextBox(9,154,87,167,0,
                                              gettext("Intelligence"));
   attPoints[3] = intWindow->getObjectsList()->insertTextBox(103,152,122,169,
                                                             1,"");
   attPoints[3]->setFont(DNT_FONT_ARIAL, 10, 0);
   attButtonPrev[3] = intWindow->getObjectsList()->insertButton(92,152,102,169,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[3] = intWindow->getObjectsList()->insertButton(123,152,
                                                                133,169,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[3]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[3]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[3] = intWindow->getObjectsList()->insertTextBox(170,154,195,169,
                                                             0,"");
   attTotals[3]->setFont(DNT_FONT_TIMES, 12, 0);
   attMods[3] = intWindow->getObjectsList()->insertTextBox(196,151,226,166,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(135,143,0,0,
                        dir.getRealFile("skills/Img/inteligencia.png").c_str());

   /* Wisdow */
   intWindow->getObjectsList()->insertTextBox(9,188,87,201,0,
                                              gettext("Wisdom"));
   attPoints[4] = intWindow->getObjectsList()->insertTextBox(103,186,122,203,
                                                             1,"");
   attPoints[4]->setFont(DNT_FONT_ARIAL, 10, 0);
   attButtonPrev[4] = intWindow->getObjectsList()->insertButton(92,186,102,203,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[4] = intWindow->getObjectsList()->insertButton(123,186,
                                                                133,203,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[4]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[4]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[4] = intWindow->getObjectsList()->insertTextBox(170,188,195,203,
                                                             0,"");
   attTotals[4]->setFont(DNT_FONT_TIMES, 12, 0);
   attMods[4] = intWindow->getObjectsList()->insertTextBox(196,185,226,200,
                                                           0,"");
   intWindow->getObjectsList()->insertPicture(135,177,0,0,
                           dir.getRealFile("skills/Img/sabedoria.png").c_str());

   /* Charism */
   intWindow->getObjectsList()->insertTextBox(9,222,87,235,0,
                                              gettext("Charism"));
   attPoints[5] = intWindow->getObjectsList()->insertTextBox(103,220,122,237,1,
                                                             "");
   attPoints[5]->setFont(DNT_FONT_ARIAL, 10, 0);
   attButtonPrev[5] = intWindow->getObjectsList()->insertButton(92,220,102,237,
                                                  fnt.createUnicode(0x25C4),0);
   attButtonNext[5] = intWindow->getObjectsList()->insertButton(123,220,
                                                                133,237,
                                                  fnt.createUnicode(0x25BA),0);
   attButtonPrev[5]->defineFont(DNT_FONT_ARIAL, 9);
   attButtonNext[5]->defineFont(DNT_FONT_ARIAL, 9);
   attTotals[5] = intWindow->getObjectsList()->insertTextBox(170,221,195,237,
                                                             0,"");
   attTotals[5]->setFont(DNT_FONT_TIMES, 12, 0);
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
   textDescTitle->setFont(DNT_FONT_TIMES,12,DNT_FONT_ALIGN_CENTER,
                          DNT_FONT_STYLE_BOLD);
   
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
   textDesc->setText("");
   textDesc->addText(externalSkill->m_skills[sk].name + "||", 
                     DNT_FONT_ARIAL, 12, DNT_FONT_ALIGN_CENTER,
                     DNT_FONT_STYLE_UNDERLINE,
                     86, 161, 32);
   textDesc->addText(externalSkill->m_skills[sk].description);
   textDesc->setFirstLine(0);
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
   for(i = 0; i < 6; i++)
   {
      points[i] = saveSkill->m_skills[i+1].points;
      used[i] = true;
   }
}

/**************************************************************
 *                    assignPreviousValues                    *
 **************************************************************/
void attWindow::assignPreviousValues()
{
   int i;
   char tmp[10];
   for(i = 0; i < 6; i++)
   {
      sprintf(tmp,"%.2d", saveSkill->m_skills[i+1].points);
      attPoints[i]->setText(tmp);
      attPointsIndex[i] = i;
      assignAttMod(i);
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
   char tmpMod[10];
   char tmpTotal[10];
   int attBonus = 0;
   string total;

   /* Get the assign value */
   saveSkill->m_skills[att+1].points = points[attPointsIndex[att]];

   /* Apply the modifiers */
   saveSkill->m_skills[att+1].points += rcModifiers[att];

   /* Get the bonus */
   attBonus = (int)floor((saveSkill->m_skills[att+1].points-10) / 2.0);

   sprintf(tmpTotal,"%.2d",saveSkill->m_skills[att+1].points);
   total += tmpTotal;
   sprintf(tmpMod,"%+.2d",attBonus);

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
   attMods[att]->setFont(DNT_FONT_ARIAL,14,0);

   return(attBonus);
}

/**************************************************************
 *                            treat                           *
 **************************************************************/
int attWindow::treat(guiObject* object, int eventInfo, guiInterface* inter,
                     GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   char tmp[5];
   int i;

   if(eventInfo == PRESSED_BUTTON)
   {
      /* Confirm */
      if(object == (guiObject*) buttonConfirm)
      {
         if(allAssigned())
         {
            inter->closeWindow(intWindow);
            intWindow = NULL;
            return(ATTW_CONFIRM);
         }
         else
         {
            warning warn;
            warn.show(gettext("Warning"),
                      gettext("You Need to Assign All Attributes."), inter);
            return(ATTW_OTHER);
         }
      }
      /* Cancel */
      else if(object == (guiObject*) buttonCancel) 
      {
          inter->closeWindow(intWindow);
          intWindow = NULL;
         return(ATTW_CANCEL);
      }
      /* Reroll */
      else if(object == (guiObject*) rerollButton)
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
      else if(object == (guiObject*) clearButton)
      {
         clear();
      }
      /* Auto */
      else if(object == (guiObject*) autoButton)
      {
         autoAssign();
      }
      /* Assign Buttons */
      else
      {
         for(i=0; i<6; i++) 
         {
            if(object == (guiObject*) attButtonNext[i])
            {
                nextAvaiblePoints(i);
                sprintf(tmp,"%.2d", points[attPointsIndex[i]]);
                attPoints[i]->setText(tmp);
                assignAttMod(i);
                setDescription(i+1);
                intWindow->draw(0,0);
            }
            else if(object == (guiObject*) attButtonPrev[i])
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

