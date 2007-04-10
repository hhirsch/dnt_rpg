/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "aspectwindow.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
aspectWindow::aspectWindow(personagem* character, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   usedCharacter = character;
   window = inter->insertWindow(276,206,531,320,"Aspect"
                                /*language.ASPECTW_TITLE.c_str()*/,1,1);

   /* Portrait Figure */
   figurePortrait = window->objects->InserirFigura(5,25,68,88,NULL);

   /* Previous Image Button */
   buttonPreviousImage = window->objects->InserirBotao(5,90,19,108,
                                                   window->Cores.corBot.R,
                                                   window->Cores.corBot.G,
                                                   window->Cores.corBot.B,
                                                   "<",0, NULL);
   /* Next Image Button */
   buttonNextImage = window->objects->InserirBotao(54,90,68,108,
                                                   window->Cores.corBot.R,
                                                   window->Cores.corBot.G,
                                                   window->Cores.corBot.B,
                                                   ">",0, NULL);
   /* Contorn to the previous and next buttons */
   window->objects->InserirQuadroTexto(20,90,53,108,1,"");

   /* Name Text */
   window->objects->InserirQuadroTexto(72,25,112,39,1,"Name:");
   textName = window->objects->InserirBarraTexto(113,25,251,39,"Jose",0,NULL);

   /* Age Text */
   window->objects->InserirQuadroTexto(72,40,112,54,1,"Age:");
   textAge = window->objects->InserirBarraTexto(113,40,143,54,"20",0,NULL);

   /* Sex Selectors */
   window->objects->InserirQuadroTexto(72,55,112,69,1,"Sex:");
   cxSelSexF = window->objects->insertCxSel(113, 57, true);
   window->objects->InserirFigura(125,55,0,0,
                                  "../data/texturas/aspectw/sex_f.png");
   cxSelSexM = window->objects->insertCxSel(143, 57, false);
   window->objects->InserirFigura(155,55,0,0,
                                  "../data/texturas/aspectw/sex_m.png");
   cxSelSexO = window->objects->insertCxSel(173, 57, false);
   window->objects->InserirFigura(185,55,0,0,
                                  "../data/texturas/aspectw/sex_o.png");

   /* Confirm Button */
   buttonConfirm = window->objects->InserirBotao(181,90,251,108, 
                                window->Cores.corBot.R,
                                window->Cores.corBot.G,window->Cores.corBot.B,
                                language.SKILL_CONFIRM.c_str(),1,NULL);
   
   /* Cancel Button */
   buttonCancel = window->objects->InserirBotao(72,90,140,108, 
                                 window->Cores.corBot.R,
                                 window->Cores.corBot.G,window->Cores.corBot.B,
                                 language.SKILL_CANCEL.c_str(),1,NULL);

   window->ptrExterno = &window;
   window->fechavel = false;
   inter->openWindow(window);
}

/**************************************************************
 *                          Destructor                        *
 **************************************************************/
aspectWindow::~aspectWindow()
{
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int aspectWindow::treat(Tobjeto* object, int eventInfo, interface* inter)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      if(object == (Tobjeto*) buttonConfirm)
      {
         inter->closeWindow(window);
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CONFIRM);
      }
      else if(object == (Tobjeto*) buttonCancel) 
      {
          inter->closeWindow(window);
          window = NULL;
          glEnable(GL_LIGHTING);
          SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CANCEL);
      }
   }
   else if(eventInfo == CXSELMODIFICADA)
   {
      /* Female */
      if(object == (Tobjeto*) cxSelSexF)
      {
         if(cxSelSexF->isSelected())
         {
            cxSelSexM->setSelection(false);
         }
         else
         {
            cxSelSexM->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Male */
      if(object == (Tobjeto*) cxSelSexM)
      {
         if(cxSelSexM->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexO->setSelection(false);
      }
      /* Other */
      if(object == (Tobjeto*) cxSelSexO)
      {
         if(cxSelSexO->isSelected())
         {
            cxSelSexF->setSelection(false);
         }
         else
         {
            cxSelSexF->setSelection(true);
         }
         cxSelSexM->setSelection(false);
      }
   }
   window->Desenhar(0,0);
   return(ASPECTW_OTHER);
}

