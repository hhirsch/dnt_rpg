#include "alignwindow.h"


alignWindow::alignWindow(aligns* alg, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalAligns = alg;
   actualAlign = externalAligns->getAlignByInteger(0);
   
   /* create window */
   window = inter->ljan->InserirJanela(276,186,531,441,
                                       "Alignment & Tendency",
                                       1,1,NULL,NULL);
   /* Align Image */
   alignImage = window->objects->InserirFigura(99,185,0,0,NULL);   
   alignImage->fig = actualAlign->image;

   /* Align Description */
   textDesc = window->objects->InserirQuadroTexto(5,42,250,180,1,
                                              actualAlign->description.c_str());
   textDesc->Cores.corTexto.R = 20;
   textDesc->Cores.corTexto.G = 29;
   textDesc->Cores.corTexto.B = 217;
   textDesc->Cores.corBot.R = textDesc->Cores.corCont[2].R; 
   textDesc->Cores.corBot.G = textDesc->Cores.corCont[2].G;
   textDesc->Cores.corBot.B = textDesc->Cores.corCont[2].B;
   textDesc->fonte = FMINI;
   textDesc->tamFonte = 1;

   /* Name and Selectors */
   buttonPrevious = window->objects->InserirBotao(5,19,19,37,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   buttonNext = window->objects->InserirBotao(236,19,250,37,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   textName = window->objects->InserirQuadroTexto(20,19,235,37,1, 
                                                  actualAlign->name.c_str());
   textName->fonte = FMINI;
   textName->tamFonte = 1;

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,225,251,244, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(8,225,78,244, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);

   /* Open Skill Window */
   window->ptrExterno = &window;
   window->fechavel = false;
   window->Abrir(inter->ljan);
}

int alignWindow::treat(Tobjeto* object, int eventInfo, 
                       interface* inter, align** actual)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if( (object == (Tobjeto*) buttonNext) || 
          (object == (Tobjeto*) buttonPrevious))
      {
         if(object == (Tobjeto*) buttonNext)
         {
            actualAlign = actualAlign->next;
         }
         else
         {
            actualAlign = actualAlign->previous;
         }
         textName->texto = actualAlign->name;
         textDesc->texto = actualAlign->description;
         alignImage->fig = actualAlign->image;
         window->Desenhar(0,0);
      }
      else if(object == (Tobjeto*) buttonConfirm)
      {
         alignImage->fig = NULL;
         *actual = actualAlign;
         window->Fechar(inter->ljan);
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CONFIRM);
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
         alignImage->fig = NULL; //to not delete skill images
         window->Fechar(inter->ljan);
         *actual = NULL;
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ALIGNW_CANCEL);
      }
   }
   return(ALIGNW_OTHER);
}

