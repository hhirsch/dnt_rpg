/*************************************************************************
 *  DccNiTghtmare is public domain. Do whatever you want with this code. *
 *************************************************************************/

#include "aspectwindow.h"

/**************************************************************
 *                          Constructor                       *
 **************************************************************/
aspectWindow::aspectWindow(character* dude, interface* inter)
{
   glDisable(GL_LIGHTING);
   SDL_ShowCursor(SDL_ENABLE);

   curImage = 0;
   images = NULL;
   loadImages();

   usedCharacter = dude;
   window = inter->insertWindow(276,206,531,320,"Aspect"
                                /*language.ASPECTW_TITLE.c_str()*/,1,1);

   /* Portrait Figure */
   figurePortrait = window->objects->InserirFigura(5,25,0,0,NULL);
   figurePortrait->fig = images[curImage].image;

   /* Previous Image Button */
   buttonPreviousImage = window->objects->InserirBotao(5,90,19,108,
                                                   window->Cores.corBot.R,
                                                   window->Cores.corBot.G,
                                                   window->Cores.corBot.B,
                                                   "<",0, NULL);
   /* Next Image Button */
   buttonNextImage = window->objects->InserirBotao(55,90,69,108,
                                                   window->Cores.corBot.R,
                                                   window->Cores.corBot.G,
                                                   window->Cores.corBot.B,
                                                   ">",0, NULL);
   /* Contorn to the previous and next buttons */
   window->objects->InserirQuadroTexto(20,90,54,108,1,"");

   /* Name Text */
   window->objects->InserirQuadroTexto(72,25,112,39,1,"Name:");
   textName = window->objects->InserirBarraTexto(113,25,251,39,"Bruno",0,NULL);

   /* Age Text */
   window->objects->InserirQuadroTexto(72,40,112,54,1,"Age:");
   textAge = window->objects->InserirBarraTexto(113,40,143,54,"20",0,NULL);

   /* Sex Selectors */
   window->objects->InserirQuadroTexto(72,55,112,69,1,"Sex:");
   cxSelSexF = window->objects->insertCxSel(113, 57, false);
   window->objects->InserirFigura(125,55,0,0,
                                  "../data/texturas/aspectw/sex_f.png");
   cxSelSexM = window->objects->insertCxSel(143, 57, true);
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
   /* Delete all images */
   if(images)
   {
      delete[] images;
   }
}

/**************************************************************
 *                          loadImages                        *
 **************************************************************/
void aspectWindow::loadImages()
{
   FILE* f;
   int i;
   char buffer[256];

   totalImages = 0;

   if( !(f = fopen("../data/characters/portraits/portraits.lst", "r")) )
   {
      printf("Can't open the list with portraits: portraits.lst!\n");
      return;
   }

   fscanf(f,"%d", &totalImages);
   images = new portraitImage[totalImages];

   for(i = 0; i < totalImages; i++)
   {
      fscanf(f,"%s", &buffer[0]);
      images[i].imageFile = buffer;
      images[i].image = IMG_Load(images[i].imageFile.c_str());
   }
}

/**************************************************************
 *                             treat                          *
 **************************************************************/
int aspectWindow::treat(Tobjeto* object, int eventInfo, interface* inter)
{
   if(eventInfo == BOTAOPRESSIONADO)
   {
      /* Confirm */
      if(object == (Tobjeto*) buttonConfirm)
      {
         /* Save Things to the character */
         usedCharacter->definePortrait(images[curImage].imageFile);
         /* Define Sex */
         if(cxSelSexF->isSelected())
         {
            usedCharacter->sexType = SEX_FEMALE;
         }
         else if(cxSelSexM->isSelected())
         {
            usedCharacter->sexType = SEX_MALE;
         }
         else
         {
            usedCharacter->sexType = SEX_OTHER;
         }
         /* Define Name */
         usedCharacter->nome = textName->texto;
         /* Define Age */
         sscanf(textAge->texto.c_str(),"%d", &usedCharacter->age);

         /* Close Window */
         figurePortrait->fig = NULL;
         inter->closeWindow(window);
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CONFIRM);
      }
      /* Cancel */
      else if(object == (Tobjeto*) buttonCancel) 
      {
         figurePortrait->fig = NULL;
         inter->closeWindow(window);
         window = NULL;
         glEnable(GL_LIGHTING);
         SDL_ShowCursor(SDL_DISABLE);
         return(ASPECTW_CANCEL);
      }
      /* Previous Image */
      else if(object == (Tobjeto*) buttonPreviousImage)
      {
         curImage--;
         if(curImage < 0)
         {
            curImage = totalImages-1;
         }
         figurePortrait->fig = images[curImage].image;
      }
      /* Next Image */
      else if(object == (Tobjeto*) buttonNextImage)
      {
         curImage++;
         if(curImage >= totalImages)
         {
            curImage = 0;
         }
         figurePortrait->fig = images[curImage].image;
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

