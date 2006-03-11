#include "skillwindow.h"
#include "../lang/lang.h"

#define ACT_NONE    0
#define ACT_SUM     1
#define ACT_SUB     2
#define ACT_NEXT    3
#define ACT_PREV    4
#define ACT_CONFIRM 5
#define ACT_CANCEL  6

int curAction = ACT_NONE;

int botaoSum(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_SUM;
   return(1);
}

int botaoSub(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_SUB;
   return(1);
}

int botaoNext(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_NEXT;
   return(1);
}

int botaoPrev(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_PREV;
   return(1);
}

int botaoConfirm(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_CONFIRM;
   return(1);
}

int botaoCancel(void *jan,void *ljan,SDL_Surface *screen)
{
   curAction = ACT_CANCEL;
   return(1);
}


skillWindow::skillWindow(skills* sk, int points, interface* inter)
{
   char tmp[5];
   string saux;
   botao* baux;
   int aux;

   curSkill = ATT_CARISMA+1; //Set to first skill after attribute
   externalSkill = sk;
   avaiblePoints = points;


   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   /* Define previous points, for undo */
   for(aux = curSkill; aux < VAR_AUX1; aux++)
   { 
     externalSkill->m_skills[aux].antPontos=externalSkill->m_skills[aux].pontos;
   }
   
   /* Create Skill Window */
   window = inter->ljan->InserirJanela(316,186,571,441,SKILL_TITLE,
                                       1,1,NULL,NULL);

   /* Free Points */
   sprintf(tmp,"%d",avaiblePoints);
   saux = tmp;
   window->objetos->InserirQuadroTexto(8,20,125,33,0,SKILL_FREE_POINTS);
   txtAvaiblePoints = window->objetos->InserirQuadroTexto(127,20,162,33,0,
                                                                  saux.c_str());

   /* Skill Description */
   desc = window->objetos->InserirQuadroTexto(8,38,251,170,1,
                           externalSkill->m_skills[curSkill].descricao.c_str());
   desc->Cores.corTexto.R = 20;
   desc->Cores.corTexto.G = 29;
   desc->Cores.corTexto.B = 217;
   desc->Cores.corBot.R = desc->Cores.corCont[2].R; 
   desc->Cores.corBot.G = desc->Cores.corCont[2].G;
   desc->Cores.corBot.B = desc->Cores.corCont[2].B;
   desc->fonte = FMINI;
   desc->tamFonte = 1;
 
   /* Skill Name & Selectors */
   window->objetos->InserirBotao(52,175,66,193,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,&botaoPrev);
   window->objetos->InserirBotao(237,175,251,193,window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,&botaoNext);
   skillName = window->objetos->InserirQuadroTexto(67,175,236,193,1,
                                externalSkill->m_skills[curSkill].nome.c_str());
   skillName->fonte = FMINI;

   /* Skill Image */
   skFig = window->objetos->InserirFigura(13,175,0,0,NULL);
   skFig->fig = externalSkill->m_skills[curSkill].imagem;
 
   /* Skill Points */
   window->objetos->InserirQuadroTexto(52,200,101,214,0,SKILL_POINTS);
   sprintf(tmp,"%d",externalSkill->m_skills[curSkill].pontos);
   saux = tmp;
   txtPoints = window->objetos->InserirQuadroTexto(113,198,135,216,1,
                                                   saux.c_str());
   txtPoints->fonte = FMINI;
   baux = window->objetos->InserirBotao(136,198,146,216, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,&botaoSum);
   baux = window->objetos->InserirBotao(102,198,112,216, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,&botaoSub);

   /* Skill Costs */
   window->objetos->InserirQuadroTexto(160,200,215,214,0,SKILL_COST);
   sprintf(tmp,"%d",externalSkill->m_skills[curSkill].mod);
   saux = tmp;
   txtCosts = window->objetos->InserirQuadroTexto(216,200,251,214,0,
                                                  saux.c_str());
   txtCosts->Cores.corTexto.R = 255;
   txtCosts->Cores.corTexto.G = 156;
   txtCosts->Cores.corTexto.B = 0;


   /* Confirm Button */
   baux = window->objetos->InserirBotao(181,225,251,244, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                        SKILL_CONFIRM,1,&botaoConfirm);
   
   /* Cancel Button */
   baux = window->objetos->InserirBotao(8,225,78,244, window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                        SKILL_CANCEL,1,&botaoCancel);
   
   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(inter->ljan);
 
}

int skillWindow::treat(interface* inter)
{
   switch(curAction)
   {
      case ACT_SUM:
      {
         if( avaiblePoints - externalSkill->m_skills[curSkill].mod >=0 )
         {
             externalSkill->m_skills[curSkill].pontos++;
             avaiblePoints -= externalSkill->m_skills[curSkill].mod;
             
             char tmp[5];
             sprintf(tmp,"%d",avaiblePoints);
             string saux = tmp;
             txtAvaiblePoints->texto = saux;
        
             sprintf(tmp,"%d",externalSkill->m_skills[curSkill].pontos);
             saux = tmp;
             txtPoints->texto = saux;

         }
         break;
      }
      case ACT_SUB:
      {
         if(externalSkill->m_skills[curSkill].pontos - 1 >= 
            externalSkill->m_skills[curSkill].antPontos)
         {
            externalSkill->m_skills[curSkill].pontos--;
            avaiblePoints += externalSkill->m_skills[curSkill].mod;

            char tmp[5];
            sprintf(tmp,"%d",avaiblePoints);
            string saux = tmp;
            txtAvaiblePoints->texto = saux;

            sprintf(tmp,"%d",externalSkill->m_skills[curSkill].pontos);
            saux = tmp;
            txtPoints->texto = saux;
         }
         break;
      }
      case ACT_NEXT:
      {
         if(curSkill < VAR_AUX1-1)
         {
            curSkill++;
         }
         else
         {
            curSkill = ATT_CARISMA+1;
         } 
         skillName->texto = externalSkill->m_skills[curSkill].nome.c_str();
         desc->texto = externalSkill->m_skills[curSkill].descricao.c_str();
         skFig->fig = externalSkill->m_skills[curSkill].imagem;
          
         char tmp[5];
         sprintf(tmp,"%d",externalSkill->m_skills[curSkill].pontos);
         string saux = tmp;
         txtPoints->texto = saux;
         sprintf(tmp,"%d",externalSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->texto = saux;

         break;
      }
      case ACT_PREV:
      {
         if(curSkill > ATT_CARISMA+1)
         {
            curSkill--;
         }
         else
         {
            curSkill = VAR_AUX1-1;
         }
         skillName->texto = externalSkill->m_skills[curSkill].nome.c_str();
         desc->texto = externalSkill->m_skills[curSkill].descricao.c_str();
         skFig->fig = externalSkill->m_skills[curSkill].imagem;

         char tmp[5];
         sprintf(tmp,"%d",externalSkill->m_skills[curSkill].pontos);
         string saux = tmp;
         txtPoints->texto = saux;
         sprintf(tmp,"%d",externalSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->texto = saux;

         break;
      }
      case ACT_CONFIRM:
      {
         skFig->fig = NULL; //to not delete skill images
         window->Fechar(inter->ljan);
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         curAction = ACT_NONE;
         return(SKILLW_CONFIRM);
         break;
      }
      case ACT_CANCEL: 
      {
         /* Undo */
         int aux;
         for(aux = curSkill; aux < VAR_AUX1; aux++)
         { 
            externalSkill->m_skills[aux].pontos = 
                                        externalSkill->m_skills[aux].antPontos;
          }
          skFig->fig = NULL; //to not delete skill images
          window->Fechar(inter->ljan);
          window = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
          curAction = ACT_NONE;
         return(SKILLW_CANCEL);
         break;
      }
   }

   if( externalSkill->m_skills[curSkill].pontos > 
       externalSkill->m_skills[curSkill].antPontos )
   {
      txtPoints->Cores.corTexto.R = 13;
      txtPoints->Cores.corTexto.G = 250;
      txtPoints->Cores.corTexto.B = 85; 
   }
   else
   {
      txtPoints->Cores.corTexto.R = 255;
      txtPoints->Cores.corTexto.G = 255;
      txtPoints->Cores.corTexto.B = 255;
   }
   
   window->Desenhar();
   curAction = ACT_NONE;
   return(SKILLW_OTHER);
}

