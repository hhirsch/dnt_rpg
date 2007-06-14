#include "messages.h"
#include "../engine/util.h"

/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   interface* gui = new interface(NULL);
   janela* getWindow;
   botao* okButton;
   botao* cancelButton;
   barraTexto* getText = NULL;
   bool quit = false;
   string returnStr;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;

   getWindow = gui->insertWindow(300,200,500,262,title.c_str(),1,1);
   okButton = getWindow->objects->InserirBotao(40,37,95,55,
                                               getWindow->Cores.corBot.R,
                                               getWindow->Cores.corBot.G,
                                               getWindow->Cores.corBot.B,
                                               "Ok",1,NULL);
   cancelButton = getWindow->objects->InserirBotao(100,37,155,55,
                                                   getWindow->Cores.corBot.R,
                                                   getWindow->Cores.corBot.G,
                                                   getWindow->Cores.corBot.B,
                                                   "Cancel",1,NULL);
   getText = getWindow->objects->InserirBarraTexto(10,17,190,33,
                                                   previous.c_str(),0,
                                                   NULL);
   getWindow->movivel = 0;
   getWindow->ptrExterno = &getWindow;
   gui->openWindow(getWindow);
   

   while(!quit)
   {
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      Tobjeto* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

      if(eventInfo == BOTAOPRESSIONADO)
      {
         if(object == (Tobjeto*) okButton)
         {
            returnStr = getText->texto;
            quit =true;
         }
         else if(object == (Tobjeto*) cancelButton)
         {
            returnStr = previous;
            quit = true;
         }
      }

      if(getWindow == NULL)
      {
         quit = true;
      }

      /* Draw */
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glPushMatrix();
        draw2DMode();
        gui->draw(proj,modl,viewPort);
	draw3DMode(FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);

   return(returnStr);

}

/******************************************************************
 *                            showMessage                         *
 ******************************************************************/
int getOptionFromUser(string title, string message, string opt1, string opt2,
                      GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   interface* gui = new interface(NULL);
   janela* getWindow;
   botao* opt1Button;
   botao* opt2Button;
   quadroTexto* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sizeX = message.length()*7;
   if(sizeX < 180)
   {
      sizeX = 180;
   }
   int med = sizeX / 2;
   int ret = -1;

   getWindow = gui->insertWindow(300,200,300+sizeX,262,
                                 title.c_str(),1,1);
   opt1Button = getWindow->objects->InserirBotao(med-80,37,med-10,55,
                                                 getWindow->Cores.corBot.R,
                                                 getWindow->Cores.corBot.G,
                                                 getWindow->Cores.corBot.B,
                                                 opt1.c_str(),1,NULL);
   opt2Button = getWindow->objects->InserirBotao(med+10,37,med+80,55,
                                                 getWindow->Cores.corBot.R,
                                                 getWindow->Cores.corBot.G,
                                                 getWindow->Cores.corBot.B,
                                                 opt2.c_str(),1,NULL);                  
                                                 
   quadText = getWindow->objects->InserirQuadroTexto(10,17,sizeX-10,33,0,
                                                      message.c_str());
   getWindow->movivel = 0;
   getWindow->ptrExterno = &getWindow;
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      Tobjeto* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

      if(eventInfo == BOTAOPRESSIONADO)
      {
         if(object == (Tobjeto*) opt1Button)
         {
            quit =true;
            ret = 1;
         }
         else if(object == (Tobjeto*) opt2Button)
         {
            quit =true;
            ret = 2;
         }
      }

      if(getWindow == NULL)
      {
         quit = true;
      }

      /* Draw */
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glPushMatrix();
        draw2DMode();
        gui->draw(proj,modl,viewPort);
	draw3DMode(FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);
   
   return(ret);
}

/******************************************************************
 *                            showMessage                         *
 ******************************************************************/
void showMessage(string title, string message,
                 GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   interface* gui = new interface(NULL);
   janela* getWindow;
   botao* okButton;
   quadroTexto* quadText = NULL;
   bool quit = false;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;
   int sizeX = message.length()*7;
   int med = sizeX / 2;

   getWindow = gui->insertWindow(300,200,300+sizeX,262,
                                 title.c_str(),1,1);
   okButton = getWindow->objects->InserirBotao(med-28,37,med+28,55,
                                               getWindow->Cores.corBot.R,
                                               getWindow->Cores.corBot.G,
                                               getWindow->Cores.corBot.B,
                                               "Ok",1,NULL);
   quadText = getWindow->objects->InserirQuadroTexto(10,17,sizeX-10,33,0,
                                                      message.c_str());
   getWindow->movivel = 0;
   getWindow->ptrExterno = &getWindow;
   gui->openWindow(getWindow);

   while(!quit)
   {
      int eventInfo;
      SDL_PumpEvents();
      keys = SDL_GetKeyState(NULL);
      mButton = SDL_GetMouseState(&mouseX,&mouseY);

      Tobjeto* object;
      object = gui->manipulateEvents(mouseX, mouseY, mButton, keys, &eventInfo);

      if(eventInfo == BOTAOPRESSIONADO)
      {
         if(object == (Tobjeto*) okButton)
         {
            quit =true;
         }
      }

      if(getWindow == NULL)
      {
         quit = true;
      }

      /* Draw */
      glDisable(GL_LIGHTING);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_BLEND);
      glDisable(GL_FOG);
      glPushMatrix();
        draw2DMode();
        gui->draw(proj,modl,viewPort);
	draw3DMode(FARVIEW);
      glPopMatrix();
      glEnable(GL_FOG);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);

}


