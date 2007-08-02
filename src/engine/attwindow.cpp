/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "attwindow.h"
#include "../classes/skills.h"
#include "../classes/dices.h"
#include "../gui/messages.h"

/**************************************************************
 *                      Constructor                           *
 **************************************************************/
attWindow::attWindow(skills* sk, skills* savSkill, interface* inter,
                     bool usePreviousValues)
{
   int i;
   string saux;
   char tmp[5];

   externalSkill = sk;
   saveSkill = savSkill;

   /* Disable lighting */
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   /* clear internal informations */
   for( i = 0; i < 6; i++)
   {
      points[i] = 0;
      used[i] = false;
      attPointsIndex[i] = -1;
   }

   /* create intWindow */
   intWindow = inter->insertWindow(146,166,661,441,language.ATTW_TITLE.c_str());

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
      sprintf(tmp,"%d ", points[i]); 
      saux += tmp;
   }
   rolledPoints = intWindow->getObjectsList()->insertTextBox(8,18,117,31,0,
                                                                  saux.c_str());
   /* reroll button */
   rerollButton = intWindow->getObjectsList()->insertButton(119,16,182,33,
                                                  language.ATTW_ROLL.c_str(),0);
   /* clear button */
   clearButton = intWindow->getObjectsList()->insertButton(183,16,247,33,
                                                 language.ATTW_CLEAR.c_str(),0);

   /* auto button */
   autoButton = intWindow->getObjectsList()->insertButton(248,16,312,33,
                                                          "Auto",0);

   /* Strenght */
   intWindow->getObjectsList()->insertTextBox(9,49,85,62,0,
                                       language.ATTW_STRENGTH.c_str());
   attPoints[0] = intWindow->getObjectsList()->insertTextBox(98,47,117,64,1,"");
   attPoints[0]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[0] = intWindow->getObjectsList()->insertButton(87,47,97,64,
                                                                "<",0);
   attButtonNext[0] = intWindow->getObjectsList()->insertButton(118,47,128,64,
                                                                ">",0);
   attTotals[0] = intWindow->getObjectsList()->insertTextBox(165,49,195,64,0,
                                                             "");
   attTotals[0]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[0] = intWindow->getObjectsList()->insertTextBox(196,49,226,64,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(130,40,0,0,
                                              "../data/skills/Img/forca.png");

   /* Dextery */
   intWindow->getObjectsList()->insertTextBox(9,83,85,96,0,
                                       language.ATTW_DEXTERITY.c_str());
   attPoints[1] = intWindow->getObjectsList()->insertTextBox(98,81,117,98,1,"");
   attPoints[1]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[1] = intWindow->getObjectsList()->insertButton(87,81,97,98,"<",0);
   attButtonNext[1] = intWindow->getObjectsList()->insertButton(118,81,128,98,">",0);
   attTotals[1] = intWindow->getObjectsList()->insertTextBox(165,83,195,98,0,"");
   attTotals[1]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[1] = intWindow->getObjectsList()->insertTextBox(196,82,226,98,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(130,74,0,0,
                                             "../data/skills/Img/destreza.png");

   /* Constitution */
   intWindow->getObjectsList()->insertTextBox(9,119,87,132,0,
                                       language.ATTW_CONSTITUTION.c_str());
   attPoints[2] = intWindow->getObjectsList()->insertTextBox(98,117,117,134,1,"");
   attPoints[2]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[2] = intWindow->getObjectsList()->insertButton(87,117,97,134,"<",0);
   attButtonNext[2] = intWindow->getObjectsList()->insertButton(118,117,128,134,">",0);
   attTotals[2] = intWindow->getObjectsList()->insertTextBox(165,119,195,134,0,"");
   attTotals[2]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[2] = intWindow->getObjectsList()->insertTextBox(196,119,226,134,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(130,108,0,0,
                                         "../data/skills/Img/constituicao.png");

   /* Inteligency */
   intWindow->getObjectsList()->insertTextBox(9,153,87,166,0,
                                       language.ATTW_INTELLIGENCE.c_str());
   attPoints[3] = intWindow->getObjectsList()->insertTextBox(98,151,117,168,1,"");
   attPoints[3]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[3] = intWindow->getObjectsList()->insertButton(87,151,97,168,"<",0);
   attButtonNext[3] = intWindow->getObjectsList()->insertButton(118,151,128,168,">",0);
   attTotals[3] = intWindow->getObjectsList()->insertTextBox(165,153,195,168,0,"");
   attTotals[3]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[3] = intWindow->getObjectsList()->insertTextBox(196,153,226,168,0,
                                                    "");
   intWindow->getObjectsList()->insertPicture(130,142,0,0,
                                         "../data/skills/Img/inteligencia.png");

   /* Wisdow */
   intWindow->getObjectsList()->insertTextBox(9,187,87,200,0,
                                       language.ATTW_WISDOW.c_str());
   attPoints[4] = intWindow->getObjectsList()->insertTextBox(98,185,117,202,1,"");
   attPoints[4]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[4] = intWindow->getObjectsList()->insertButton(87,185,97,202,"<",0);
   attButtonNext[4] = intWindow->getObjectsList()->insertButton(118,185,128,202,">",0);
   attTotals[4] = intWindow->getObjectsList()->insertTextBox(165,187,195,202,0,"");
   attTotals[4]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[4] = intWindow->getObjectsList()->insertTextBox(196,187,226,202,0,"");
   intWindow->getObjectsList()->insertPicture(130,176,0,0,
                                            "../data/skills/Img/sabedoria.png");

   /* Charism */
   intWindow->getObjectsList()->insertTextBox(9,221,87,234,0,
                                       language.ATTW_CHARISM.c_str());
   attPoints[5] = intWindow->getObjectsList()->insertTextBox(98,219,117,236,1,"");
   attPoints[5]->setFont(FMINI, 1, ALIGN_LEFT);
   attButtonPrev[5] = intWindow->getObjectsList()->insertButton(87,219,97,236,"<",0);
   attButtonNext[5] = intWindow->getObjectsList()->insertButton(118,219,128,236,">",0);
   attTotals[5] = intWindow->getObjectsList()->insertTextBox(165,220,195,236,0,"");
   attTotals[5]->setFont(FTIMES, 1, ALIGN_LEFT);
   attMods[5] = intWindow->getObjectsList()->insertTextBox(196,221,226,236,0,"");
   intWindow->getObjectsList()->insertPicture(130,210,0,0,"../data/skills/Img/carisma.png");
                                              

   /* Contorns */
   intWindow->getObjectsList()->insertTextBox(10,39,245,72,1,"");
   intWindow->getObjectsList()->insertTextBox(10,73,245,106,1,"");
   intWindow->getObjectsList()->insertTextBox(10,107,245,140,1,"");
   intWindow->getObjectsList()->insertTextBox(10,141,245,174,1,"");
   intWindow->getObjectsList()->insertTextBox(10,175,245,208,1,"");
   intWindow->getObjectsList()->insertTextBox(10,209,245,242,1,"");
   intWindow->getObjectsList()->insertTextBox(8,37,247,244,1,"");

   /* Attribute Description */
   textDescTitle = intWindow->getObjectsList()->insertTextBox(248,37,507,52,1,
                                             language.ATTW_DESCRIPTION.c_str());
   textDescTitle->setFont(FHELVETICA, 1, ALIGN_LEFT);
   
   textDesc = intWindow->getObjectsList()->insertTextBox(248,53,507,244,1,
                      (externalSkill->m_skills[1].name + "||" +
                       externalSkill->m_skills[1].description).c_str());
   textDesc->setFont(FMINI,1,ALIGN_LEFT);

   /* Confirm Button */
   buttonConfirm = intWindow->getObjectsList()->insertButton(437,248,507,267,
                                              language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = intWindow->getObjectsList()->insertButton(8,248,78,267,
                                               language.SKILL_CANCEL.c_str(),1);

   /* Define Previous Values, if needed */
   if(usePreviousValues)
   {
      assignPreviousValues();
   }
   
   /* Open Skill Window */
   intWindow->setExternPointer(&intWindow);
   intWindow->setAttributes(false,false,false,false);
   inter->openWindow(intWindow);
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
      sprintf(tmp,"%d", saveSkill->m_skills[i+1].points);
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
      sprintf(tmp,"%d", points[attPointsIndex[i]]);
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
   int attBonus = (int)floor((points[attPointsIndex[att]]-10) / 2.0);

   saveSkill->m_skills[att+1].points = points[attPointsIndex[att]];

   //TODO calculate race and class bonus
   
   sprintf(tmpTotal,"%d",points[attPointsIndex[att]]);

   if(attBonus >= 0)
   {
      sprintf(tmpMod,"+%d",attBonus);
   }
   else
   {
      sprintf(tmpMod,"%d",attBonus);
   }

   attTotals[att]->setText(tmpTotal);
   attMods[att]->setText(tmpMod);

   return(attBonus);
}

/**************************************************************
 *                            threat                          *
 **************************************************************/
int attWindow::treat(guiObject* object, int eventInfo, interface* inter,
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
            glEnable(GL_LIGHTING);
            SDL_ShowCursor(SDL_DISABLE);
            return(ATTW_CONFIRM);
         }
         else
         {
            showMessage(language.ATTW_WARN_TITLE, language.ATTW_WARN_MESSAGE,
                        proj, modl,viewPort);
            glDisable(GL_LIGHTING);
            glDisable(GL_FOG);
            return(ATTW_OTHER);
         }
      }
      /* Cancel */
      else if(object == (guiObject*) buttonCancel) 
      {
          inter->closeWindow(intWindow);
          intWindow = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
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
              sprintf(tmp,"%d ", points[i]); 
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
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->setText(tmp);
                assignAttMod(i);
                textDesc->setText(externalSkill->m_skills[i+1].name + "||" +
                                  externalSkill->m_skills[i+1].description);
                intWindow->draw(0,0);
            }
            else if(object == (guiObject*) attButtonPrev[i])
            {
                previousAvaiblePoints(i);
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->setText(tmp);
                assignAttMod(i);
                textDesc->setText(externalSkill->m_skills[i+1].name + "||" +
                                  externalSkill->m_skills[i+1].description);

                intWindow->draw(0,0);
            }
         }
      }
   }
   return(ATTW_OTHER);
}

