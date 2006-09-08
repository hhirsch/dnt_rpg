/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "attwindow.h"
#include "../classes/skills.h"
#include "../classes/dices.h"

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
   window = inter->ljan->InserirJanela(276,166,531,441,
                                       "Attributes",
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
   rerollButton = window->objects->InserirBotao(119,16,170,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "Reroll",1,NULL);
   /* clear button */
   clearButton = window->objects->InserirBotao(175,16,220,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "Clear",1,NULL);

   /* Strenght */
   window->objects->InserirQuadroTexto(8,47,85,60,0,"Strenght");
   attPoints[0] = window->objects->InserirQuadroTexto(98,45,117,62,1,"");
   attPoints[0]->fonte = FMINI;
   attButtonPrev[0] = window->objects->InserirBotao(87,45,97,62,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[0] = window->objects->InserirBotao(118,45,128,62,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[0] = window->objects->InserirQuadroTexto(162,46,245,61,0,
                                                    "T:  M: ");
   window->objects->InserirFigura(130,38,0,0,"../data/skills/Img/forca.png");

   /* Dextery */
   window->objects->InserirQuadroTexto(8,81,85,94,0,"Dextery");
   attPoints[1] = window->objects->InserirQuadroTexto(98,79,117,96,1,"");
   attPoints[1]->fonte = FMINI;
   attButtonPrev[1] = window->objects->InserirBotao(87,79,97,96,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[1] = window->objects->InserirBotao(118,79,128,96,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[1] = window->objects->InserirQuadroTexto(162,80,245,95,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,72,0,0,"../data/skills/Img/destreza.png");

   /* Constitution */
   window->objects->InserirQuadroTexto(8,117,87,130,0,"Constitution");
   attPoints[2] = window->objects->InserirQuadroTexto(98,115,117,132,1,"");
   attPoints[2]->fonte = FMINI;
   attButtonPrev[2] = window->objects->InserirBotao(87,115,97,132,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[2] = window->objects->InserirBotao(118,115,128,132,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[2] = window->objects->InserirQuadroTexto(162,116,245,131,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,106,0,0,
                                         "../data/skills/Img/constituicao.png");

   /* Inteligency */
   window->objects->InserirQuadroTexto(8,151,87,164,0,"Inteligency");
   attPoints[3] = window->objects->InserirQuadroTexto(98,149,117,166,1,"");
   attPoints[3]->fonte = FMINI;
   attButtonPrev[3] = window->objects->InserirBotao(87,149,97,166,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[3] = window->objects->InserirBotao(118,149,128,166,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[3] = window->objects->InserirQuadroTexto(162,150,245,167,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,140,0,0,
                                         "../data/skills/Img/inteligencia.png");

   /* Wisdow */
   window->objects->InserirQuadroTexto(8,185,87,198,0,"Wisdow");
   attPoints[4] = window->objects->InserirQuadroTexto(98,183,117,200,1,"");
   attPoints[4]->fonte = FMINI;
   attButtonPrev[4] = window->objects->InserirBotao(87,183,97,200,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[4] = window->objects->InserirBotao(118,183,128,200,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[4] = window->objects->InserirQuadroTexto(162,184,245,199,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,174,0,0,
                                            "../data/skills/Img/sabedoria.png");

   /* Charism */
   window->objects->InserirQuadroTexto(8,219,87,232,0,"Charism");
   attPoints[5] = window->objects->InserirQuadroTexto(98,217,117,234,1,"");
   attPoints[5]->fonte = FMINI;
   attButtonPrev[5] = window->objects->InserirBotao(87,217,97,234,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[5] = window->objects->InserirBotao(119,217,128,234,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[5] = window->objects->InserirQuadroTexto(162,218,245,233,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,208,0,0,"../data/skills/Img/carisma.png");
                                              

   /* Contorns */
   window->objects->InserirQuadroTexto(8,35,247,244,1,"");

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,248,251,265, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,248,78,265, 
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
      attMods[i]->texto = "T:  M: ";
      window->Desenhar(0,0);
   }
}

/**************************************************************
 *                         assignAttMod                       *
 **************************************************************/
int attWindow::assignAttMod(int att)
{
   char tmp[15];
   int attBonus = (int)floor((points[attPointsIndex[att]]-10) / 2.0);

   //TODO calculate race and class bonus
   if(attBonus > 0)
   {
      sprintf(tmp,"T:%d M:+%d",points[attPointsIndex[att]],attBonus);
   }
   else
   {
      sprintf(tmp,"T:%d M:%d",points[attPointsIndex[att]],attBonus);
   }
   attMods[att]->texto = tmp;

   return(attBonus);
}

/**************************************************************
 *                            threat                          *
 **************************************************************/
int attWindow::treat(Tobjeto* object, int eventInfo, interface* inter)
{
   char tmp[5];
   int i;

   if(eventInfo == BOTAOPRESSIONADO)
   {
      if(object == (Tobjeto*) buttonConfirm)
      {
         window->Fechar(inter->ljan);
         window = NULL;
         //TODO Save values
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ATTW_CONFIRM);
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
                window->Desenhar(0,0);
            }
            else if(object == (Tobjeto*) attButtonPrev[i])
            {
                previousAvaiblePoints(i);
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->texto = tmp;
                assignAttMod(i);
                window->Desenhar(0,0);
            }
         }
      }
   }
   return(ATTW_OTHER);
}

