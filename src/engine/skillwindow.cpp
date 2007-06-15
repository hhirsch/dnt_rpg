/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "skillwindow.h"
#include "../lang/lang.h"
#include "../classes/defs.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
skillWindow::skillWindow(skills* sk, skills* savSkill, interface* inter)
{
   char tmp[5];
   string saux;
   int aux;

   curSkill = ATT_SKILL_FIRST; //Set to first skill after attributes
   externalSkill = sk;
   saveSkill = savSkill;
   avaiblePoints = saveSkill->getAvaiblePoints();


   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   /* Define previous points, for undo */
   for(aux = curSkill; aux < ATT_SKILL_LAST; aux++)
   { 
     saveSkill->m_skills[aux].antPontos = saveSkill->m_skills[aux].pontos;
   }
   
   /* Create Skill Window */
   window = inter->insertWindow(276,186,531,441,
                                language.SKILL_TITLE.c_str(),1,1);

   /* Free Points */
   sprintf(tmp,"%d",avaiblePoints);
   saux = tmp;
   window->objects->InserirQuadroTexto(8,20,125,33,0,
                                       language.SKILL_FREE_POINTS.c_str());
   txtAvaiblePoints = window->objects->InserirQuadroTexto(127,20,162,33,0,
                                                                  saux.c_str());

   /* Skill Description */
   desc = window->objects->InserirQuadroTexto(8,38,251,170,1,
                           externalSkill->m_skills[curSkill].descricao.c_str());
   desc->Cores.corTexto.R = 246;
   desc->Cores.corTexto.G = 190;
   desc->Cores.corTexto.B = 190;
   desc->Cores.corBot.R = 155;
   desc->Cores.corBot.G = 5;
   desc->Cores.corBot.B = 5;
   desc->fonte = FMINI;
   desc->tamFonte = 1;
 
   /* Skill Name & Selectors */
   buttonPrevious = window->objects->insertButton(52,175,66,193,"<",0);
   buttonNext = window->objects->insertButton(237,175,251,193,">",0);
   skillName = window->objects->InserirQuadroTexto(67,175,236,193,1,
                                externalSkill->m_skills[curSkill].nome.c_str());
   skillName->fonte = FMINI;

   /* Skill Image */
   skFig = window->objects->InserirFigura(13,175,0,0,NULL);
   skFig->fig = externalSkill->m_skills[curSkill].imagem;
 
   /* Skill Points */
   window->objects->InserirQuadroTexto(52,200,101,214,0,
                                       language.SKILL_POINTS.c_str());
   sprintf(tmp,"%d",saveSkill->m_skills[curSkill].pontos);
   saux = tmp;
   txtPoints = window->objects->InserirQuadroTexto(113,198,135,216,1,
                                                   saux.c_str());
   txtPoints->fonte = FMINI;
   buttonSum = window->objects->insertButton(136,198,146,216,">",0);
   buttonDec = window->objects->insertButton(102,198,112,216,"<",0);

   /* Skill Costs */
   window->objects->InserirQuadroTexto(160,200,215,214,0,
                                       language.SKILL_COST.c_str());
   sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
   saux = tmp;
   txtCosts = window->objects->InserirQuadroTexto(216,200,251,214,0,
                                                  saux.c_str());
   txtCosts->Cores.corTexto.R = 255;
   txtCosts->Cores.corTexto.G = 156;
   txtCosts->Cores.corTexto.B = 0;


   /* Confirm Button */
   buttonConfirm = window->objects->insertButton(181,225,251,244,
                                              language.SKILL_CONFIRM.c_str(),1);
   
   /* Cancel Button */
   buttonCancel = window->objects->insertButton(8,225,78,244,
                                               language.SKILL_CANCEL.c_str(),1);
   
   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   inter->openWindow(window);
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int skillWindow::treat(guiObject* object, int eventInfo, interface* inter)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if(object == (guiObject*) buttonSum)
      {
         if( avaiblePoints - saveSkill->m_skills[curSkill].mod >=0 )
         {
             saveSkill->m_skills[curSkill].pontos++;
             avaiblePoints -= saveSkill->m_skills[curSkill].mod;
             
             char tmp[5];
             sprintf(tmp,"%d",avaiblePoints);
             string saux = tmp;
             txtAvaiblePoints->texto = saux;
        
             sprintf(tmp,"%d",saveSkill->m_skills[curSkill].pontos);
             saux = tmp;
             txtPoints->texto = saux;

         }
      }
      else if(object == (guiObject*) buttonDec)
      {
         if(saveSkill->m_skills[curSkill].pontos - 1 >= 
            saveSkill->m_skills[curSkill].antPontos)
         {
            saveSkill->m_skills[curSkill].pontos--;
            avaiblePoints += saveSkill->m_skills[curSkill].mod;

            char tmp[5];
            sprintf(tmp,"%d",avaiblePoints);
            string saux = tmp;
            txtAvaiblePoints->texto = saux;

            sprintf(tmp,"%d",saveSkill->m_skills[curSkill].pontos);
            saux = tmp;
            txtPoints->texto = saux;
         }
      }
      else if(object == (guiObject*) buttonNext)
      {
         if(curSkill < ATT_SKILL_LAST)
         {
            curSkill++;
         }
         else
         {
            curSkill = ATT_SKILL_FIRST;
         } 
         skillName->texto = externalSkill->m_skills[curSkill].nome.c_str();
         desc->texto = externalSkill->m_skills[curSkill].descricao.c_str();
         skFig->fig = externalSkill->m_skills[curSkill].imagem;
          
         char tmp[5];
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].pontos);
         string saux = tmp;
         txtPoints->texto = saux;
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->texto = saux;

      }
      else if(object == (guiObject*) buttonPrevious)
      {
         if(curSkill > ATT_SKILL_FIRST)
         {
            curSkill--;
         }
         else
         {
            curSkill = ATT_SKILL_LAST;
         }
         skillName->texto = externalSkill->m_skills[curSkill].nome.c_str();
         desc->texto = externalSkill->m_skills[curSkill].descricao.c_str();
         skFig->fig = externalSkill->m_skills[curSkill].imagem;

         char tmp[5];
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].pontos);
         string saux = tmp;
         txtPoints->texto = saux;
         sprintf(tmp,"%d",saveSkill->m_skills[curSkill].mod);
         saux = tmp;
         txtCosts->texto = saux;

      }
      else if(object == (guiObject*) buttonConfirm)
      {
         skFig->fig = NULL; //to not delete skill images
         inter->closeWindow(window);
         window = NULL;
         saveSkill->setAvaiblePoints(avaiblePoints);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(SKILLW_CONFIRM);
      }
      else if(object == (guiObject*) buttonCancel) 
      {
         /* Undo */
         int aux;
         for(aux = curSkill; aux < VAR_AUX1; aux++)
         { 
            saveSkill->m_skills[aux].pontos = saveSkill->m_skills[aux].antPontos;
          }
          skFig->fig = NULL; //to not delete skill images
          inter->closeWindow(window);
          window = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
         return(SKILLW_CANCEL);
      }
   }

   if( saveSkill->m_skills[curSkill].pontos > 
       saveSkill->m_skills[curSkill].antPontos )
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
   
   window->Desenhar(0,0);
   return(SKILLW_OTHER);
}

