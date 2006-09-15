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
   window->objects->InserirQuadroTexto(9,49,85,62,0,"Strenght");
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
   attMods[0] = window->objects->InserirQuadroTexto(162,48,245,63,0,
                                                    "T:   M: ");
   window->objects->InserirFigura(130,40,0,0,"../data/skills/Img/forca.png");

   /* Dextery */
   window->objects->InserirQuadroTexto(9,83,85,96,0,"Dextery");
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
   attMods[1] = window->objects->InserirQuadroTexto(162,82,245,97,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,74,0,0,"../data/skills/Img/destreza.png");

   /* Constitution */
   window->objects->InserirQuadroTexto(9,119,87,132,0,"Constitution");
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
   attMods[2] = window->objects->InserirQuadroTexto(162,118,245,132,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,108,0,0,
                                         "../data/skills/Img/constituicao.png");

   /* Inteligency */
   window->objects->InserirQuadroTexto(9,153,87,166,0,"Inteligency");
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
   attMods[3] = window->objects->InserirQuadroTexto(162,152,245,169,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,142,0,0,
                                         "../data/skills/Img/inteligencia.png");

   /* Wisdow */
   window->objects->InserirQuadroTexto(9,187,87,200,0,"Wisdow");
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
   attMods[4] = window->objects->InserirQuadroTexto(162,186,245,201,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,176,0,0,
                                            "../data/skills/Img/sabedoria.png");

   /* Charism */
   window->objects->InserirQuadroTexto(9,221,87,234,0,"Charism");
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
   attMods[5] = window->objects->InserirQuadroTexto(162,220,245,235,0,
                                                    "T:   M:  ");
   window->objects->InserirFigura(130,210,0,0,"../data/skills/Img/carisma.png");
                                              

   /* Contorns */
   window->objects->InserirQuadroTexto(10,39,245,72,1,"");
   window->objects->InserirQuadroTexto(10,73,245,106,1,"");
   window->objects->InserirQuadroTexto(10,107,245,140,1,"");
   window->objects->InserirQuadroTexto(10,141,245,174,1,"");
   window->objects->InserirQuadroTexto(10,175,245,208,1,"");
   window->objects->InserirQuadroTexto(10,209,245,242,1,"");
   window->objects->InserirQuadroTexto(8,37,247,244,1,"");

   

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
      attMods[i]->texto = "T:   M: ";
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

