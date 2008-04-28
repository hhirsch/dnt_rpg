#include "message.h"

/******************************************************************
 *                        messageForTool                          *
 ******************************************************************/
string messageForTool(int tool)
{
   switch(tool)
   {
      case TOOL_TERRAIN_UP: 
         return("Select Square Vertex to Up"); 
      break;
      case TOOL_TERRAIN_NIVELATE:
         return("Select Area to Nivelate");
      break;
      case TOOL_TERRAIN_DOWN:
         return("Select Square Vertex to Down");
      break;
      case TOOL_TERRAIN_TEXTURE:
         return("Select Square to Texturize");
      break;
      case TOOL_PORTAL_ADD:
         return("Select Area to Add Portal (Max One portal per Square)");
      break;
      case TOOL_PORTAL_TAG:
         return("Select Portal to Define Destiny");
      break;
      case TOOL_PORTAL_DOOR:
         return("Put Door on Wall");
      break;
      case TOOL_WALL_ADD_X:
         return("Draw The Wall on X Axis (press B tor Add only on Square Borders)");
      break;
      case TOOL_WALL_ADD_Z:
         return("Draw The Wall on Z axis (press B to Add only on Square Borders)");
      break;
      case TOOL_WALL2_ADD_X:
         return("Draw The little Wall on X Axis (press B to Add only on Square Borders)");
      break;
      case TOOL_WALL2_ADD_Z:
         return("Draw The little Wall on Z Axis (press B to Add only on Square Borders)");
      break;
      case TOOL_WALL_TEXTURE:
         return("Select a Wall to Change Texture");
      break;
      case TOOL_WALL_LESS_Y_TEXTURE:
         return("Select a Wall to Less Y Texture Repeat");
      break;
      case TOOL_WALL_MORE_Y_TEXTURE:
         return("Select a Wall to More Y Texture Repeat");
      break;
      case TOOL_WALL_LESS_X_TEXTURE:
         return("Select a Wall to Less X Texture Repeat");
      break;
      case TOOL_WALL_MORE_X_TEXTURE:
         return("Select a Wall to More X Texture Repeat");
      break;
      case TOOL_WALL_LESS_Z_TEXTURE:
         return("Select a Wall to Less Z Texture Repeat");
      break;
      case TOOL_WALL_MORE_Z_TEXTURE:
         return("Select a Wall to More Z Texture Repeat");
      break;
      case TOOL_OBSTACLE_ADD:
         return("Put Object on Map with Left Mouse Button. Right and Middle Buttons rotate");
      break;
      case TOOL_NPC_ADD:
         return("Put NPC on Map with left mouse Button.");
      break;
   }
   return("DccNiTghtmare!");
}

#if 0
/******************************************************************
 *                        getStringForUser                        *
 ******************************************************************/
string getStringFromUser(string title, string previous,
                         GLdouble proj[16],GLdouble modl[16],GLint viewPort[4])
{
   guiInterface* gui = new guiInterface(NULL);
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

#endif

