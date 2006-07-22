/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "initial.h"
#include <SDL/SDL_image.h>
#include "../lang/lang.h"

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

int botaoOpcoes(void *jan,void *ljan,SDL_Surface *screen)
{
   result = OPTIONS;
   pronto = 1;
   return(1);
}

int initialScreen::Execute(int Status,GLdouble proj[16],
                           GLdouble modl[16], GLint viewPort[4],
                           GLuint* tituloID)
{
   Uint32 tempo;
   Uint32 tempoAnterior = 0;
   pronto = 0;
   Tobjeto* object = NULL;
   janela* jan = gui->ljan->InserirJanela(335,235,462,362,
                                          language.VERSION.c_str(),1,1,
                                          NULL,NULL);  
   jan->fechavel = 0;
   //jan->movivel = 0;
   if(Status == ON_INIT)
   {
      jan->objects->InserirBotao(30,20,98,38,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_NEW.c_str(),1,&botaoNovo);
   }
   else
   {
      jan->objects->InserirBotao(30,20,98,38,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_CONTINUE.c_str(),
                                 1,&botaoContinuar);
   }
   jan->objects->InserirBotao(30,40,98,58,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_SAVE.c_str(),
                                 1,&botaoSalvar);
   jan->objects->InserirBotao(30,60,98,78,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_LOAD.c_str(),
                                 1,&botaoCarregar);
   jan->objects->InserirBotao(30,80,98,98,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_OPTIONS.c_str(),
                                 1,&botaoOpcoes);
   jan->objects->InserirBotao(30,100,98,118,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_EXIT.c_str(),
                                 1,&botaoSair);
 
   jan->Abrir(gui->ljan);


   glDisable(GL_LIGHTING);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   /* Load backImage */
   SDL_Surface* img = IMG_Load("../data/texturas/inicio.png");
   //GLuint tituloID;
   carregaTextura(img,tituloID);
   SDL_FreeSurface(img);

   Uint8 *keys;
   int x,y;

   SDL_ShowCursor(SDL_ENABLE);
   while (!pronto)
   {
      tempo = SDL_GetTicks();
      if(tempo - tempoAnterior >= 20) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         glClearColor(0,0,0,1);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         AtualizaTela2D(*tituloID,proj,modl,viewPort,0,0,799,599,0.012);
         gui->manipulateEvents(x,y,Mbotao,keys, object);
         gui->draw(proj,modl,viewPort);
         glFlush();
         SDL_GL_SwapBuffers();
      }
      else if(19 - (tempo - tempoAnterior) > 0 ) 
      {
         SDL_Delay(19 - (tempo - tempoAnterior) );
      }
   }

   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);
   return(result);
}
