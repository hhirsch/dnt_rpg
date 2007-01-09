#include "alignwindow.h"


alignWindow::alignWindow(aligns* alg, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);
   
   externalAligns = alg;
   actualAlign = externalAligns->getAlignByInteger(0);
   
   /* create window */
   window = inter->ljan->InserirJanela(270,186,537,441,
                                       language.ALIGNW_TITLE.c_str(),
                                       1,1,NULL,NULL);
   /* Align Image */
   alignImage = window->objects->InserirFigura(111,185,0,0,NULL);   
   alignImage->fig = actualAlign->image;

   /* Align Description */
   textDesc = window->objects->InsertRolBar(5,38,262,180, 
                                            actualAlign->description.c_str());
      
   /* Name and Selectors */
   buttonPrevious = window->objects->InserirBotao(5,19,19,37,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 "<",0,NULL);
   buttonNext = window->objects->InserirBotao(248,19,262,37,
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 ">",0,NULL);
   textName = window->objects->InserirQuadroTexto(20,19,247,37,1, 
                                                  actualAlign->name.c_str());
   textName->fonte = FMINI;
   textName->tamFonte = 1;

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(203,225,263,244, 
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
         textDesc->setText(actualAlign->description);
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

