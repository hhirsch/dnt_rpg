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
attWindow::attWindow(skills* sk, interface* inter)
{
   int i;
   string saux;
   char tmp[5];

   externalSkill = sk;

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

   /* create window */
   window = inter->ljan->InserirJanela(146,166,661,441,
                                       language.ATTW_TITLE.c_str(),
                                       1,1,NULL,NULL);

   /* roll and write all rolled dices to string */
   rollAllDices();
   saux = "";
   for(i = 0; i < 6; i++)
   {
      sprintf(tmp,"%d ", points[i]); 
      saux += tmp;
   }
   rolledPoints = window->objects->InserirQuadroTexto(8,18,117,31,0,
                                                                  saux.c_str());
   /* reroll button */
   rerollButton = window->objects->InserirBotao(119,16,182,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.ATTW_ROLL.c_str(),0,NULL);
   /* clear button */
   clearButton = window->objects->InserirBotao(183,16,247,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.ATTW_CLEAR.c_str(),0,NULL);

   /* Strenght */
   window->objects->InserirQuadroTexto(9,49,85,62,0,
                                       language.ATTW_STRENGTH.c_str());
   attPoints[0] = window->objects->InserirQuadroTexto(98,47,117,64,1,"");
   attPoints[0]->fonte = FMINI;
   attButtonPrev[0] = window->objects->InserirBotao(87,47,97,64,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[0] = window->objects->InserirBotao(118,47,128,64,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[0] = window->objects->InserirQuadroTexto(165,49,195,64,0,"");
   attTotals[0]->fonte = FTIMES;
   attTotals[0]->tamFonte = 3;
   attMods[0] = window->objects->InserirQuadroTexto(196,49,226,64,0,
                                                    "");
   window->objects->InserirFigura(130,40,0,0,"../data/skills/Img/forca.png");

   /* Dextery */
   window->objects->InserirQuadroTexto(9,83,85,96,0,
                                       language.ATTW_DEXTERITY.c_str());
   attPoints[1] = window->objects->InserirQuadroTexto(98,81,117,98,1,"");
   attPoints[1]->fonte = FMINI;
   attButtonPrev[1] = window->objects->InserirBotao(87,81,97,98,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[1] = window->objects->InserirBotao(118,81,128,98,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[1] = window->objects->InserirQuadroTexto(165,83,195,98,0,"");
   attTotals[1]->fonte = FTIMES;
   attTotals[1]->tamFonte = 3;
   attMods[1] = window->objects->InserirQuadroTexto(196,82,226,98,0,
                                                    "");
   window->objects->InserirFigura(130,74,0,0,"../data/skills/Img/destreza.png");

   /* Constitution */
   window->objects->InserirQuadroTexto(9,119,87,132,0,
                                       language.ATTW_CONSTITUTION.c_str());
   attPoints[2] = window->objects->InserirQuadroTexto(98,117,117,134,1,"");
   attPoints[2]->fonte = FMINI;
   attButtonPrev[2] = window->objects->InserirBotao(87,117,97,134,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[2] = window->objects->InserirBotao(118,117,128,134,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[2] = window->objects->InserirQuadroTexto(165,119,195,134,0,"");
   attTotals[2]->fonte = FTIMES;
   attTotals[2]->tamFonte = 3;
   attMods[2] = window->objects->InserirQuadroTexto(196,119,226,134,0,
                                                    "");
   window->objects->InserirFigura(130,108,0,0,
                                         "../data/skills/Img/constituicao.png");

   /* Inteligency */
   window->objects->InserirQuadroTexto(9,153,87,166,0,
                                       language.ATTW_INTELLIGENCE.c_str());
   attPoints[3] = window->objects->InserirQuadroTexto(98,151,117,168,1,"");
   attPoints[3]->fonte = FMINI;
   attButtonPrev[3] = window->objects->InserirBotao(87,151,97,168,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[3] = window->objects->InserirBotao(118,151,128,168,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[3] = window->objects->InserirQuadroTexto(165,153,195,168,0,"");
   attTotals[3]->fonte = FTIMES;
   attTotals[3]->tamFonte = 3;
   attMods[3] = window->objects->InserirQuadroTexto(196,153,226,168,0,
                                                    "");
   window->objects->InserirFigura(130,142,0,0,
                                         "../data/skills/Img/inteligencia.png");

   /* Wisdow */
   window->objects->InserirQuadroTexto(9,187,87,200,0,
                                       language.ATTW_WISDOW.c_str());
   attPoints[4] = window->objects->InserirQuadroTexto(98,185,117,202,1,"");
   attPoints[4]->fonte = FMINI;
   attButtonPrev[4] = window->objects->InserirBotao(87,185,97,202,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[4] = window->objects->InserirBotao(118,185,128,202,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[4] = window->objects->InserirQuadroTexto(165,187,195,202,0,"");
   attTotals[4]->fonte = FTIMES;
   attTotals[4]->tamFonte = 3;
   attMods[4] = window->objects->InserirQuadroTexto(196,187,226,202,0,"");
   window->objects->InserirFigura(130,176,0,0,
                                            "../data/skills/Img/sabedoria.png");

   /* Charism */
   window->objects->InserirQuadroTexto(9,221,87,234,0,
                                       language.ATTW_CHARISM.c_str());
   attPoints[5] = window->objects->InserirQuadroTexto(98,219,117,236,1,"");
   attPoints[5]->fonte = FMINI;
   attButtonPrev[5] = window->objects->InserirBotao(87,219,97,236,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[5] = window->objects->InserirBotao(119,219,128,236,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attTotals[5] = window->objects->InserirQuadroTexto(165,220,195,236,0,"");
   attTotals[5]->fonte = FTIMES;
   attTotals[5]->tamFonte = 3;
   attMods[5] = window->objects->InserirQuadroTexto(196,221,226,236,0,"");
   window->objects->InserirFigura(130,210,0,0,"../data/skills/Img/carisma.png");
                                              

   /* Contorns */
   window->objects->InserirQuadroTexto(10,39,245,72,1,"");
   window->objects->InserirQuadroTexto(10,73,245,106,1,"");
   window->objects->InserirQuadroTexto(10,107,245,140,1,"");
   window->objects->InserirQuadroTexto(10,141,245,174,1,"");
   window->objects->InserirQuadroTexto(10,175,245,208,1,"");
   window->objects->InserirQuadroTexto(10,209,245,242,1,"");
   window->objects->InserirQuadroTexto(8,37,247,244,1,"");

   /* Race Description */
   textDescTitle = window->objects->InserirQuadroTexto(251,20,507,35,1,
                                              language.ATTW_DESCRIPTION.c_str());
   textDescTitle->fonte = FHELVETICA;
   textDescTitle->tamFonte = 1;
   
   textDesc = window->objects->InserirQuadroTexto(251,36,507,244,1,
                      (externalSkill->m_skills[1].nome + "||" +
                       externalSkill->m_skills[1].descricao).c_str());

   /*textDesc->Cores.corTexto.R = 246;
   textDesc->Cores.corTexto.G = 190;
   textDesc->Cores.corTexto.B = 190;*/
   textDesc->fonte = FMINI;

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(437,248,507,267, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,248,78,267, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);
   
   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(inter->ljan);
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
      value[i] = (rand() % DICE_D6) + 1;
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
 *                      rollAllDices                          *
 **************************************************************/
void attWindow::rollAllDices()
{
   int i;
   for(i = 0; i < 6; i++)
   {
      srand(SDL_GetTicks());
      points[i] = rollDices();
      SDL_Delay(40);
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
      attPoints[i]->texto = "";
      attTotals[i]->texto = "";
      attMods[i]->texto = "";
      window->Desenhar(0,0);
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

   attTotals[att]->texto = tmpTotal;
   attMods[att]->texto = tmpMod;

   return(attBonus);
}

/**************************************************************
 *                            threat                          *
 **************************************************************/
int attWindow::treat(Tobjeto* object, int eventInfo, interface* inter,
                     GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   char tmp[5];
   int i;

   if(eventInfo == BOTAOPRESSIONADO)
   {
      if(object == (Tobjeto*) buttonConfirm)
      {
         if(allAssigned())
         {
            window->Fechar(inter->ljan);
            window = NULL;
            //TODO Save values
            glEnable(GL_LIGHTING);
            SDL_ShowCursor(SDL_DISABLE);
            return(ATTW_CONFIRM);
         }
         else
         {
            showMessage("Warning", "You Need to Assign All Attributes.",
                        proj, modl,viewPort);
            glDisable(GL_LIGHTING);
            return(ATTW_OTHER);
         }
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
          window->Fechar(inter->ljan);
          window = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
         return(ATTW_CANCEL);
      }
      else if(object == (Tobjeto*) rerollButton)
      {
          clear();
          /* ReRoll All Dices */
          rollAllDices();
          rolledPoints->texto = "";
          for(i = 0; i < 6; i++)
          {
              sprintf(tmp,"%d ", points[i]); 
              rolledPoints->texto += tmp;
          }
          window->Desenhar(0,0);
      }
      else if(object == (Tobjeto*) clearButton)
      {
         clear();
      }
      else
      {
         for(i=0; i<6; i++) 
         {
            if(object == (Tobjeto*) attButtonNext[i])
            {
                nextAvaiblePoints(i);
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->texto = tmp;
                assignAttMod(i);
                textDesc->texto = (externalSkill->m_skills[i+1].nome + "||" +
                                  externalSkill->m_skills[i+1].descricao);
                window->Desenhar(0,0);
            }
            else if(object == (Tobjeto*) attButtonPrev[i])
            {
                previousAvaiblePoints(i);
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->texto = tmp;
                assignAttMod(i);
                textDesc->texto = (externalSkill->m_skills[i+1].nome + "||" +
                                  externalSkill->m_skills[i+1].descricao);

                window->Desenhar(0,0);
            }
         }
      }
   }
   return(ATTW_OTHER);
}

