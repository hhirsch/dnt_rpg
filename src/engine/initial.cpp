/*
 *  DccNiTghtmare is public domain. Do whatever you want with this code.
 */

#include "initial.h"
#include "util.h"
#include <SDL/SDL_image.h>
#include "../lang/lang.h"

initialScreen::initialScreen()
{
   gui  = new interface(NULL);
   buttonNew = NULL;
   buttonContinue = NULL;
   buttonSave = NULL;
   buttonLoad = NULL;
   buttonOptions = NULL;
   buttonExit = NULL;
}

initialScreen::~initialScreen()
{
   if(gui)
      delete(gui);
}


int initialScreen::Execute(int Status,GLdouble proj[16],
                           GLdouble modl[16], GLint viewPort[4],
                           GLuint tituloID, sound* snd)
{
   Uint32 tempo;
   Uint32 tempoAnterior = 0;
   done = false;
   guiObject* object = NULL;
   int eventInfo = NADA;
   janela* jan = gui->insertWindow(335,235,462,362,
                                   language.VERSION.c_str(),1,1);  
   jan->fechavel = 0;
   //jan->movivel = 0;
   if(Status == ON_INIT)
   {
      buttonNew = jan->objects->InserirBotao(30,20,98,38,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_NEW.c_str(),1,NULL);
   }
   else
   {
      buttonContinue = jan->objects->InserirBotao(30,20,98,38,
                                 jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_CONTINUE.c_str(),
                                 1,NULL);
   }
   buttonSave = jan->objects->InserirBotao(30,40,98,58,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_SAVE.c_str(),
                                 1,NULL);
   buttonLoad = jan->objects->InserirBotao(30,60,98,78,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_LOAD.c_str(),
                                 1,NULL);
   buttonOptions = jan->objects->InserirBotao(30,80,98,98,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_OPTIONS.c_str(),
                                 1,NULL);
   buttonExit = jan->objects->InserirBotao(30,100,98,118,jan->Cores.corBot.R, 
                                 jan->Cores.corBot.G,jan->Cores.corBot.B,
                                 language.INITIAL_EXIT.c_str(),
                                 1,NULL);
   gui->openWindow(jan);

   glDisable(GL_LIGHTING);
   glClearColor(0,0,0,1);
   glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

   Uint8 *keys;
   int x,y;

   SDL_ShowCursor(SDL_ENABLE);
   while (!done)
   {
      tempo = SDL_GetTicks();
      if((tempo - tempoAnterior) >= 20) 
      {
         tempoAnterior = tempo;
         SDL_PumpEvents();
         keys = SDL_GetKeyState(NULL);
         glClearColor(0,0,0,1);
         glClear ((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
         Uint8 Mbotao = SDL_GetMouseState(&x,&y);
         AtualizaTela2D(tituloID,proj,modl,viewPort,0,0,799,599,0.012);
         object = gui->manipulateEvents(x,y,Mbotao,keys, &eventInfo);
         if(eventInfo != NADA)
         {
             if(eventInfo == BOTAOPRESSIONADO)
             {
                 //snd->addSoundEffect(false, "../data/sndfx/menu/click.ogg");

                 if(object == (guiObject*) buttonNew)
                 {
                    result = NEW_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonContinue)
                 {
                    result = CONTINUE_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonLoad)
                 {
                    result = LOAD_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonSave)
                 {
                    result = SAVE_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonExit)
                 {
                    result = EXIT_GAME;
                    done = true;
                 }
                 else if(object == (guiObject*) buttonOptions)
                 {
                    result = OPTIONS;
                    done = true;
                 }
             }
         }
         glPushMatrix();
            draw2DMode();
            gui->draw(proj,modl,viewPort);
            draw3DMode(FARVIEW);
         glPopMatrix();
         glFlush();
         SDL_GL_SwapBuffers();
      }
      else if((19 - (int)(tempo - tempoAnterior)) > 0 ) 
      {
         SDL_Delay((19 - (tempo - tempoAnterior)) );
      }
   }

   gui->closeWindow(jan);

   glEnable(GL_LIGHTING);
   SDL_ShowCursor(SDL_DISABLE);
   return(result);
}

