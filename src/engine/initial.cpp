/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "initial.h"
#include <SDL/SDL_image.h>

int result;
int pronto;

initialScreen::initialScreen()
{
   gui  = new interface(NULL);
}

initialScreen::~initialScreen()
{
   if(gui)
      delete(gui);
}

int botaoNovo(void *jan,void *ljan,SDL_Surface *screen)
{
   result = NEW_GAME;
   pronto = 1;
   return(1);
}

int botaoContinuar(void *jan,void *ljan,SDL_Surface *screen)
{
   result = CONTINUE_GAME;
   pronto = 1;
   return(1);
}

int botaoSalvar(void *jan,void *ljan,SDL_Surface *screen)
{
   result = SAVE_GAME;
   return(1);
}

int botaoCarregar(void *jan,void *ljan,SDL_Surface *screen)
{
   result = LOAD_GAME;
   return(1);
}

int botaoSair(void *jan,void *ljan,SDL_Surface *screen)
{
   result = EXIT_GAME;
   pronto = 1;
   return(1);
}


int initialScreen::Execute(int Status,GLdouble proj[16],
                           GLdouble modl[16], GLint viewPort[4])
{
   pronto = 0;
   janela* jan = gui->ljan->InserirJanela(335,235,462,362,
                                          "0.0.2",1,1,
                                          NULL,NULL);  
   jan->fechavel = 0;
   jan->movivel = 0;
   if(Status == ON_INIT)
   {
      jan->objetos->InserirBotao(30,20,98,38,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "New",1,&botaoNovo);
   }
   else
   {
      jan->objetos->InserirBotao(30,20,98,38,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "Continue",1,&botaoContinuar);
   }
   jan->objetos->InserirBotao(30,40,98,58,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "Save",1,&botaoSalvar);
   jan->objetos->InserirBotao(30,60,98,78,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "Load",1,&botaoCarregar);
   jan->objetos->InserirBotao(30,80,98,98,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "Options",1,NULL);
   jan->objetos->InserirBotao(30,100,98,118,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 "Exit",1,&botaoSair);
 
   jan->Abrir(gui->ljan);


   glDisable(GL_LIGHTING);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   /* Load backImage */
   SDL_Surface* img = IMG_Load("../data/texturas/inicio.png");
   SDL_Surface* fig = SDL_CreateRGBSurface(SDL_HWSURFACE,
                       img->w,img->h,32,
                       0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
   SDL_BlitSurface(img,NULL,fig,NULL);
   SDL_FreeSurface(img);
   GLuint tituloID;
   carregaTextura(fig,&tituloID);
   SDL_FreeSurface(fig);

   Uint8 *keys;
   int x,y;

   SDL_ShowCursor(SDL_ENABLE);
   while (!pronto)
   {
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      Uint8 Mbotao = SDL_GetMouseState(&x,&y);
      AtualizaTela2D(tituloID,proj,modl,viewPort,0,0,799,599,0.012);
      gui->ManipulaEventos(x,y,Mbotao,keys);
      gui->Desenhar(proj,modl,viewPort);
      glFlush();
      SDL_GL_SwapBuffers();
   }


   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);
   return(result);
}
