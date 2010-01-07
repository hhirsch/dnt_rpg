#include "message.h"

/******************************************************************
 *                        messageForTool                          *
 ******************************************************************/
string messageForTool(int tool)
{
   switch(tool)
   {
      case TOOL_SIM_PLAY:
      {
         return("Simulation Started");
         break;
      }
      case TOOL_SIM_PAUSE:
      {
         return("Simulation Paused");
         break;
      }
      case TOOL_SIM_STOP:
      {
         return("Simulation Stoped");
         break;
      }
      case TOOL_PATTERN_ADD:
      {
         return("Add Agent, select way points and press right mouse button");
         break;
      }
      case TOOL_POTENTIAL_ADD:
      {
         return("Put potential Agent on initial Position");
         break;
      }
      case TOOL_OBSTACLE_ADD:
      {
         return("Put obstacle on position. Right Mouse Button to Next Object, Middle to Rotate. ");
         break;
      }
      case TOOL_GOAL_ADD:
      {
         return("Define Potential Agents Goal Position");
         break;
      }
      case TOOL_TP3_BRIEF_ADD:
      {
         return("Put briefcase on position. Middle mouse button to Rotate.");
         break;
      }
      case TOOL_TP3_POLICE_ADD:
      {
         return("Add Police Agent, select way points and press right mouse button");
         break;
      }
      case TOOL_TP3_ROGUE_ADD:
      {
         return("Put Rogue Agent on initial Position");
         break;
      }
      case TOOL_TP3_PSDB_ADD:
      {
         return("Define PSDB bandit's area");
         break;
      }
      case TOOL_TP3_PT_ADD:
      {
         return("Define PT bandit's area");
         break;
      }
      case TOOL_TP3_PFL_ADD:
      {
         return("Define PFL bandit's area");
         break;
      }
      case TOOL_TP3_CONGRESS_ADD:
      {
         return("Define bandit's work area");
         break;
      }
      case TOOL_TP3_FEDERAL_ADD:
      {
         return("Define Federal Police's area");
         break;
      }
   }
   return("DccNiTghtmare!");
}

/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   guiInterface* gui = new guiInterface("");
   janela* getWindow;
   botao* okButton;
   botao* cancelButton;
   barraTexto* getText = NULL;
   bool quit = false;
   string returnStr;
   Uint8 mButton;
   Uint8* keys;
   int mouseX, mouseY;

   getWindow = gui->ljan->InserirJanela(300,200,500,262,title.c_str(),1,1,
                                        NULL,NULL);
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
   getWindow->Abrir(gui->ljan);

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
      gui->draw(proj,modl,viewPort);
      glEnable(GL_LIGHTING);
      glEnable(GL_DEPTH_TEST);

      glFlush();
      SDL_GL_SwapBuffers();

      SDL_Delay(20);

   }

   delete(gui);

   return(returnStr);

}

