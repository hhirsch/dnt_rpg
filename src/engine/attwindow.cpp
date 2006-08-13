#include "attwindow.h"
#include "../classes/skills.h"
#include "../classes/dices.h"

attWindow::attWindow(skills* sk, interface* inter)
{
   int i;
   string saux;
   char tmp[5];

   externalSkill = sk;
   for( i = 0; i < 6; i++)
   {
      points[i] = 0;
      used[i] = false;
      attPointsIndex[i] = -1;
   }

   window = inter->ljan->InserirJanela(316,186,571,441,
                                       "Attributes",
                                       1,1,NULL,NULL);
   rollAllDices();

   saux = "";
   for(i = 0; i < 6; i++)
   {
      sprintf(tmp,"%d ", points[i]); 
      saux += tmp;
   }

   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);


   rolledPoints = window->objects->InserirQuadroTexto(8,18,117,31,0,
                                                                  saux.c_str());
   rerollButton = window->objects->InserirBotao(119,16,170,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "Reroll",1,NULL);
   clearButton = window->objects->InserirBotao(175,16,220,33,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "Clear",1,NULL);


   window->objects->InserirQuadroTexto(8,46,85,59,0,"Strenght");
   attPoints[0] = window->objects->InserirQuadroTexto(98,44,115,61,1,"");
   attPoints[0]->fonte = FMINI;
   attButtonPrev[0] = window->objects->InserirBotao(87,44,97,61,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[0] = window->objects->InserirBotao(116,44,126,61,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[0] = window->objects->InserirQuadroTexto(162,45,245,60,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,37,0,0,"../data/skills/Img/forca.png");

   window->objects->InserirQuadroTexto(8,78,85,91,0,"Dextery");
   attPoints[1] = window->objects->InserirQuadroTexto(98,76,115,93,1,"");
   attPoints[1]->fonte = FMINI;
   attButtonPrev[1] = window->objects->InserirBotao(87,76,97,93,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[1] = window->objects->InserirBotao(116,76,126,93,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[1] = window->objects->InserirQuadroTexto(162,77,245,92,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,69,0,0,"../data/skills/Img/destreza.png");

   window->objects->InserirQuadroTexto(8,112,87,125,0,"Constitution");
   attPoints[2] = window->objects->InserirQuadroTexto(98,110,115,127,1,"");
   attPoints[2]->fonte = FMINI;
   attButtonPrev[2] = window->objects->InserirBotao(87,110,97,127,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[2] = window->objects->InserirBotao(116,110,126,127,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[2] = window->objects->InserirQuadroTexto(162,111,245,126,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,101,0,0,
                                         "../data/skills/Img/constituicao.png");

   window->objects->InserirQuadroTexto(8,144,87,157,0,"Inteligency");
   attPoints[3] = window->objects->InserirQuadroTexto(98,142,115,159,1,"");
   attPoints[3]->fonte = FMINI;
   attButtonPrev[3] = window->objects->InserirBotao(87,142,97,159,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[3] = window->objects->InserirBotao(116,142,126,159,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[3] = window->objects->InserirQuadroTexto(162,143,245,88,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,133,0,0,
                                         "../data/skills/Img/inteligencia.png");

   window->objects->InserirQuadroTexto(8,176,87,189,0,"Wisdow");
   attPoints[4] = window->objects->InserirQuadroTexto(98,174,115,191,1,"");
   attPoints[4]->fonte = FMINI;
   attButtonPrev[4] = window->objects->InserirBotao(87,174,97,191,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[4] = window->objects->InserirBotao(116,174,126,191,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[4] = window->objects->InserirQuadroTexto(162,175,245,190,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,165,0,0,
                                            "../data/skills/Img/sabedoria.png");

   window->objects->InserirQuadroTexto(8,208,87,221,0,"Charism");
   attPoints[5] = window->objects->InserirQuadroTexto(98,206,115,223,1,"");
   attPoints[5]->fonte = FMINI;
   attButtonPrev[5] = window->objects->InserirBotao(87,206,97,223,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    "<",0,NULL);
   attButtonNext[5] = window->objects->InserirBotao(116,206,126,223,
                                                    window->Cores.corBot.R,
                                                    window->Cores.corBot.G,
                                                    window->Cores.corBot.B,
                                                    ">",0,NULL);
   attMods[5] = window->objects->InserirQuadroTexto(162,207,245,222,0,
                                                    "T:?? M:??");
   window->objects->InserirFigura(128,197,0,0,"../data/skills/Img/carisma.png");
                                              

   /* Contorns */
   window->objects->InserirQuadroTexto(8,35,247,230,1,"");

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,232,251,250, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,232,78,250, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);
   
   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(inter->ljan);
}

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

void attWindow::clear()
{
   int i;
   for( i = 0; i < 6; i++)
   {
      used[i] = false;
      attPointsIndex[i] = -1;
      attPoints[i]->texto = "";
      window->Desenhar(0,0);
   }
}


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
                window->Desenhar(0,0);
            }
            else if(object == (Tobjeto*) attButtonPrev[i])
            {
                previousAvaiblePoints(i);
                sprintf(tmp,"%d", points[attPointsIndex[i]]);
                attPoints[i]->texto = tmp;
                window->Desenhar(0,0);
            }
         }
      }
   }
   return(ATTW_OTHER);
}

